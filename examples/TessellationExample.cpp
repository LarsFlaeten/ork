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

#include "ork/core/Logger.h"
#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

#include "examples/Main.h"

using namespace ork;

class TessellationExample : public GlutWindow
{
public:
    ptr< Mesh<vec3f, unsigned int> > model;
    ptr<UniformMatrix4f> localToScreen;
    ptr<Program> p;
    ptr<Uniform1f> inner;
    ptr<Uniform1f> outer;
    ptr<FrameBuffer> fb;
    float fov;
    float alpha;
    float theta;
    float dist;

    TessellationExample() :
        GlutWindow(Window::Parameters().size(1024, 768).version(4,0).depth(true)), fov(80.0), alpha(135), theta(45), dist(5)
    {
        model = new Mesh<vec3f, unsigned int>(PATCHES, GPU_STATIC);
        model->setPatchVertices(3);
        model->addAttributeType(0, 3, A32F, false);
        model->addVertex(vec3f(0.000f,  0.000f,  1.000f));
        model->addVertex(vec3f(0.894f,  0.000f,  0.447f));
        model->addVertex(vec3f(0.276f,  0.851f,  0.447f));
        model->addVertex(vec3f(-0.724f,  0.526f,  0.447f));
        model->addVertex(vec3f(-0.724f, -0.526f,  0.447f));
        model->addVertex(vec3f(0.276f, -0.851f,  0.447f));
        model->addVertex(vec3f(0.724f,  0.526f, -0.447f));
        model->addVertex(vec3f(-0.276f,  0.851f, -0.447f));
        model->addVertex(vec3f(-0.894f,  0.000f, -0.447f));
        model->addVertex(vec3f(-0.276f, -0.851f, -0.447f));
        model->addVertex(vec3f(0.724f, -0.526f, -0.447f));
        model->addVertex(vec3f(0.000f,  0.000f, -1.000f));
        model->addIndice(2);
        model->addIndice(1);
        model->addIndice(0);
        model->addIndice(3);
        model->addIndice(2);
        model->addIndice(0);
        model->addIndice(4);
        model->addIndice(3);
        model->addIndice(0);
        model->addIndice(5);
        model->addIndice(4);
        model->addIndice(0);
        model->addIndice(1);
        model->addIndice(5);
        model->addIndice(0);
        model->addIndice(11);
        model->addIndice(6);
        model->addIndice(7);
        model->addIndice(11);
        model->addIndice(7);
        model->addIndice(8);
        model->addIndice(11);
        model->addIndice(8);
        model->addIndice(9);
        model->addIndice(11);
        model->addIndice(9);
        model->addIndice(10);
        model->addIndice(11);
        model->addIndice(10);
        model->addIndice(6);
        model->addIndice(1);
        model->addIndice(2);
        model->addIndice(6);
        model->addIndice(2);
        model->addIndice(3);
        model->addIndice(7);
        model->addIndice(3);
        model->addIndice(4);
        model->addIndice(8);
        model->addIndice(4);
        model->addIndice(5);
        model->addIndice(9);
        model->addIndice(5);
        model->addIndice(1);
        model->addIndice(10);
        model->addIndice(2);
        model->addIndice(7);
        model->addIndice(6);
        model->addIndice(3);
        model->addIndice(8);
        model->addIndice(7);
        model->addIndice(4);
        model->addIndice(9);
        model->addIndice(8);
        model->addIndice(5);
        model->addIndice(10);
        model->addIndice(9);
        model->addIndice(1);
        model->addIndice(6);
        model->addIndice(10);

        p = new Program(new Module(400, "\
            #ifdef _VERTEX_\n\
            layout(location = 0) in vec3 pos;\n\
            out vec3 vPos;\n\
            void main() {\n\
                vPos = pos;\n\
            }\n\
            #endif\n\
            #ifdef _TESS_CONTROL_\n\
            layout(vertices = 3) out;\n\
	        in vec3 vPos[];\n\
            out vec3 tcPos[];\n\
            uniform float inner;\n\
            uniform float outer;\n\
            void main() {\n\
                tcPos[gl_InvocationID] = vPos[gl_InvocationID];\n\
                if (gl_InvocationID == 0) {\n\
                    gl_TessLevelInner[0] = inner;\n\
                    gl_TessLevelOuter[0] = outer;\n\
                    gl_TessLevelOuter[1] = outer;\n\
                    gl_TessLevelOuter[2] = outer;\n\
                }\n\
            }\n\
            #endif\n\
            #ifdef _TESS_EVAL_\n\
            layout(triangles, equal_spacing, cw) in;\n\
	        in vec3 tcPos[];\n\
            uniform mat4 localToScreen;\n\
            void main() {\n\
                vec3 p0 = gl_TessCoord.x * tcPos[0];\n\
                vec3 p1 = gl_TessCoord.y * tcPos[1];\n\
                vec3 p2 = gl_TessCoord.z * tcPos[2];\n\
                vec3 p = normalize(p0 + p1 + p2);\n\
                gl_Position = localToScreen * vec4(p, 1.0);\n\
            }\n\
            #endif\n\
            #ifdef _GEOMETRY_\n\
            layout(triangles) in;\n\
            layout(triangle_strip, max_vertices = 3) out;\n\
            out vec3 triDist;\n\
            void main() {\n\
                gl_Position = gl_in[0].gl_Position; triDist = vec3(1.0, 0.0, 0.0); EmitVertex();\n\
                gl_Position = gl_in[1].gl_Position; triDist = vec3(0.0, 1.0, 0.0); EmitVertex();\n\
                gl_Position = gl_in[2].gl_Position; triDist = vec3(0.0, 0.0, 1.0); EmitVertex();\n\
                EndPrimitive();\n\
            }\n\
            #endif\n\
            #ifdef _FRAGMENT_\n\
            in vec3 triDist;\n\
            layout(location = 0) out vec4 data;\n\
            void main() {\n\
                float d = min(min(triDist.x, triDist.y), triDist.z);\n\
                float w = fwidth(d) * 0.5;\n\
                float t = smoothstep(0.02 - w, 0.02 + w, d);\n\
                data = mix(vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0), t);\n\
            }\n\
            #endif\n"));

        localToScreen = p->getUniformMatrix4f("localToScreen");
        inner = p->getUniform1f("inner");
        outer = p->getUniform1f("outer");
        inner->set(5.0f);
        outer->set(5.0f);

        fb = FrameBuffer::getDefault();
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

        localToScreen->setMatrix(cameraToScreen * worldToCamera * mat4f::rotatez(15));
        fb->draw(p, *model);

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

    bool keyTyped(unsigned char c, modifier m, int x, int y)
    {
        if (c == 27) {
            ::exit(0);
        } else if (c == 'i') {
            inner->set(inner->get() - 1.0f);
        } else if (c == 'I') {
            inner->set(inner->get() + 1.0f);
        } else if (c == 'o') {
            outer->set(outer->get() - 1.0f);
        } else if (c == 'O') {
            outer->set(outer->get() + 1.0f);
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

    static static_ptr<Window> app;
};

static_ptr<Window> TessellationExample::app;

int tessellationExample(int argc, char* argv[])
{
    atexit(Object::exit);
    TessellationExample::app = new TessellationExample();
    TessellationExample::app->start();
    return 0;
}

MainFunction tessellationMain("tessellation", tessellationExample);
