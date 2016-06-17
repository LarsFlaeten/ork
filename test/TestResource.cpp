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

#include "ork/resource/XMLResourceLoader.h"
#include "ork/resource/ResourceManager.h"
#include "ork/render/FrameBuffer.h"

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
