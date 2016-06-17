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

using namespace ork;

ptr<FrameBuffer> getFrameBuffer(RenderBuffer::RenderBufferFormat f, int w, int h)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setRenderBuffer(COLOR0, new RenderBuffer(f, w, h));
    fb->setViewport(vec4<GLint>(0, 0, w, h));
    return fb;
}

// ----------------------------------------------------------------------------
// VECTORS
// ----------------------------------------------------------------------------

TEST(testUniform1f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform float u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0, 0.0); }\n"));
    p->getUniform1f("u")->set(1.0f);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f);
}

TEST(testUniform2f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform vec2 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0); }\n"));
    p->getUniform2f("u")->set(vec2<GLfloat>(1.0f, 2.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

TEST(testUniform3f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform vec3 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0); }\n"));
    p->getUniform3f("u")->set(vec3<GLfloat>(1.0f, 2.0f, 3.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f);
}

TEST(testUniform4f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform vec4 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = u; }\n"));
    p->getUniform4f("u")->set(vec4<GLfloat>(1.0f, 2.0f, 3.0f, 4.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST4(testUniform1d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform double u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u), 0.0, 0.0, 0.0); }\n"));
    p->getUniform1d("u")->set(1.0);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0);
}

TEST4(testUniform2d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform dvec2 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0); }\n"));
    p->getUniform2d("u")->set(vec2<GLdouble>(1.0, 2.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

TEST4(testUniform3d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform dvec3 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0); }\n"));
    p->getUniform3d("u")->set(vec3<GLdouble>(1.0, 2.0, 3.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f);
}

TEST4(testUniform4d)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        uniform dvec4 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u); }\n"));
    p->getUniform4d("u")->set(vec4<GLdouble>(1.0, 2.0, 3.0, 4.0));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST(testUniform1i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform int u;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0, 0, 0); }\n"));
    p->getUniform1i("u")->set(1);
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1);
}

TEST(testUniform2i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform ivec2 u;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0, 0); }\n"));
    p->getUniform2i("u")->set(vec2<GLint>(1, 2));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2);
}

TEST(testUniform3i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform ivec3 u;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = ivec4(u, 0); }\n"));
    p->getUniform3i("u")->set(vec3<GLint>(1, 2, 3));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2 && pixels[2] == 3);
}

TEST(testUniform4i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32I, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform ivec4 u;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = u; }\n"));
    p->getUniform4i("u")->set(vec4<GLint>(1, 2, 3, 4));
    GLint pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1 && pixels[1] == 2 && pixels[2] == 3 && pixels[3] == 4);
}

TEST(testUniform1b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform bool u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u), 0, 0, 0); }\n"));
    p->getUniform1b("u")->set(true);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f);
}

TEST(testUniform2b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform bvec2 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0, 0); }\n"));
    p->getUniform2b("u")->set(vec2<bool>(false, true));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 0.0f && pixels[1] == 1.0f);
}

TEST(testUniform3b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform bvec3 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0); }\n"));
    p->getUniform3b("u")->set(vec3<bool>(false, true, false));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 0.0f && pixels[1] == 1.0f && pixels[2] == 0.0f);
}

TEST(testUniform4b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform bvec4 u;\n\
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

TEST(testUniformMatrix3f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform mat3 u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color.rgb = u * vec3(1.0); }\n"));
    p->getUniformMatrix3f("u")->setMatrix(mat3f(1.0f, 2.0f, 3.0f, 0.0f, 4.0f, 5.0f, 0.0f, 0.0f, 6.0f));
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 6.0f && pixels[1] == 9.0f && pixels[2] == 6.0f);
}

TEST(testUniformMatrix4f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform mat4 u;\n\
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

TEST(testStructure1)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform struct { bool a; float b; int i; mat2 m; } u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u.a), u.b, float(u.i), dot(u.m * vec2(1.0), vec2(1.0))); }\n"));
    GLfloat m[4] = { 1.0f, 2.0f, 0.0f, 3.0f };
    p->getUniform1b("u.a")->set(true);
    p->getUniform1f("u.b")->set(2.0f);
    p->getUniform1i("u.i")->set(3);
    p->getUniformMatrix2f("u.m")->set(m);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 6.0f);
}

// ----------------------------------------------------------------------------
// ARRAYS
// ----------------------------------------------------------------------------

TEST(testArray1b)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform bool u[4];\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u[0]), float(u[1]), float(u[2]), float(u[3])); }\n"));
    p->getUniform1b("u[0]")->set(true);
    p->getUniform1b("u[1]")->set(false);
    p->getUniform1b("u[2]")->set(true);
    p->getUniform1b("u[3]")->set(true);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 0.0f && pixels[2] == 1.0f && pixels[3] == 1.0f);
}

TEST(testArray1f)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform float u[4];\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u[0], u[1], u[2], u[3]); }\n"));
    p->getUniform1f("u[0]")->set(1.0f);
    p->getUniform1f("u[1]")->set(2.0f);
    p->getUniform1f("u[2]")->set(3.0f);
    p->getUniform1f("u[3]")->set(4.0f);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

TEST(testArray1i)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform int u[4];\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u[0], u[1], u[2], u[3]); }\n"));
    p->getUniform1i("u[0]")->set(1);
    p->getUniform1i("u[1]")->set(2);
    p->getUniform1i("u[2]")->set(3);
    p->getUniform1i("u[3]")->set(4);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 4.0f);
}

// ----------------------------------------------------------------------------
// ARRAYS OF STRUCTURES
// ----------------------------------------------------------------------------

TEST(testStructureArray1)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        struct s { bool a; float b; int i; mat2 m; };\n\
        uniform s u[4];\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(float(u[0].a), u[1].b, float(u[2].i), dot(u[3].m * vec2(1.0), vec2(1.0))); }\n"));
    GLfloat m[4] = { 1.0f, 2.0f, 0.0f, 3.0f };
    p->getUniform1b("u[0].a")->set(true);
    p->getUniform1f("u[1].b")->set(2.0f);
    p->getUniform1i("u[2].i")->set(3);
    p->getUniformMatrix2f("u[3].m")->set(m);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f && pixels[2] == 3.0f && pixels[3] == 6.0f);
}

// ----------------------------------------------------------------------------
// SUBROUTINES
// ----------------------------------------------------------------------------

TEST4(testSubroutine1)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        subroutine float sr(float x);\n\
        subroutine (sr) float sr1(float x) { return x; }\n\
        subroutine (sr) float sr2(float x) { return x + 1.0f; }\n\
        subroutine uniform sr f;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(f(1.0), 0.0, 0.0, 0.0); }\n"));
    p->getUniformSubroutine(FRAGMENT, "f")->setSubroutine("sr1");
    GLfloat pixels1[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels1));
    p->getUniformSubroutine(FRAGMENT, "f")->setSubroutine("sr2");
    GLfloat pixels2[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels2));
    ASSERT(pixels1[0] == 1.0f && pixels2[0] == 2.0f);
}

TEST4(testSubroutine2)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> p = new Program(new Module(400, NULL, "\
        subroutine float sr(float x);\n\
        subroutine (sr) float sr1(float x) { return x; }\n\
        subroutine (sr) float sr2(float x) { return x + 1.0f; }\n\
        subroutine uniform sr f[2];\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(f[0](1.0), f[1](1.0), 0.0, 0.0); }\n"));
    p->getUniformSubroutine(FRAGMENT, "f[0]")->setSubroutine("sr1");
    p->getUniformSubroutine(FRAGMENT, "f[1]")->setSubroutine("sr2");
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

