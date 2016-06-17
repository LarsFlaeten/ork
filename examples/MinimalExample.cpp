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
