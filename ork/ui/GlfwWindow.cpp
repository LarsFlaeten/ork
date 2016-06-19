/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "ork/ui/GlfwWindow.h"

#include "ork/core/Logger.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <assert.h>

#include "DebugCallback.h"

using namespace std;

namespace ork
{

map<int, GlfwWindow*> GlfwWindow::INSTANCES;

GlfwWindow::GlfwWindow(const Parameters &params) : Window(params), glfwWindowHandle(NULL)
{
    // TODO
    // Implement glfw user pointers:
    // http://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback


//    int argc = 1;
//    char *argv[1] = { (char*) "dummy" };
	int x = INSTANCES.size();
    if (x == 0) {
        if(!glfwInit() && Logger::ERROR_LOGGER != NULL)
        {
            Logger::ERROR_LOGGER->log("UI", "Could not init GLFW");
            Logger::ERROR_LOGGER->flush();
            return;
        }

    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    GLFWwindow* gwd = glfwCreateWindow(params.width(), params.height(), params.name().c_str(), NULL, NULL);
    glfwWindowHandle = (void*)gwd;

    if(glfwWindowHandle == NULL && Logger::ERROR_LOGGER != NULL)
    {
        Logger::ERROR_LOGGER->log("UI", "Could not creat GLFW window!");
        Logger::ERROR_LOGGER->flush();
        glfwTerminate();
        return;
    }


/*    glutInitDisplayMode(GLUT_DOUBLE |
        (params.alpha() ? GLUT_ALPHA : 0) |
        (params.depth() ? GLUT_DEPTH : 0) |
        (params.stencil() ? GLUT_STENCIL : 0) |
        (params.multiSample() ? GLUT_MULTISAMPLE : 0));
*/	
    glGetError();
/*
#ifdef USEFREEGLUT
    //Init OpenGL context
    glutInitContextVersion(params.version().x, params.version().y);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | (params.debug() ? GLUT_DEBUG : 0));
#endif

    glutInitWindowSize(params.width(), params.height());
*/
    
    // Only allow single window for now
    // to simplify things
    // TODO: Allow formultiple windows
    // There is an implemntation here:
    // https://blog.gvnott.com/2013/05/18/tutorial-multiple-windows-with-glfw3-and-glew-mx/ 
    windowId = 1;
    if(INSTANCES.size()>0)
    {
        Logger::ERROR_LOGGER->log("UI", "Onlye one window instance allowed in curent implementation");
        Logger::ERROR_LOGGER->flush();
        glfwTerminate();
        return;
    }
    INSTANCES[windowId] = this;

    // Here we get the actual size we got from glfw
    // May not be the same as given as "hint"?
    int width, height;
    glfwGetWindowSize(gwd, &width, &height);
    size = vec2i(width, height);

    damaged = false;
    timer.start();
    t = 0.0;
    dt = 0.0;
/*
    if (params.width() == 0 && params.height() == 0) {
        glutFullScreen();
    }
    if (false) {
        glutCreateMenu(NULL);//do nothing, only used to avoid a warning
    }
*/     
    glGetError();

    // Set up callbacks
//    glutDisplayFunc(redisplayFunc);
//    glutReshapeFunc(reshapeFunc);
    glfwSetWindowSizeCallBack(gwd, reshapeFunc);
//    glutIdleFunc(idleFunc);
//    glutMouseFunc(mouseClickFunc);
//    glutMotionFunc(mouseMotionFunc);
//    glutPassiveMotionFunc(mousePassiveMotionFunc);
//    glutKeyboardFunc(keyboardFunc);
//    glutKeyboardUpFunc(keyboardUpFunc);
//    glutSpecialFunc(specialKeyFunc);
//    glutSpecialUpFunc(specialKeyUpFunc);
//    glutIgnoreKeyRepeat(1);
/*
    // should be mouse enter/leave events,
    // but implemented in freeglut with get/loose focus
    glutEntryFunc(focusFunc);
*/
    assert(glGetError() == 0);
    glewExperimental = GL_TRUE;
    glGetError();
    if((glewInit() != GLEW_OK || glGetError()!=0) && Logger::ERROR_LOGGER != NULL)
    {
        Logger::ERROR_LOGGER->log("UI", "Could not init GLEW!");
        Logger::ERROR_LOGGER->flush();
        glfwTerminate();
        return;
    }


    if (params.debug()) {
        assert(glDebugMessageCallbackARB != NULL);
        glDebugMessageCallbackARB(debugCallback, NULL);
    }
    
    
    // Lars F addtion 16.05.2016
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

GlfwWindow::~GlfwWindow()
{
    // Lars F addition 16.05.2016
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    
    INSTANCES.erase(windowId);
}

/*void GlfwWindow::shutDown()
{
	glfwTerminate();

}
*/
int GlfwWindow::getWidth() const
{
    return size.x;
}

int GlfwWindow::getHeight() const
{
    return size.y;
}

void GlfwWindow::start()
{
    //TODO:
    //Implement main loop here
}

void GlfwWindow::redisplay(double t, double dt)
{
    glfwSwapBuffers((GLFWwindow*)glfwWindowHandle);
    double newT = timer.end();
    this->dt = newT - this->t;
    this->t = newT;
}

void GlfwWindow::reshape(int x, int y)
{
    size = vec2i(x, y);
}

void GlfwWindow::idle(bool damaged)
{
    //glutPostRedisplay();
}

void GlfwWindow::redisplayFunc()
{
    // TODO: modify for multiple windows
    GlfwWindow* window = INSTANCES[1];
    window->redisplay(window->t, window->dt);
}

void GlfwWindow::reshapeFunc(GLFWwindow*, int w, int h)
{
    // TODO: modify for multiple windows
    INSTANCES[1]->reshape(x, y);
}

void GlfwWindow::idleFunc()
{
    // TODO: modify for multiple windows
    GlfwWindow *window = INSTANCES[1];
    // glutLayerGet(GLUT_NORMAL_DAMAGED) is not implemented in freeglut
    //window->idle(glutLayerGet(GLUT_NORMAL_DAMAGED) == 1);
    window->idle(window->damaged);
    window->damaged = false;
}

void GlfwWindow::mouseClickFunc(int b, int s, int x, int y)
{
/*    int m = glutGetModifiers();

#ifdef GLUT_WHEEL_UP
    if (b == GLUT_WHEEL_UP) {
#else
    if (b == GLUT_WHEEL_UP_BUTTON) {
        if (s == 0)
#endif
        INSTANCES[glutGetWindow()]->mouseWheel(WHEEL_UP, (modifier) m, x, y);
        return;
    }

#ifdef GLUT_WHEEL_DOWN
    if (b == GLUT_WHEEL_DOWN) {
#else
    if (b == GLUT_WHEEL_DOWN_BUTTON) {
        if (s == 0)
#endif
        INSTANCES[glutGetWindow()]->mouseWheel(WHEEL_DOWN, (modifier) m, x, y);
        return;
    }

    INSTANCES[glutGetWindow()]->mouseClick((button) b, (state) s, (modifier) m, x, y);
*/
}

void GlfwWindow::mouseMotionFunc(int x, int y)
{
     // TODO: modify for multiple windows
    INSTANCES[1]->mouseMotion(x, y);
}

void GlfwWindow::mousePassiveMotionFunc(int x, int y)
{
     // TODO: modify for multiple windows
    INSTANCES[1]->mousePassiveMotion(x, y);
}

void GlfwWindow::keyboardFunc(unsigned char c, int x, int y)
{
/*    int m = glutGetModifiers();
     // TODO: modify for multiple windows
    INSTANCES[1]->keyTyped(c, (modifier) m, x, y);
*/}

void GlfwWindow::keyboardUpFunc(unsigned char c, int x, int y)
{
/*    int m = glutGetModifiers();
     // TODO: modify for multiple windows
    INSTANCES[1]->keyReleased(c, (modifier) m, x, y);
*/}

void GlfwWindow::specialKeyFunc(int k, int x, int y)
{
/*    int m = glutGetModifiers();
     // TODO: modify for multiple windows
    INSTANCES[1]->specialKey((key) k, (modifier) m, x, y);
*/}

void GlfwWindow::specialKeyUpFunc(int k, int x, int y)
{
/*    int m = glutGetModifiers();
     // TODO: modify for multiple windows
    INSTANCES[1]->specialKeyReleased((key) k, (modifier) m, x, y);
*/}

void GlfwWindow::focusFunc(int focus)
{
    // TODO: modify for multiple windows
    INSTANCES[1]->damaged = (focus != 0);
}

}
