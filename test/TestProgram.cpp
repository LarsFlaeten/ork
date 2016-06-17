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
using namespace std;

ptr<FrameBuffer> getFrameBuffer(RenderBuffer::RenderBufferFormat f, int w, int h);

TEST(testProgramBinary)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32F, 1, 1);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform float u;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(u, 0.0, 0.0, 0.0); }\n"));
    p->getUniform1f("u")->set(1.0f);
    GLfloat pixels1[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels1));
    GLsizei length;
    GLenum format;
    unsigned char *binary = p->getBinary(length, format);
    p = new Program(format, length, binary);
    p->getUniform1f("u")->set(2.0f);
    GLfloat pixels2[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels2));
    ASSERT(pixels1[0] == 1.0f && pixels2[0] == 2.0f);
}

TEST(testProgramPipeline)
{
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RG32F, 1, 1);
    ptr<Program> vertex = new Program(new Module(330, "\
        uniform float u;\n\
        layout(location=0) in vec4 p;\n\
        out float v;\n\
        void main() { v = u; gl_Position = p; }\n", NULL), true);
    ptr<Program> fragment = new Program(new Module(330, NULL, "\
        uniform float u;\n\
        in float v;\n\
        layout(location=0) out vec4 color;\n\
        void main() { color = vec4(v, u, 0.0, 0.0); }\n"), true);
    ptr<Program> p = new Program(vertex, NULL, NULL, NULL, fragment);
    vertex->getUniform1f("u")->set(1.0f);
    fragment->getUniform1f("u")->set(2.0f);
    GLfloat pixels[4];
    fb->drawQuad(p);
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&pixels));
    ASSERT(pixels[0] == 1.0f && pixels[1] == 2.0f);
}

TEST(testProgramPipelineAutomaticTextureBinding)
{
    vector< ptr<Texture2D> > textures;
    for (int i = 0; i < 128; ++i) {
        textures.push_back(new Texture2D(1, 1, R32I, RED_INTEGER, INT,
            Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(&i)));
    }
    vector< ptr<Program> > programs;
    for (int i = 0; i < 48; ++i) {
        ptr<Program> vertex = new Program(new Module(330, "\
            uniform isampler2D tex1;\n\
            uniform isampler2D tex2;\n\
            layout(location=0) in vec4 p;\n\
            flat out ivec4 c1;\n\
            flat out ivec4 c2;\n\
            void main() { c1 = texture(tex1, vec2(0.0)); c2 = texture(tex2, vec2(0.0)); gl_Position = p; }\n", NULL), true);
        ptr<Program> fragment = new Program(new Module(330, NULL, "\
            uniform isampler2D tex3;\n\
            uniform isampler2D tex4;\n\
            flat in ivec4 c1;\n\
            flat in ivec4 c2;\n\
            layout(location=0) out ivec4 color;\n\
            void main() { color = c1 + c2 + texture(tex3, vec2(0.0)) + texture(tex4, vec2(0.0)); }\n"), true);
        ptr<Program> p = new Program(vertex, NULL, NULL, NULL, fragment);
        vertex->getUniformSampler("tex1")->set(textures[(3 * i) % 128]);
        vertex->getUniformSampler("tex2")->set(textures[(3 * i + 1) % 128]);
        fragment->getUniformSampler("tex3")->set(textures[(3 * i + 2) % 128]);
        fragment->getUniformSampler("tex4")->set(textures[(3 * i + 3) % 128]);
        programs.push_back(p);
    }
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
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

TEST(testProgramPipelineAutomaticSamplerBinding)
{
    vector< ptr<Texture2D> > textures;
    for (int i = 0; i < 8; ++i) {
        textures.push_back(new Texture2D(1, 1, R32I, RED_INTEGER, INT,
            Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(NULL)));
    }
    vector< ptr<Sampler> > samplers;
    for (int i = 0; i < 128; ++i) {
        samplers.push_back(new Sampler(Sampler::Parameters().wrapS(CLAMP_TO_BORDER).wrapT(CLAMP_TO_BORDER).borderIi(i, 0, 0, 0)));
    }
    vector< ptr<Program> > programs;
    for (int i = 0; i < 48; ++i) {
        ptr<Program> vertex = new Program(new Module(330, "\
            uniform isampler2D tex1;\n\
            uniform isampler2D tex2;\n\
            layout(location=0) in vec4 p;\n\
            flat out ivec4 c1;\n\
            flat out ivec4 c2;\n\
            void main() { c1 = texture(tex1, vec2(-1.0)); c2 = texture(tex2, vec2(-1.0)); gl_Position = p; }\n", NULL), true);
        ptr<Program> fragment = new Program(new Module(330, NULL, "\
            uniform isampler2D tex3;\n\
            uniform isampler2D tex4;\n\
            flat in ivec4 c1;\n\
            flat in ivec4 c2;\n\
            layout(location=0) out ivec4 color;\n\
            void main() { color = c1 + c2 + texture(tex3, vec2(-1.0)) + texture(tex4, vec2(-1.0)); }\n"), true);
        ptr<Program> p = new Program(vertex, NULL, NULL, NULL, fragment);
        vertex->getUniformSampler("tex1")->set(textures[i % 8]);
        vertex->getUniformSampler("tex2")->set(textures[(i + 1) % 8]);
        fragment->getUniformSampler("tex3")->set(textures[(i + 2) % 8]);
        fragment->getUniformSampler("tex4")->set(textures[(i + 3) % 8]);
        vertex->getUniformSampler("tex1")->setSampler(samplers[(3 * i) % 128]);
        vertex->getUniformSampler("tex2")->setSampler(samplers[(3 * i + 1) % 128]);
        fragment->getUniformSampler("tex3")->setSampler(samplers[(3 * i + 2) % 128]);
        fragment->getUniformSampler("tex4")->setSampler(samplers[(3 * i + 3) % 128]);
        programs.push_back(p);
    }
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 1, 1);
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

TEST(testTransformFeedback)
{
    ptr<Module> m = new Module(330, "\
            layout(location=0) in vec4 p;\n\
            out vec4 q;\n\
            void main() { q = p; }\n", NULL);
    m->setFeedbackMode(true);
    m->addFeedbackVarying("q");
    ptr<Program> p = new Program(m, NULL);
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
}

TEST(testTransformFeedback2)
{
    ptr<Module> m = new Module(330, "\
            layout(location=0) in vec4 p;\n\
            out vec4 q;\n\
            void main() { q = p; }\n", NULL);
    m->setFeedbackMode(true);
    m->addFeedbackVarying("q");
    ptr<Program> p = new Program(m, NULL);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr< Mesh<vec4f, unsigned int> > pt = new Mesh<vec4f, unsigned int>(POINTS, GPU_STATIC);
    pt->addAttributeType(0, 4, A32F, false);
    pt->addVertex(vec4f(1.0, 2.0, 3.0, 4.0));
    ptr<TransformFeedback> tfb = TransformFeedback::getDefault();
    ptr<GPUBuffer> b = new GPUBuffer();
    b->setData(128, NULL, STREAM_COPY);
    tfb->setVertexBuffer(0, b);
    TransformFeedback::begin(fb, p, POINTS, tfb, false);
    TransformFeedback::transform(*(pt->getBuffers()), 0, 1);
    TransformFeedback::end();
    ptr<Program> r = new Program(new Module(330, "\
            layout(location=0) in vec4 p;\n\
            out vec4 c;\n\
            void main() { gl_Position = vec4(0.5, 0.5, 0.0, 1.0); c = p; }\n", "\
            in vec4 c;\n\
            layout(location=0) out vec4 color;\n\
            void main() { color = c; }\n"));
    ptr<MeshBuffers> mb = new MeshBuffers();
    mb->addAttributeBuffer(0, 4, 16, A32F, false);
    mb->getAttributeBuffer(0)->setBuffer(b);
    fb->draw(r, *mb, POINTS, 0, 1);
    float data[4];
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&data));
    ASSERT(data[0] == 1.0f && data[1] == 2.0f && data[2] == 3.0f && data[3] == 4.0f);
}

TEST4(testTransformFeedback3)
{
    ptr<Module> m = new Module(330, "\
            layout(location=0) in vec4 p;\n\
            out vec4 q;\n\
            void main() { q = p; }\n", NULL);
    m->setFeedbackMode(true);
    m->addFeedbackVarying("q");
    ptr<Program> p = new Program(m, NULL);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::RGBA32F, 1, 1);
    ptr< Mesh<vec4f, unsigned int> > pt = new Mesh<vec4f, unsigned int>(POINTS, GPU_STATIC);
    pt->addAttributeType(0, 4, A32F, false);
    pt->addVertex(vec4f(1.0, 2.0, 3.0, 4.0));
    ptr<TransformFeedback> tfb = new TransformFeedback();
    ptr<GPUBuffer> b = new GPUBuffer();
    b->setData(128, NULL, STREAM_COPY);
    tfb->setVertexBuffer(0, b);
    TransformFeedback::begin(fb, p, POINTS, tfb, false);
    TransformFeedback::transform(*(pt->getBuffers()), 0, 1);
    TransformFeedback::end();
    ptr<Program> r = new Program(new Module(330, "\
            layout(location=0) in vec4 p;\n\
            out vec4 c;\n\
            void main() { gl_Position = vec4(0.5, 0.5, 0.0, 1.0); c = p; }\n", "\
            in vec4 c;\n\
            layout(location=0) out vec4 color;\n\
            void main() { color = c; }\n"));
    ptr<MeshBuffers> mb = new MeshBuffers();
    mb->addAttributeBuffer(0, 4, 16, A32F, false);
    mb->getAttributeBuffer(0)->setBuffer(b);
    fb->drawFeedback(r, *mb, POINTS, *tfb);
    float data[4];
    fb->readPixels(0, 0, 1, 1, RGBA, FLOAT, Buffer::Parameters(), CPUBuffer(&data));
    ASSERT(data[0] == 1.0f && data[1] == 2.0f && data[2] == 3.0f && data[3] == 4.0f);
}
