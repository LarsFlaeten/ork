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

const string LAYER_INSTANCING = "\
    #ifdef _VERTEX_\n\
    layout(location=0) in vec4 pos;\n\
    void main() { gl_Position = pos; }\n\
    #endif\n\
    #ifdef _GEOMETRY_\n\
    layout(triangles) in;\n\
    layout(triangle_strip, max_vertices = 24) out;\n\
    in vec4 pos[];\n\
    void main() {\n\
        for (int i = 0; i < 8; ++i) {\n\
            gl_Layer = i;\n\
            gl_Position = gl_in[0].gl_Position;\n\
            EmitVertex();\n\
            gl_Position = gl_in[1].gl_Position;\n\
            EmitVertex();\n\
            gl_Position = gl_in[2].gl_Position;\n\
            EmitVertex();\n\
            EndPrimitive();\n\
        }\n\
    }\n\
    #endif\n";

const char* FRAGMENT_SHADER = "\
    #ifdef _FRAGMENT_\n\
    layout(location=0) out ivec4 color;\n\
    void main() { color = ivec4(1, 2, 3, 4); }\n\
    #endif\n";

const char* FRAGMENT_SHADER_FLOAT = "\
    #ifdef _FRAGMENT_\n\
    layout(location=0) out vec4 color;\n\
    void main() { color = vec4(1.0, 2.0, 3.0, 4.0); }\n\
    #endif\n";

const char* DRAW_INSTANCING = "\
    #ifdef _VERTEX_\n\
    layout(location=0) in vec4 pos;\n\
    flat out int instance;\n\
    void main() { gl_Position = pos; instance = gl_InstanceID; }\n\
    #endif\n\
    #ifdef _GEOMETRY_\n\
    layout(triangles) in;\n\
    layout(triangle_strip, max_vertices = 3) out;\n\
    in vec4 pos[];\n\
    flat in int instance[];\n\
    void main() {\n\
        gl_Layer = instance[0];\n\
        gl_Position = gl_in[0].gl_Position;\n\
        EmitVertex();\n\
        gl_Position = gl_in[1].gl_Position;\n\
        EmitVertex();\n\
        gl_Position = gl_in[2].gl_Position;\n\
        EmitVertex();\n\
        EndPrimitive();\n\
    }\n\
    #endif\n\
    #ifdef _FRAGMENT_\n\
    layout(location=0) out ivec4 color;\n\
    void main() { color = ivec4(1, 2, 3, 4); }\n\
    #endif\n";

TEST(texture1DAttachment)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture1D(32, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST), Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 32, 1));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 32];
    int fbPixels[4 * 32];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 32, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4);
}

TEST(texture1DArrayAttachmentOneLayer)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture1DArray(32, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST), Buffer::Parameters(), CPUBuffer(NULL)), 0, 3);
    fb->setViewport(vec4<GLint>(0, 0, 32, 1));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 32 * 8];
    int fbPixels[4 * 32];
    int l = 4 * 32 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 32, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(texture1DArrayAttachmentAllLayers)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture1DArray(32, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 32, 1));
    ptr<Program> p = new Program(new Module(330, (LAYER_INSTANCING + FRAGMENT_SHADER).c_str()));
    fb->drawQuad(p);
    int tPixels[4 * 32 * 8];
    int fbPixels[4 * 32];
    int l = 4 * 32 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 32, 1, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(texture2DAttachment)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(32, 32, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 32, 32));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 32 * 32];
    int fbPixels[4 * 32 * 32];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 32, 32, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4);
}

TEST(textureRectangleAttachment)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new TextureRectangle(32, 32, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 32, 32));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 32 * 32];
    int fbPixels[4 * 32 * 32];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 32, 32, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4);
}

TEST(texture2DMultisampleAttachment)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2DMultisample(32, 32, 4, RGBA32F, true));
    fb->setViewport(vec4<GLint>(0, 0, 32, 32));
    fb->setMultisample(true);
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER_FLOAT));
    fb->drawQuad(p);
    fb->setMultisample(false);
    p = new Program(new Module(330, NULL, "\
        uniform sampler2DMS sampler;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = texelFetch(sampler, ivec2(floor(gl_FragCoord.xy)), 0); }\n"));
    p->getUniformSampler("sampler")->set(fb->getTextureBuffer(COLOR0));
    fb->setTextureBuffer(COLOR0, new Texture2D(32, 32, RGBA32F, RGBA, FLOAT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->drawQuad(p);
    float tPixels[4 * 32 * 32];
    float fbPixels[4 * 32 * 32];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA, FLOAT, tPixels);
    fb->readPixels(0, 0, 32, 32, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1.0f && fbPixels[1] == 2.0f && fbPixels[2] == 3.0f && fbPixels[3] == 4.0f &&
        tPixels[0] == 1.0f && tPixels[1] == 2.0f && tPixels[2] == 3.0f && tPixels[3] == 4.0f);
}

TEST(texture2DArrayAttachmentOneLayer)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2DArray(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, 3);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(texture2DArrayAttachmentAllLayers)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2DArray(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, (LAYER_INSTANCING + FRAGMENT_SHADER).c_str()));
    fb->drawQuad(p);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(texture2DMultisampleArrayAttachmentOneLayer)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2DMultisampleArray(8, 8, 8, 4, RGBA32F, true), 3);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    fb->setMultisample(true);
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER_FLOAT));
    fb->drawQuad(p);
    fb->setMultisample(false);
    p = new Program(new Module(330, NULL, "\
        uniform sampler2DMSArray sampler;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = texelFetch(sampler, ivec3(floor(gl_FragCoord.xy), 3), 0); }\n"));
    p->getUniformSampler("sampler")->set(fb->getTextureBuffer(COLOR0));
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA32F, RGBA, FLOAT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->drawQuad(p);
    float tPixels[4 * 8 * 8];
    float fbPixels[4 * 8 * 8];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA, FLOAT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1.0f && fbPixels[1] == 2.0f && fbPixels[2] == 3.0f && fbPixels[3] == 4.0f &&
        tPixels[0] == 1.0f && tPixels[1] == 2.0f && tPixels[2] == 3.0f && tPixels[3] == 4.0f);
}

TEST(texture2DMultisampleArrayAttachmentAllLayers)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2DMultisampleArray(8, 8, 8, 4, RGBA32F, true), -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    fb->setMultisample(true);
    ptr<Program> p = new Program(new Module(330, (LAYER_INSTANCING + FRAGMENT_SHADER_FLOAT).c_str()));
    fb->drawQuad(p);
    fb->setMultisample(false);
    p = new Program(new Module(330, NULL, "\
        uniform sampler2DMSArray sampler;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = texelFetch(sampler, ivec3(floor(gl_FragCoord.xy), 3), 0); }\n"));
    p->getUniformSampler("sampler")->set(fb->getTextureBuffer(COLOR0));
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA32F, RGBA, FLOAT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->drawQuad(p);
    float tPixels[4 * 8 * 8];
    float fbPixels[4 * 8 * 8];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA, FLOAT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1.0f && fbPixels[1] == 2.0f && fbPixels[2] == 3.0f && fbPixels[3] == 4.0f &&
        tPixels[0] == 1.0f && tPixels[1] == 2.0f && tPixels[2] == 3.0f && tPixels[3] == 4.0f);
}

TEST(texture3DAttachmentOneLayer)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, 3);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    fb->drawQuad(p);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(texture3DAttachmentAllLayers)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, (LAYER_INSTANCING + FRAGMENT_SHADER).c_str()));
    fb->drawQuad(p);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

// TODO TEST(textureCubeAttachmentOneLayer)

// TODO TEST(textureCubeAttachmentAllLayers)

// TODO TEST(textureCubeArrayAttachmentOneLayer)

// TODO TEST(textureCubeArrayAttachmentAllLayers)

TEST(draw)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4);
}

TEST(drawInstancing)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad, 8);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4);
}

TEST(drawPartDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 0, 3);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4 &&
        fbPixels[o] == 0 && fbPixels[o + 1] == 0 && fbPixels[o + 2] == 0 && fbPixels[o + 3] == 0 &&
        tPixels[o] == 0 && tPixels[o + 1] == 0 && tPixels[o + 2] == 0 && tPixels[o + 3] == 0);
}

TEST(drawPartInstancingDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 0, 3, 8);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4 &&
        fbPixels[o] == 0 && fbPixels[o + 1] == 0 && fbPixels[o + 2] == 0 && fbPixels[o + 3] == 0 &&
        tPixels[l + o] == 0 && tPixels[l + o + 1] == 0 && tPixels[l + o + 2] == 0 && tPixels[l + o + 3] == 0);
}

TEST(drawPartIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 3, 3);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[0] == 0 && tPixels[1] == 0 && tPixels[2] == 0 && tPixels[3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[o] == 1 && tPixels[o + 1] == 2 && tPixels[o + 2] == 3 && tPixels[o + 3] == 4);
}

TEST(drawPartInstancingIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 3, 3, 8);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[l] == 0 && tPixels[l + 1] == 0 && tPixels[l + 2] == 0 && tPixels[l + 3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[l + o] == 1 && tPixels[l + o + 1] == 2 && tPixels[l + o + 2] == 3 && tPixels[l + o + 3] == 4);
}

TEST(drawPartIndicesWithBase)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 0, 3, 1, 1);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[0] == 0 && tPixels[1] == 0 && tPixels[2] == 0 && tPixels[3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[o] == 1 && tPixels[o + 1] == 2 && tPixels[o + 2] == 3 && tPixels[o + 3] == 4);
}

TEST(drawPartInstancingIndicesWithBase)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    fb->draw(p, *m, TRIANGLES, 0, 3, 8, 1);
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[l] == 0 && tPixels[l + 1] == 0 && tPixels[l + 2] == 0 && tPixels[l + 3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[l + o] == 1 && tPixels[l + o + 1] == 2 && tPixels[l + o + 2] == 3 && tPixels[l + o + 3] == 4);
}

TEST(multiDrawInstancingDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int firsts[2] = { 0, 18 };
    int counts[2] = { 6, 6 };
    fb->multiDraw(p, *m, TRIANGLES, firsts, counts, 2);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    int a = 4 * (8 - 1);
    int b = 4 * (7 * 8);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[o] == 1 && tPixels[o + 1] == 2 && tPixels[o + 2] == 3 && tPixels[o + 3] == 4 &&
        fbPixels[a] == 0 && fbPixels[a + 1] == 0 && fbPixels[a + 2] == 0 && fbPixels[a + 3] == 0 &&
        tPixels[a] == 0 && tPixels[a + 1] == 0 && tPixels[a + 2] == 0 && tPixels[a + 3] == 0 &&
        fbPixels[b] == 0 && fbPixels[b + 1] == 0 && fbPixels[b + 2] == 0 && fbPixels[b + 3] == 0 &&
        tPixels[b] == 0 && tPixels[b + 1] == 0 && tPixels[b + 2] == 0 && tPixels[b + 3] == 0);
}

TEST(multiDrawInstancingIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(3);
    quad->addIndice(3);
    quad->addIndice(1);
    quad->addIndice(4);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(4);
    quad->addIndice(4);
    quad->addIndice(2);
    quad->addIndice(5);
    quad->addIndice(3);
    quad->addIndice(4);
    quad->addIndice(6);
    quad->addIndice(6);
    quad->addIndice(4);
    quad->addIndice(7);
    quad->addIndice(4);
    quad->addIndice(5);
    quad->addIndice(7);
    quad->addIndice(7);
    quad->addIndice(5);
    quad->addIndice(8);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int firsts[2] = { 0, 18 };
    int counts[2] = { 6, 6 };
    fb->multiDraw(p, *m, TRIANGLES, firsts, counts, 2);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    int a = 4 * (8 - 1);
    int b = 4 * (7 * 8);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[0] == 1 && tPixels[1] == 2 && tPixels[2] == 3 && tPixels[3] == 4 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[o] == 1 && tPixels[o + 1] == 2 && tPixels[o + 2] == 3 && tPixels[o + 3] == 4 &&
        fbPixels[a] == 0 && fbPixels[a + 1] == 0 && fbPixels[a + 2] == 0 && fbPixels[a + 3] == 0 &&
        tPixels[a] == 0 && tPixels[a + 1] == 0 && tPixels[a + 2] == 0 && tPixels[a + 3] == 0 &&
        fbPixels[b] == 0 && fbPixels[b + 1] == 0 && fbPixels[b + 2] == 0 && fbPixels[b + 3] == 0 &&
        tPixels[b] == 0 && tPixels[b + 1] == 0 && tPixels[b + 2] == 0 && tPixels[b + 3] == 0);
}

TEST(multiDrawInstancingIndicesWithBase)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(3);
    quad->addIndice(3);
    quad->addIndice(1);
    quad->addIndice(4);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(4);
    quad->addIndice(4);
    quad->addIndice(2);
    quad->addIndice(5);
    quad->addIndice(3);
    quad->addIndice(4);
    quad->addIndice(6);
    quad->addIndice(6);
    quad->addIndice(4);
    quad->addIndice(7);
    quad->addIndice(4);
    quad->addIndice(5);
    quad->addIndice(7);
    quad->addIndice(7);
    quad->addIndice(5);
    quad->addIndice(8);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int firsts[2] = { 0, 18 };
    int counts[2] = { 6, 6 };
    int bases[2] = { 1, -1 };
    fb->multiDraw(p, *m, TRIANGLES, firsts, counts, 2, bases);
    int tPixels[4 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    int a = 4 * (8 - 1);
    int b = 4 * (7 * 8);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[0] == 0 && tPixels[1] == 0 && tPixels[2] == 0 && tPixels[3] == 0 &&
        fbPixels[o] == 0 && fbPixels[o + 1] == 0 && fbPixels[o + 2] == 0 && fbPixels[o + 3] == 0 &&
        tPixels[o] == 0 && tPixels[o + 1] == 0 && tPixels[o + 2] == 0 && tPixels[o + 3] == 0 &&
        fbPixels[a] == 1 && fbPixels[a + 1] == 2 && fbPixels[a + 2] == 3 && fbPixels[a + 3] == 4 &&
        tPixels[a] == 1 && tPixels[a + 1] == 2 && tPixels[a + 2] == 3 && tPixels[a + 3] == 4 &&
        fbPixels[b] == 1 && fbPixels[b + 1] == 2 && fbPixels[b + 2] == 3 && fbPixels[b + 3] == 4 &&
        tPixels[b] == 1 && tPixels[b + 1] == 2 && tPixels[b + 2] == 3 && tPixels[b + 3] == 4);
}

TEST4(drawIndirectInstancingDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int buf[4] = { 3, 8, 0, 0};
    fb->drawIndirect(p, *m, TRIANGLES, CPUBuffer(buf));
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        tPixels[l] == 1 && tPixels[l + 1] == 2 && tPixels[l + 2] == 3 && tPixels[l + 3] == 4 &&
        fbPixels[o] == 0 && fbPixels[o + 1] == 0 && fbPixels[o + 2] == 0 && fbPixels[o + 3] == 0 &&
        tPixels[l + o] == 0 && tPixels[l + o + 1] == 0 && tPixels[l + o + 2] == 0 && tPixels[l + o + 3] == 0);
}

TEST4(drawIndirectInstancingIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int buf[5] = { 3, 8, 3, 0, 0 };
    fb->drawIndirect(p, *m, TRIANGLES, CPUBuffer(buf));
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[l] == 0 && tPixels[l + 1] == 0 && tPixels[l + 2] == 0 && tPixels[l + 3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[l + o] == 1 && tPixels[l + o + 1] == 2 && tPixels[l + o + 2] == 3 && tPixels[l + o + 3] == 4);
}

TEST4(drawIndirectInstancingIndicesWithBase)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture3D(8, 8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0, -1);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, DRAW_INSTANCING));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLES, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    quad->addIndice(2);
    quad->addIndice(1);
    quad->addIndice(3);
    ptr<MeshBuffers> m = quad->getBuffers();
    fb->clear(true, true, true);
    int buf[5] = { 3, 8, 0, 1, 0 };
    fb->drawIndirect(p, *m, TRIANGLES, CPUBuffer(buf));
    int tPixels[4 * 8 * 8 * 8];
    int fbPixels[4 * 8 * 8];
    int l = 4 * 8 * 8 * 3;
    int o = 4 * (8 * 8 - 1);
    fb->getTextureBuffer(COLOR0)->getImage(0, RGBA_INTEGER, INT, tPixels);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 0 && fbPixels[1] == 0 && fbPixels[2] == 0 && fbPixels[3] == 0 &&
        tPixels[l] == 0 && tPixels[l + 1] == 0 && tPixels[l + 2] == 0 && tPixels[l + 3] == 0 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        tPixels[l + o] == 1 && tPixels[l + o + 1] == 2 && tPixels[l + o + 2] == 3 && tPixels[l + o + 3] == 4);
}

TEST(primitiveRestart)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, GLubyte> > quad = new Mesh<vec4f, GLubyte>(TRIANGLE_STRIP, GPU_STATIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(0, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 0, 0, 1));
    quad->addVertex(vec4f(0, 0, 0, 1));
    quad->addVertex(vec4f(1, 0, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(0, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(3);
    quad->addIndice(255);
    quad->addIndice(7);
    quad->addIndice(5);
    quad->addIndice(8);
    quad->setPrimitiveRestart(255);
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int fbPixels[4 * 8 * 8];
    int o = 4 * (8 * 8 - 1);
    int c = 4 * (8 * 3 + 3);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(fbPixels));
    ASSERT(fbPixels[0] == 1 && fbPixels[1] == 2 && fbPixels[2] == 3 && fbPixels[3] == 4 &&
        fbPixels[o] == 1 && fbPixels[o + 1] == 2 && fbPixels[o + 2] == 3 && fbPixels[o + 3] == 4 &&
        fbPixels[c] == 0 && fbPixels[c + 1] == 0 && fbPixels[c + 2] == 0 && fbPixels[c + 3] == 0);
}

TEST(cpuMeshModificationDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, CPU);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int pixels1[4 * 8 * 8];
    int pixels2[4 * 8 * 8];
    int l = 4 * (8 * 8 - 1);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels1));
    quad->setVertex(0, vec4f(-1, 1, 0, 1));
    quad->setVertex(1, vec4f(1, -1, 0, 1));
    quad->setVertex(2, vec4f(1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels2));
    ASSERT(pixels1[0] == 1 && pixels1[1] == 2 && pixels1[2] == 3 && pixels1[3] == 4 &&
        pixels1[l] == 0 && pixels1[l + 1] == 0 && pixels1[l + 2] == 0 && pixels1[l + 3] == 0 &&
        pixels2[0] == 0 && pixels2[1] == 0 && pixels2[2] == 0 && pixels2[3] == 0 &&
        pixels2[l] == 1 && pixels2[l + 1] == 2 && pixels2[l + 2] == 3 && pixels2[l + 3] == 4);
}

TEST(cpuMeshModificationIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, CPU);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int pixels1[4 * 8 * 8];
    int pixels2[4 * 8 * 8];
    int l = 4 * (8 * 8 - 1);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels1));
    quad->setIndice(0, 2);
    quad->setIndice(1, 1);
    quad->setIndice(2, 3);
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels2));
    ASSERT(pixels1[0] == 1 && pixels1[1] == 2 && pixels1[2] == 3 && pixels1[3] == 4 &&
        pixels1[l] == 0 && pixels1[l + 1] == 0 && pixels1[l + 2] == 0 && pixels1[l + 3] == 0 &&
        pixels2[0] == 0 && pixels2[1] == 0 && pixels2[2] == 0 && pixels2[3] == 0 &&
        pixels2[l] == 1 && pixels2[l + 1] == 2 && pixels2[l + 2] == 3 && pixels2[l + 3] == 4);
}

TEST(gpuMeshModificationDirect)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_DYNAMIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int pixels1[4 * 8 * 8];
    int pixels2[4 * 8 * 8];
    int l = 4 * (8 * 8 - 1);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels1));
    quad->setVertex(0, vec4f(-1, 1, 0, 1));
    quad->setVertex(1, vec4f(1, -1, 0, 1));
    quad->setVertex(2, vec4f(1, 1, 0, 1));
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels2));
    ASSERT(pixels1[0] == 1 && pixels1[1] == 2 && pixels1[2] == 3 && pixels1[3] == 4 &&
        pixels1[l] == 0 && pixels1[l + 1] == 0 && pixels1[l + 2] == 0 && pixels1[l + 3] == 0 &&
        pixels2[0] == 0 && pixels2[1] == 0 && pixels2[2] == 0 && pixels2[3] == 0 &&
        pixels2[l] == 1 && pixels2[l + 1] == 2 && pixels2[l + 2] == 3 && pixels2[l + 3] == 4);
}

TEST(gpuMeshModificationIndices)
{
    ptr<FrameBuffer> fb = new FrameBuffer();
    fb->setTextureBuffer(COLOR0, new Texture2D(8, 8, RGBA8I, RGBA_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)), 0);
    fb->setViewport(vec4<GLint>(0, 0, 8, 8));
    ptr<Program> p = new Program(new Module(330, FRAGMENT_SHADER));
    ptr< Mesh<vec4f, unsigned int> > quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_DYNAMIC);
    quad->addAttributeType(0, 4, A32F, false);
    quad->addVertex(vec4f(-1, -1, 0, 1));
    quad->addVertex(vec4f(1, -1, 0, 1));
    quad->addVertex(vec4f(-1, 1, 0, 1));
    quad->addVertex(vec4f(1, 1, 0, 1));
    quad->addIndice(0);
    quad->addIndice(1);
    quad->addIndice(2);
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    int pixels1[4 * 8 * 8];
    int pixels2[4 * 8 * 8];
    int l = 4 * (8 * 8 - 1);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels1));
    quad->setIndice(0, 2);
    quad->setIndice(1, 1);
    quad->setIndice(2, 3);
    fb->clear(true, true, true);
    fb->draw(p, *quad);
    fb->readPixels(0, 0, 8, 8, RGBA_INTEGER, INT, Buffer::Parameters(), CPUBuffer(pixels2));
    ASSERT(pixels1[0] == 1 && pixels1[1] == 2 && pixels1[2] == 3 && pixels1[3] == 4 &&
        pixels1[l] == 0 && pixels1[l + 1] == 0 && pixels1[l + 2] == 0 && pixels1[l + 3] == 0 &&
        pixels2[0] == 0 && pixels2[1] == 0 && pixels2[2] == 0 && pixels2[3] == 0 &&
        pixels2[l] == 1 && pixels2[l + 1] == 2 && pixels2[l + 2] == 3 && pixels2[l + 3] == 4);
}
