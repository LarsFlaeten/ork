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
