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

#include <time.h>

#include "ork/core/FileLogger.h"
#include "ork/resource/XMLResourceLoader.h"
#include "ork/resource/ResourceManager.h"
#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

#include "examples/Main.h"

using namespace ork;

class ResourceExample : public GlutWindow
{
public:
    ptr<ResourceManager> resManager;
    ptr<MeshBuffers> cube;
    ptr<MeshBuffers> plane;
    ptr<Program> p1;
    ptr<Program> p2;
    ptr<Uniform3f> worldCamera;
    ptr<UniformMatrix4f> localToWorld1;
    ptr<UniformMatrix4f> localToScreen1;
    ptr<UniformMatrix4f> localToWorld2;
    ptr<UniformMatrix4f> localToScreen2;
    ptr<FrameBuffer> fb;
    float fov;
    float alpha;
    float theta;
    float dist;

    ResourceExample(const string &dir) :
        GlutWindow(Window::Parameters().size(1024, 768).depth(true)), fov(80.0), alpha(135), theta(45), dist(15)
    {
        FileLogger::File *out = new FileLogger::File("log.html");
        Logger::INFO_LOGGER = new FileLogger("INFO", out, Logger::INFO_LOGGER);
        Logger::WARNING_LOGGER = new FileLogger("WARNING", out, Logger::WARNING_LOGGER);
        Logger::ERROR_LOGGER = new FileLogger("ERROR", out, Logger::ERROR_LOGGER);

        ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
        resLoader->addPath(dir + "/textures");
        resLoader->addPath(dir + "/shaders");
        resLoader->addPath(dir + "/meshes");

        resManager = new ResourceManager(resLoader);

        cube = resManager->loadResource("cube.mesh").cast<MeshBuffers>();
        plane = resManager->loadResource("plane.mesh").cast<MeshBuffers>();

        p1 = resManager->loadResource("camera;spotlight;plastic;").cast<Program>();
        p2 = resManager->loadResource("camera;spotlight;texturedPlastic;").cast<Program>();

        worldCamera = p1->getUniform3f("worldCameraPos");
        localToWorld1 = p1->getUniformMatrix4f("localToWorld");
        localToScreen1 = p1->getUniformMatrix4f("localToScreen");
        localToWorld2 = p2->getUniformMatrix4f("localToWorld");
        localToScreen2 = p2->getUniformMatrix4f("localToScreen");

        fb = FrameBuffer::getDefault();
        fb->setDepthTest(true, LESS);
    }

    void redisplay(double t, double dt)
    {
        fb->clear(true, false, true);

        mat4f cameraToWorld = mat4f::rotatex(90);
        cameraToWorld = cameraToWorld * mat4f::rotatey(-alpha);
        cameraToWorld = cameraToWorld * mat4f::rotatex(-theta);
        cameraToWorld = cameraToWorld * mat4f::translate(vec3f(0.0, 0.0, dist));

        mat4f worldToCamera = cameraToWorld.inverse();

        vec4<GLint> vp = fb->getViewport();
        float width = (float) vp.z;
        float height = (float) vp.w;
        float vfov = degrees(2 * atan(height / width * tan(radians(fov / 2))));
        mat4f cameraToScreen = mat4f::perspectiveProjection(vfov, width / height, 0.1f, 1e5f);

        worldCamera->set(cameraToWorld * vec3f::ZERO);

        localToWorld1->setMatrix(mat4f::rotatez(15));
        localToScreen1->setMatrix(cameraToScreen * worldToCamera * mat4f::rotatez(15));
        fb->draw(p1, *cube, cube->mode, 0, cube->nvertices, 1, cube->nindices);

        localToWorld2->setMatrix(mat4f::translate(vec3f(0.0, 0.0, -2.0)) * mat4f::rotatez(180));
        localToScreen2->setMatrix(cameraToScreen * worldToCamera * mat4f::translate(vec3f(0.0, 0.0, -2.0)) * mat4f::rotatez(180));
        fb->draw(p2, *plane, plane->mode, 0, plane->nvertices, 1, plane->nindices);

        GlutWindow::redisplay(t, dt);

        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->flush();
        }
    }

    void reshape(int x, int y)
    {
        fb->setViewport(vec4<GLint>(0, 0, x, y));
        fb->setDepthTest(true, LESS);
        GlutWindow::reshape(x, y);
        idle(false);
    }

    void idle(bool damaged)
    {
        GlutWindow::idle(damaged);
        if (damaged) {
            resManager->updateResources();
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
        case KEY_F5:
            resManager->updateResources();
            break;
        default:
            break;
        }
        return true;
    }

    static static_ptr<Window> app;
};

static_ptr<Window> ResourceExample::app;

int resourceExample(int argc, char* argv[])
{
    assert(argc > 2);
    atexit(Object::exit);
    ResourceExample::app = new ResourceExample(string(argv[2]));
    ResourceExample::app->start();
    return 0;
}

static MainFunction resourceMain("resource", resourceExample);
