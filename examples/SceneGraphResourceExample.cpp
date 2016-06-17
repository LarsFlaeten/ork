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

#include <iostream>
#include <fstream>

#include "ork/core/FileLogger.h"
#include "ork/resource/XMLResourceLoader.h"
#include "ork/render/FrameBuffer.h"
#include "ork/taskgraph/MultithreadScheduler.h"
#include "ork/scenegraph/SceneManager.h"
#include "ork/scenegraph/ShowLogTask.h"
#include "ork/ui/GlutWindow.h"

#include "examples/Main.h"

using namespace ork;

class SceneGraphResourceExample : public GlutWindow
{
public:
    ptr<SceneManager> manager;
    float fov;
    float alpha;
    float theta;
    float dist;

    SceneGraphResourceExample(const string &dir, const string &scene) :
        GlutWindow(Window::Parameters().size(1024, 768).depth(true)), fov(80.0), alpha(135), theta(45), dist(15)
    {
        FileLogger::File *out = new FileLogger::File("log.html");
        //Logger::DEBUG_LOGGER = new Logger("DEBUG");
        //Logger::DEBUG_LOGGER = new FileLogger("DEBUG", out, Logger::DEBUG);
        Logger::INFO_LOGGER = new FileLogger("INFO", out, Logger::INFO_LOGGER);
        Logger::WARNING_LOGGER = new FileLogger("WARNING", out, Logger::WARNING_LOGGER);
        Logger::ERROR_LOGGER = new FileLogger("ERROR", out, Logger::ERROR_LOGGER);

        ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
        resLoader->addPath(dir + "/textures");
        resLoader->addPath(dir + "/shaders");
        resLoader->addPath(dir + "/meshes");
        resLoader->addPath(dir + "/methods");
        resLoader->addPath(dir + "/scenes");

        ptr<ResourceManager> resManager = new ResourceManager(resLoader, 8);

        manager = new SceneManager();
        manager->setResourceManager(resManager);
        manager->setScheduler(new MultithreadScheduler());

        ptr<SceneNode> root = resManager->loadResource(scene).cast<SceneNode>();

        manager->setRoot(root);
        manager->setCameraNode("camera");
        manager->setCameraMethod("draw");
    }

    void redisplay(double t, double dt)
    {
        mat4f cameraToWorld = mat4f::rotatex(90);
        cameraToWorld = cameraToWorld * mat4f::rotatey(-alpha);
        cameraToWorld = cameraToWorld * mat4f::rotatex(-theta);
        cameraToWorld = cameraToWorld * mat4f::translate(vec3f(0.0, 0.0, dist));

        manager->getCameraNode()->setLocalToParent(cameraToWorld.cast<double>());

        ptr<FrameBuffer> fb = FrameBuffer::getDefault();
        fb->clear(true, false, true);

        manager->update(t, dt);
        manager->draw();

        GlutWindow::redisplay(t, dt);

        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->flush();
        }
    }

    void reshape(int x, int y)
    {
        ptr<FrameBuffer> fb = FrameBuffer::getDefault();
        fb->setViewport(vec4<GLint>(0, 0, x, y));
        fb->setDepthTest(true, LESS);

        float vfov = degrees(2 * atan(float(y) / float(x) * tan(radians(fov) / 2)));
        manager->setCameraToScreen(mat4d::perspectiveProjection(vfov, float(x) / float(y), 0.1f, 1e5f));

        GlutWindow::reshape(x, y);
        idle(false);
    }

    void idle(bool damaged)
    {
        GlutWindow::idle(damaged);
        if (damaged) {
            manager->getResourceManager()->updateResources();
        }
    }

    bool keyTyped(unsigned char c, modifier m, int x, int y)
    {
        if (c == 27) {
            ::exit(0);
        }
        return true;
    }

    bool mouseWheel(wheel b, modifier m, int x, int y)
    {
        dist = b == WHEEL_UP ? dist * 1.05f : dist / 1.05f;
        return true;
    }

    bool mousePassiveMotion(int x, int y)
    {
        alpha = float(x) / getWidth() * 360.0;
        theta = (float(y) / getHeight() - 0.5) * 180.0;
        return true;
    }

    bool specialKey(key k, modifier m, int x, int y)
    {
        switch (k) {
        case KEY_F1:
            ShowLogTask::enabled = !ShowLogTask::enabled;
            break;
        case KEY_F5:
            manager->getResourceManager()->updateResources();
            break;
        default:
            break;
        }
        return true;
    }

    static static_ptr<Window> app;
};

static_ptr<Window> SceneGraphResourceExample::app;

int sceneGraphResourceExample(int argc, char* argv[])
{
    assert(argc > 3);
    atexit(Object::exit);
    SceneGraphResourceExample::app = new SceneGraphResourceExample(string(argv[2]), string(argv[3]));
    SceneGraphResourceExample::app->start();
    return 0;
}

static MainFunction sceneGraphResourceMain("scenegraphresource", sceneGraphResourceExample);
