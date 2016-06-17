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

class SceneGraphExample : public GlutWindow
{
public:
    ptr<SceneManager> manager;
    float fov;
    float alpha;
    float theta;
    float dist;

    SceneGraphExample(const string &dir) :
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

        ptr<ResourceManager> resManager = new ResourceManager(resLoader);

        manager = new SceneManager();
        manager->setResourceManager(resManager);
        manager->setScheduler(new MultithreadScheduler());

        ptr<SceneNode> root = new SceneNode();

        ptr<SceneNode> camera = new SceneNode();
        camera->addFlag("camera");
        camera->addModule("material", resManager->loadResource("camera").cast<Module>());
        camera->addMethod("draw", new Method(resManager->loadResource("cameraMethod").cast<TaskFactory>()));
        root->addChild(camera);

        ptr<SceneNode> light = new SceneNode();
        light->setLocalToParent(mat4d::translate(vec3d(3, 3, 3)) * mat4d::rotatez(135) * mat4d::rotatex(45));
        light->addFlag("light");
        light->addModule("material", resManager->loadResource("spotlight").cast<Module>());
        light->addMethod("draw", new Method(resManager->loadResource("lightMethod").cast<TaskFactory>()));
        root->addChild(light);

        ptr<SceneNode> cube = new SceneNode();
        cube->setLocalToParent(mat4d::rotatez(15));
        cube->addFlag("object");
        cube->addMesh("geometry", resManager->loadResource("cube.mesh").cast<MeshBuffers>());
        cube->addModule("material", resManager->loadResource("plastic").cast<Module>());
        cube->addMethod("draw", new Method(resManager->loadResource("objectMethod").cast<TaskFactory>()));
        root->addChild(cube);

        ptr<SceneNode> plane = new SceneNode();
        plane->setLocalToParent(mat4d::translate(vec3d(0, 0, -2)) * mat4d::rotatez(180));
        plane->addFlag("object");
        plane->addMesh("geometry", resManager->loadResource("plane.mesh").cast<MeshBuffers>());
        plane->addModule("material", resManager->loadResource("texturedPlastic").cast<Module>());
        plane->addMethod("draw", new Method(resManager->loadResource("objectMethod").cast<TaskFactory>()));
        root->addChild(plane);

        ptr<SceneNode> log = new SceneNode();
        log->addFlag("overlay");
        log->addMethod("draw", new Method(resManager->loadResource("logMethod").cast<TaskFactory>()));
        root->addChild(log);

        ptr<SceneNode> info = new SceneNode();
        info->addFlag("overlay");
        info->addMethod("draw", new Method(resManager->loadResource("infoMethod").cast<TaskFactory>()));
        root->addChild(info);

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

        float vfov = degrees(2 * atan(float(y) / float(x)* tan(radians(fov / 2))));
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

static_ptr<Window> SceneGraphExample::app;

int sceneGraphExample(int argc, char* argv[])
{
    assert(argc > 2);
    atexit(Object::exit);
    SceneGraphExample::app = new SceneGraphExample(string(argv[2]));
    SceneGraphExample::app->start();
    return 0;
}

static MainFunction sceneGraphMain("scenegraph", sceneGraphExample);
