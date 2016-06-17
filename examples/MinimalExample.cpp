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

#include "ork/render/FrameBuffer.h"
#include "ork/ui/GlutWindow.h"

#include "examples/Main.h"

using namespace ork;

class MinimalExample : public GlutWindow
{
public:
    ptr< Mesh<vec2f, unsigned int> > m;

    ptr<Program> p;

    MinimalExample() : GlutWindow(Window::Parameters().size(512, 512))
    {
        // creates a mesh whose vertices, made of vec2f, form triangle strips,
        // and which is stored on GPU and not frequently modified
        m = new Mesh<vec2f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
        // adds a vertex attribute of id #0, made of two float coordinates
        m->addAttributeType(0, 2, A32F, false);
        // adds four vertices to the mesh
        m->addVertex(vec2f(-1, -1));
        m->addVertex(vec2f(+1, -1));
        m->addVertex(vec2f(-1, +1));
        m->addVertex(vec2f(+1, +1));

        // creates a 2D texture with 4x4 pixels, using one 8bits channel
        // per pixel, with a magnification filter in nearest mode
        unsigned char data[16] = { 0, 255, 0, 255, 255, 0, 255, 0, 0, 255, 0, 255, 255, 0, 255, 0 };
        ptr<Texture2D> tex = new Texture2D(4, 4, R8, RED, UNSIGNED_BYTE,
            Texture::Parameters().mag(NEAREST), Buffer::Parameters(), CPUBuffer(data));

        // creates a program made of a single module,
        // itself made of a single fragment shader
        p = new Program(new Module(330, NULL, "\
            uniform sampler2D sampler;\n\
            uniform vec2 scale;\n\
            layout(location = 0) out vec4 data;\n\
            void main() {\n\
                data = texture(sampler, gl_FragCoord.xy * scale).rrrr;\n\
            }\n"));

        // sets the "sampler" uniform of 'p' to 'tex'
        p->getUniformSampler("sampler")->set(tex);
    }

    virtual void redisplay(double t, double dt)
    {
        ptr<FrameBuffer> fb = FrameBuffer::getDefault();
        // clears the color buffer of the default framebuffer
        fb->clear(true, false, false);
        // draws the mesh 'm' in the default framebuffer, with the program 'p'
        fb->draw(p, *m);
        // calls the overriden redisplay method to display the result
        GlutWindow::redisplay(t, dt);
    }

    virtual void reshape(int x, int y)
    {
        // sets the viewport of the default framebuffer to the new window size
        FrameBuffer::getDefault()->setViewport(vec4<GLint>(0, 0, x, y));
        // sets the 'scale' uniform in program 'p' to the inverse of the new window size
        p->getUniform2f("scale")->set(vec2f(1.0f / x, 1.0f / y));
        // calls the overriden method, which updates the Window state
        GlutWindow::reshape(x, y);
        // calls the idle method to request the display of a new frame
        idle(false);
    }

    bool keyTyped(unsigned char c, modifier m, int x, int y)
    {
        if (c == 27) {
            ::exit(0);
        }
        return true;
    }

    static static_ptr<Window> app;
};

static_ptr<Window> MinimalExample::app;

int minimalExample(int argc, char* argv[])
{
    atexit(Object::exit);
    MinimalExample::app = new MinimalExample();
    MinimalExample::app->start();
    return 0;
}

static MainFunction minimalMain("minimal", minimalExample);
