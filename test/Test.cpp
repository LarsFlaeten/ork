/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Copyright (c) 2008-2010 INRIA
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 */

/*
 * Authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "test/Test.h"

#include "ork/core/FileLogger.h"
#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

#if defined( _WIN64 ) || defined( _WIN32 )
#include "process.h"
#else
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#endif

using namespace ork;

TestSuite *TestSuite::getInstance()
{
    if (INSTANCE == NULL) {
        INSTANCE = new TestSuite();
    }
    return INSTANCE;
}

TestSuite *TestSuite::INSTANCE = NULL;

Test::Test(const char *name, testFunction test, int majorVersion)
{
    TestSuite::getInstance()->tests.push_back(test);
    TestSuite::getInstance()->testNames.push_back(name);
    TestSuite::getInstance()->testVersions.push_back(majorVersion);
}

const char* testName = NULL;

unsigned int passed = 0;

unsigned int failed = 0;

void test(bool result, const char *file, int line)
{
    if (result) {
        ++passed;
        printf("[OK]\n");
    } else {
        ++failed;
        printf("[FAILED]\n");
        //printf("[FAILED] (FILE %s AT LINE %d)\n", file, line);
    }
}

class TestWindow : public GlutWindow
{
public:
    const char* tests;

    unsigned int currentTest;

    TestWindow(const char *tests, int major = 3, int minor = 3) :
        GlutWindow(Window::Parameters().name("Test").size(128, 128).version(major, minor, true)),
        tests(tests), currentTest(0)
    {
        Logger::INFO_LOGGER = new FileLogger("INFO", new FileLogger::File("testLog.html"), NULL);
    }

    void redisplay(double t, double dt)
    {
        FrameBuffer::getDefault()->clear(true, false, false);
        if (currentTest < TestSuite::getInstance()->tests.size()) {
            testName = TestSuite::getInstance()->testNames[currentTest].c_str();
            if (strcmp(tests, "ALL") == 0 || strcmp(tests, testName) == 0) {
                printf("%s...%*s", testName, 60 - strlen(testName), "");
                fflush(NULL);
                if (TestSuite::getInstance()->testVersions[currentTest] <= FrameBuffer::getMajorVersion()) {
                    TestSuite::getInstance()->tests[currentTest]();
                } else {
                    printf("[SKIPPED]\n");
                }
                fflush(NULL);
            }
            currentTest++;
        } else {
            if (failed > 0) {
                if (failed + passed > 1) {
                    printf("\n\n%d test(s) FAILED (%d tests passed).\n", failed, passed);
                }
                ::exit(1);
            } else {
                if (passed > 1) {
                    printf("\n\nAll %d tests passed!\n", passed);
                }
                ::exit(0);
            }
        }

        GlutWindow::redisplay(t, dt);
    }

    void reshape(int x, int y)
    {
        FrameBuffer::getDefault()->setViewport(vec4<GLint>(0, 0, x, y));
        GlutWindow::reshape(x, y);
        idle(false);
    }

    static static_ptr<Window> app;
};

static_ptr<Window> TestWindow::app;

#if defined( _WIN64 ) || defined( _WIN32 )

bool testProcess(const char *cmd, int argc, const char *const *argv)
{
    return _spawnv(_P_WAIT, cmd, argv) == 0;
}

#else

using namespace std;

int testMain(int argc, char* argv[]);

bool testProcess(const char *cmd, int argc, const char *const *argv)
{
    pid_t w;
    pid_t pId = fork();
    int status;

    if (pId == -1) { // fork failed
        return false;
    }
    if (pId == 0) { // child
        return testMain(argc, (char**) argv) == 0;
    } else {
        do {
            w = waitpid(pId, &status, WCONTINUED | WUNTRACED);
            if (w == -1) {
                return false;
            }
            if (WIFEXITED(status)) {
               return status == 0;
            } else if (WIFSIGNALED(status)) {
               return false;
            } else if (WIFSTOPPED(status)) {
               return false;
            } else if (WIFCONTINUED(status)) {
               return false;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return false;
    }
}

#endif

int testMain(int argc, char* argv[])
{
    atexit(Object::exit);
    assert(argc > 1);
    bool GL4 = argc > 2 && strncmp(argv[2], "GL4", 3) == 0;
    if (strcmp(argv[1], "FORK") == 0) {
        if (strchr(argv[0], ' ') != NULL) {
            printf("Cannot launch tests from a path containing spaces ('%s')\n", argv[0]);
            ::exit(1);
        }
        unsigned int passed = 0;
        for (unsigned int i = 0; i < TestSuite::getInstance()->tests.size(); ++i) {
            const char* args[4] = {
                argv[0],
                TestSuite::getInstance()->testNames[i].c_str(),
                GL4 ? "GL4" : "GL3",
                NULL
            };
            passed += testProcess(argv[0], 3, args) ? 1 : 0;
        }
        if (passed < TestSuite::getInstance()->tests.size()) {
            printf("\n\n%d test(s) FAILED (%d tests passed).\n", TestSuite::getInstance()->tests.size() - passed, passed);
            ::exit(1);
        } else {
            printf("\n\nAll %d tests passed!\n", passed);
            ::exit(0);
        }
        return 0;
    } else {
        TestWindow::app = GL4 ? new TestWindow(argv[1], 4, 0) : new TestWindow(argv[1], 3, 3);
        TestWindow::app->start();
        return 0;
    }
}

int main(int argc, char* argv[])
{
    return testMain(argc, argv);
}
