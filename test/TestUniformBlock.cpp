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

#include "test/Test.h"

#include "ork/render/FrameBuffer.h"

using namespace std;
using namespace ork;

ptr<FrameBuffer> getFrameBuffer(RenderBuffer::RenderBufferFormat f, int w, int h);
ptr<GPUBuffer> getGPUBuffer(GLuint size)
{
    ptr<GPUBuffer> b = new GPUBuffer();
    b->setData(size, NULL, STREAM_DRAW);
    return b;
}

// ----------------------------------------------------------------------------
// VECTORS
// ----------------------------------------------------------------------------

TEST(testUniformBlock1f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { float u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0, 0.0); }\n"));
    p->getUniform1f("u")->set(1.0f);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f);
}

TEST(testUniformBlock2f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { vec2 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0); }\n"));
    p->getUniform2f("u")->set(vec2<GLfloat>(1.0f, 2.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

TEST(testUniformBlock3f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { vec3 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0); }\n"));
    p->getUniform3f("u")->set(vec3<GLfloat>(1.0f, 2.0f, 3.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f);
}

TEST(testUniformBlock4f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { vec4 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = u; }\n"));
    p->getUniform4f("u")->set(vec4<GLfloat>(1.0f, 2.0f, 3.0f, 4.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST4(testUniformBlock1d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform b { double u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0, 0.0); }\n"));
    p->getUniform1d("u")->set(1.0);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0);
}

TEST4(testUniformBlock2d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform b { dvec2 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0); }\n"));
    p->getUniform2d("u")->set(vec2<GLdouble>(1.0, 2.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

TEST4(testUniformBlock3d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform b { dvec3 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0); }\n"));
    p->getUniform3d("u")->set(vec3<GLdouble>(1.0, 2.0, 3.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f);
}

TEST4(testUniformBlock4d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform b { dvec4 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u); }\n"));
    p->getUniform4d("u")->set(vec4<GLdouble>(1.0, 2.0, 3.0, 4.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST(testUniformBlock1i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { int u; };\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0, 0, 0); }\n"));
    p->getUniform1i("u")->set(1);
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1);
}

TEST(testUniformBlock2i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { ivec2 u; };\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0, 0); }\n"));
    p->getUniform2i("u")->set(vec2<GLint>(1, 2));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2);
}

TEST(testUniformBlock3i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { ivec3 u; };\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0); }\n"));
    p->getUniform3i("u")->set(vec3<GLint>(1, 2, 3));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2 && pixels[2] == 3);
}

TEST(testUniformBlock4i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { ivec4 u; };\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = u; }\n"));
    p->getUniform4i("u")->set(vec4<GLint>(1, 2, 3, 4));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2 && pixels[2] == 3 && pixels[3] == 4);
}

TEST(testUniformBlock1b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { bool u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u), 0, 0, 0); }\n"));
    p->getUniform1b("u")->set(true);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f);
}

TEST(testUniformBlock2b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { bvec2 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0, 0); }\n"));
    p->getUniform2b("u")->set(vec2<bool>(false, true));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 0.0f && pixels[1] == 1.0f);
}

TEST(testUniformBlock3b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { bvec3 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0); }\n"));
    p->getUniform3b("u")->set(vec3<bool>(false, true, false));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 0.0f && pixels[1] == 1.0f && pixels[2] == 0.0f);
}

TEST(testUniformBlock4b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { bvec4 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u); }\n"));
    p->getUniform4b("u")->set(vec4<bool>(true, false, true, true));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 0.f && pixels[2] == 1.0f && pixels[3] == 1.0f);
}

// ----------------------------------------------------------------------------
// MATRICES
// ----------------------------------------------------------------------------

TEST(testUniformMatrixBlock3f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { mat3 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color.rgb = u * vec3(1.0); }\n"));
    p->getUniformMatrix3f("u")->setMatrix(mat3f(1.0f, 2.0f, 3.0f, 0.0f, 4.0f, 5.0f, 0.0f, 0.0f, 6.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 6.0f && pixels[1] == 9.0f && pixels[2] == 6.0f);
}

TEST(testUniformMatrixBlock4f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { mat4 u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = u * vec4(1.0); }\n"));
    p->getUniformMatrix4f("u")->setMatrix(mat4f(1.0f, 2.0f, 3.0f, 4.0f, 0.0f, 5.0f, 6.0f, 7.0f, 0.0f, 0.0f, 8.0f, 9.0f, 0.0f, 0.0f, 0.0f, 10.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 10.0f && pixels[1] == 18.0f && pixels[2] == 17.0f && pixels[3] == 10.0f);
}

// TODO test non square matrices

// ----------------------------------------------------------------------------
// STRUCTURES
// ----------------------------------------------------------------------------

TEST(testStructureBlock1)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        struct s { bool a; float b; int i; mat2 m; };\n\
        uniform b { s u; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u.a), u.b, float(u.i), dot(u.m * vec2(1.0), vec2(1.0))); }\n"));
    GLfloat m[4] = { 1.0f, 2.0f, 0.0f, 3.0f };
    p->getUniform1b("u.a")->set(true);
    p->getUniform1f("u.b")->set(2.0f);
    p->getUniformBlock("b")->getUniform1i("u.i")->set(3);
    p->getUniformBlock("b")->getUniformMatrix2f("u.m")->set(m);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 6.0f);
}

// ----------------------------------------------------------------------------
// ARRAYS
// ----------------------------------------------------------------------------

TEST(testArrayBlock1b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { bool u[4]; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u[0]), float(u[1]), float(u[2]), float(u[3])); }\n"));
    p->getUniform1b("u[0]")->set(true);
    p->getUniform1b("u[1]")->set(false);
    p->getUniformBlock("b")->getUniform1b("u[2]")->set(true);
    p->getUniformBlock("b")->getUniform1b("u[3]")->set(true);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 0.0f && pixels[2] == 1.0f && pixels[3] == 1.0f);
}

TEST(testArrayBlock1f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { float u[4]; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u[0], u[1], u[2], u[3]); }\n"));
    p->getUniform1f("u[0]")->set(1.0f);
    p->getUniform1f("u[1]")->set(2.0f);
    p->getUniformBlock("b")->getUniform1f("u[2]")->set(3.0f);
    p->getUniformBlock("b")->getUniform1f("u[3]")->set(4.0f);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST(testArrayBlock1i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform b { int u[4]; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u[0], u[1], u[2], u[3]); }\n"));
    p->getUniform1i("u[0]")->set(1);
    p->getUniform1i("u[1]")->set(2);
    p->getUniformBlock("b")->getUniform1i("u[2]")->set(3);
    p->getUniformBlock("b")->getUniform1i("u[3]")->set(4);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

// ----------------------------------------------------------------------------
// ARRAYS OF STRUCTURES
// ----------------------------------------------------------------------------

TEST(testStructureArrayBlock1)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        struct s { bool a; float b; int i; mat2 m; };\n\
        uniform b { s u[4]; };\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u[0].a), u[1].b, float(u[2].i), dot(u[3].m * vec2(1.0), vec2(1.0))); }\n"));
    GLfloat m[4] = { 1.0f, 2.0f, 0.0f, 3.0f };
    p->getUniform1b("u[0].a")->set(true);
    p->getUniform1f("u[1].b")->set(2.0f);
    p->getUniformBlock("b")->getUniform1i("u[2].i")->set(3);
    p->getUniformBlock("b")->getUniformMatrix2f("u[3].m")->set(m);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 6.0f);
}

TEST(automaticUniformBlockBufferBinding)
{
    vector< ptr<GPUBuffer> > buffers;
    for (int i = 0; i < 128; ++i) {
        ptr<GPUBuffer> b = new GPUBuffer();
        b->setData(16, NULL, DYNAMIC_DRAW);
        buffers.push_back(b);
    }
    vector< ptr<Program> > programs;
    for (int i = 0; i < 48; ++i) {
        ptr<Program> p = new Program(new Module(330, NULL, "\
            uniform b1 { int i1; };\n\
            uniform b2 { int i2; };\n\
            uniform b3 { int i3; };\n\
            uniform b4 { int i4; };\n\
            layout(location=0) out ivec4 color;\n\
            void main() { color = ivec4(i1 + i2 + i3 + i4); }\n"));
        p->getUniformBlock("b1")->setBuffer(buffers[(3 * i) % 128]);
        p->getUniformBlock("b2")->setBuffer(buffers[(3 * i + 1) % 128]);
        p->getUniformBlock("b3")->setBuffer(buffers[(3 * i + 2) % 128]);
        p->getUniformBlock("b4")->setBuffer(buffers[(3 * i + 3) % 128]);
        p->getUniform1i("i1")->set((3 * i) % 128);
        p->getUniform1i("i2")->set((3 * i + 1) % 128);
        p->getUniformBlock("b3")->getUniform1i("i3")->set((3 * i + 2) % 128);
        p->getUniformBlock("b4")->getUniform1i("i4")->set((3 * i + 3) % 128);
        programs.push_back(p);
    }
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setRenderBuffer(COLOR0, new RenderBuffer(RenderBuffer::R32I, 1, 1));
    fb->setViewport(vec4<GLint>(0, 0, 1, 1));
    bool ok = true;
    for (int i = 0; i < 96; ++i) {
        int p = i % 48;
        int v = (3 * p) % 128 + (3 * p + 1) % 128 + (3 * p + 2) % 128 + (3 * p + 3) % 128;
        GLint pixel;
        fb->clear(true, true, true);
        fb->drawQuad(programs[p]);
        fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel));
        ok = ok && (pixel == v);
    }
    ASSERT(ok);
}
