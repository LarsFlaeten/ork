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

#include "ork/resource/XMLResourceLoader.h"
#include "ork/resource/ResourceManager.h"
#include "ork/render/FrameBuffer.h"

using namespace std;
using namespace ork;

ptr<FrameBuffer> getFrameBuffer(RenderBuffer::RenderBufferFormat f, int w, int h);

void createFile(const char *name, const char* content)
{
    FILE *f = fopen(name, "w");
    assert(f);
    fwrite(content, strlen(content), 1, f);
    fclose(f);
}

void createFile(const char *name, int size, const unsigned char* content)
{
    FILE *f = fopen(name, "w");
    assert(f);
    fwrite(content, size, 1, f);
    fclose(f);
}

class TestResourceLoader : public XMLResourceLoader
{
public:
    virtual ptr<ResourceDescriptor> reloadResource(const string &name, ptr<ResourceDescriptor> currentValue)
    {
        return loadResource(name);
    }
};

TEST(testModuleResource)
{
    createFile("test.glsl", "#ifdef _VERTEX_\nlayout(location=0) in vec4 p; out vec4 q; void main() { q = p; }\n#endif\n");
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"330\" source=\"test.glsl\" feedback=\"interleaved\" varyings=\"q\"/>\n");
    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr< Mesh<vec4f, unsigned int> > pt = new Mesh<vec4f, unsigned int>(POINTS, GPU_STATIC);
    pt->addAttributeType(0, 4, A32F, false);
    pt->addVertex(vec4f(1.0, 2.0, 3.0, 4.0));
    ptr<TransformFeedback> tfb = TransformFeedback::getDefault();
    ptr<Query> q = new Query(PRIMITIVES_GENERATED);
    ptr<GPUBuffer> b = new GPUBuffer();
    b->setData(128, NULL, STREAM_COPY);
    tfb->setVertexBuffer(0, b);
    q->begin();
    TransformFeedback::begin(fb, p, POINTS, tfb, false);
    TransformFeedback::transform(*(pt->getBuffers()), 0, 1);
    TransformFeedback::end();
    q->end();
    int n = q->getResult();
    float data[4];
    b->getSubData(0, 16, data);
    ASSERT(data[0] == 1.0f && data[1] == 2.0f && data[2] == 3.0f && data[3] == 4.0f && n == 1);
    remove("test.glsl");
    remove("test.xml");
}

TEST(textureResourceUpdate)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<texture2D name=\"test\" source=\"test.tga\" internalformat=\"RGB8UI\" format=\"RGB_INTEGER\" min=\"NEAREST\" mag=\"NEAREST\"/>\n");
    unsigned char img1[] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 24, 0, 2, 1, 0 };
    createFile("test.tga", 25, img1);

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Texture2D> t = resManager->loadResource("test").cast<Texture2D>();

    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler2D u;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = texture(u, vec2(0.0)); }\n"));
    p->getUniformSampler("u")->set(t);

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB8UI, 1, 1);
    int pixel1[3] = { 0, 0, 0 };
    int pixel2[3] = { 0, 0, 0 };

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixel1));

    unsigned char img2[] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 24, 0, 5, 4, 3 };
    createFile("test.tga", 25, img2);
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixel2));

    ASSERT(pixel1[0] == 0 && pixel1[1] == 1 && pixel1[2] == 2 &&
        pixel2[0] == 3 && pixel2[1] == 4 && pixel2[2] == 5);

    remove("test.xml");
    remove("test.tga");
}

TEST(moduleResourceUpdate)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"330\" source=\"test.glsl\">\n<uniform1i name=\"u\" x=\"1\"/>\n</module>\n");
    createFile("test.glsl", "#ifdef _FRAGMENT_\nlayout(location=0) out ivec4 color;\nuniform int u;\nvoid main() { color = ivec4(u); }\n#endif\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<Uniform1i> u = p->getUniform1i("u");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    int pixel1 = 0;
    int pixel2 = 0;
    int pixel3 = 0;
    int pixel4 = 0;

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    u->set(2);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    createFile("test.glsl", "#ifdef _FRAGMENT_\nlayout(location=0) out ivec4 color;\nuniform int u;\nvoid main() { color = ivec4(u + 1); }\n#endif\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel3));

    u->set(3);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel4));

    ASSERT(pixel1 == 1 && pixel2 == 2 && pixel3 == 3 && pixel4 == 4);

    remove("test.xml");
    remove("test.glsl");
}

TEST(moduleResourceUpdateWithUniformSamplers)
{
    unsigned char img1[] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 24, 0, 2, 1, 0 };
    unsigned char img2[] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 24, 0, 5, 4, 3 };
    createFile("tex1.xml", "<?xml version=\"1.0\" ?>\n<texture2D name=\"tex1\" source=\"tex1.tga\" internalformat=\"RGB8UI\" format=\"RGB_INTEGER\" min=\"NEAREST\" mag=\"NEAREST\"/>\n");
    createFile("tex1.tga", 25, img1);
    createFile("tex2.xml", "<?xml version=\"1.0\" ?>\n<texture2D name=\"tex2\" source=\"tex2.tga\" internalformat=\"RGB8UI\" format=\"RGB_INTEGER\" min=\"NEAREST\" mag=\"NEAREST\"/>\n");
    createFile("tex2.tga", 25, img2);
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"330\" fragment=\"test.glsl\">\n<uniformSampler name=\"u1\" texture=\"tex1\"/>\n<uniformSampler name=\"u2\" texture=\"tex2\"/>\n</module>\n");
    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform isampler2D u1;\nuniform isampler2D u2;\nvoid main() { color = texture(u1, vec2(0.5)) + texture(u2, vec2(0.5)); }\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<Uniform1i> u = p->getUniform1i("u");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB8UI, 1, 1);
    int pixel1[3] = { 0, 0, 0 };
    int pixel2[3] = { 0, 0, 0 };

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform isampler2D u1;\nuniform isampler2D u2;\nvoid main() { color = texture(u1, vec2(0.5)) + texture(u2, vec2(0.5)) + ivec4(1); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    ASSERT(pixel1[0] == 3 && pixel1[1] == 5 && pixel1[2] == 7 &&
        pixel2[0] == 4 && pixel2[1] == 6 && pixel2[2] == 8);

    remove("tex1.xml");
    remove("tex1.tga");
    remove("tex2.xml");
    remove("tex2.tga");
    remove("test.xml");
    remove("test.glsl");
}

TEST4(moduleResourceUpdateWithUniformSubroutines)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"400\" fragment=\"test.glsl\">\n<uniformSubroutine stage=\"FRAGMENT\" name=\"u\" subroutine=\"sr1\"/>\n</module>\n");
    createFile("test.glsl", "layout(location=0) out ivec4 color;\nsubroutine int sr(int x);\nsubroutine (sr) int sr1(int x) { return x; }\nsubroutine (sr) int sr2(int x) { return x + 1; }\nsubroutine uniform sr u;\nvoid main() { color = ivec4(u(0)); }\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<UniformSubroutine> u = p->getUniformSubroutine(FRAGMENT, "u");
    u->setSubroutine("sr2");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGB8UI, 1, 1);
    int pixel1[3] = { 0, 0, 0 };
    int pixel2[3] = { 0, 0, 0 };

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nsubroutine int sr(int x);\nsubroutine (sr) int sr1(int x) { return x + 2; }\nsubroutine (sr) int sr2(int x) { return x + 3; }\nsubroutine uniform sr u;\nvoid main() { color = ivec4(u(0)); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGB_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    ASSERT(pixel1[0] == 1 && pixel2[0] == 3);

    remove("test.xml");
    remove("test.glsl");
}

TEST(moduleResourceUpdateRemovedUniform)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"330\" fragment=\"test.glsl\">\n<uniform1i name=\"u\" x=\"3\"/>\n</module>\n");
    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform int u;\nvoid main() { color = ivec4(u); }\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<Uniform1i> u = p->getUniform1i("u");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    int pixel1 = 0;
    int pixel2 = 0;
    int pixel3 = 0;
    int pixel4 = 0;

    u->set(1);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\n\nvoid main() { color = ivec4(2); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform int u;\nvoid main() { color = ivec4(u + 2); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel3));

    u->set(2);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel4));

    ASSERT(pixel1 == 1 && pixel2 == 2 && pixel3 == 3 && pixel4 == 4);

    remove("test.xml");
    remove("test.glsl");
}

TEST4(moduleResourceUpdateRemovedUniformSubroutine)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"400\" fragment=\"test.glsl\">\n<uniformSubroutine stage=\"FRAGMENT\" name=\"u\" subroutine=\"sr1\"/>\n</module>\n");
    createFile("test.glsl", "layout(location=0) out ivec4 color;\nsubroutine int sr(int x);\nsubroutine (sr) int sr1(int x) { return x; }\nsubroutine (sr) int sr2(int x) { return x + 1; }\nsubroutine uniform sr u;\nvoid main() { color = ivec4(u(0)); }\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<UniformSubroutine> u = p->getUniformSubroutine(FRAGMENT, "u");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    int pixel1 = 0;
    int pixel2 = 0;
    int pixel3 = 0;
    int pixel4 = 0;

    u->setSubroutine("sr1");

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nvoid main() { color = ivec4(1); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nsubroutine int sr(int x);\nsubroutine (sr) int sr1(int x) { return x; }\nsubroutine (sr) int sr2(int x) { return x + 1; }\nsubroutine uniform sr u;\nvoid main() { color = ivec4(u(0)); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel3));

    u->setSubroutine("sr2");

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel4));

    ASSERT(pixel1 == 0 && pixel2 == 1 && pixel3 == 0 && pixel4 == 1);

    remove("test.xml");
    remove("test.glsl");
}

TEST(moduleResourceUpdateUniformBlock)
{
    createFile("test.xml", "<?xml version=\"1.0\" ?>\n<module name=\"test\" version=\"330\" fragment=\"test.glsl\">\n<uniform1i name=\"u\" x=\"1\"/>\n</module>\n");
    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform b { int u; };\nvoid main() { color = ivec4(u); }\n");

    ptr<XMLResourceLoader> resLoader = new TestResourceLoader();
    resLoader->addPath(".");
    ptr<ResourceManager> resManager = new ResourceManager(resLoader);
    ptr<Program> p = resManager->loadResource("test;").cast<Program>();
    ptr<Uniform1i> u = p->getUniform1i("u");

    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
    int pixel1 = 0;
    int pixel2 = 0;
    int pixel3 = 0;
    int pixel4 = 0;

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel1));

    u->set(2);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel2));

    createFile("test.glsl", "layout(location=0) out ivec4 color;\nuniform b { int u; };\nvoid main() { color = ivec4(u + 1); }\n");
    resManager->updateResources();

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel3));

    u->set(3);

    fb->clear(true, true, true);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(&pixel4));

    ASSERT(pixel1 == 1 && pixel2 == 2 && pixel3 == 3 && pixel4 == 4);

    remove("test.xml");
    remove("test.glsl");
}
