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

TEST(textureBuffer)
{
    GLbyte in[4] = { 1, 2, 3, 4 };
    GLint out[4];
    ptr<GPUBuffer> b = new GPUBuffer();
    b->setData(4, in, STATIC_DRAW);
    ptr<Texture> t = new TextureBuffer(R8I, b);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isamplerBuffer tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = texelFetch(tex, int(floor(gl_FragCoord.x))); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 4, 1);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 4, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
}

TEST(texture1D)
{
    GLint in[4] = { 1, 2, 3, 4 };
    GLint out[4];
    ptr<Texture> t = new Texture1D(4, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler1D tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { color = texture(tex, gl_FragCoord.x / 4.0); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 4, 1);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 4, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
}

TEST(texture1DArray)
{
    GLint in[4] = { 1, 2, 3, 4 };
    GLint out[4];
    ptr<Texture> t = new Texture1DArray(2, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler1DArray tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { ivec2 uv = ivec2(floor(gl_FragCoord.xy)); color = texture(tex, uv); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 2, 2);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 2, 2, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
}

TEST(texture2D)
{
    GLint in[4] = { 1, 2, 3, 4 };
    GLint out[4];
    ptr<Texture> t = new Texture2D(2, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler2D tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { ivec2 uv = ivec2(floor(gl_FragCoord.xy)); color = texture(tex, uv); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 2, 2);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 2, 2, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
}

TEST(textureRectangle)
{
    GLint in[4] = { 1, 2, 3, 4 };
    GLint out[4];
    ptr<Texture> t = new TextureRectangle(2, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler2DRect tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { ivec2 uv = ivec2(floor(gl_FragCoord.xy)); color = texture(tex, uv); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 2, 2);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 2, 2, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4);
}

TEST(texture2DArray)
{
    GLint in[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    GLint out[4];
    ptr<Texture> t = new Texture2DArray(2, 2, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler2DArray tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { ivec2 uv = ivec2(floor(gl_FragCoord.xy)); color = texture(tex, vec3(uv, 1.0)); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 2, 2);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 2, 2, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 5 && out[1] == 6 && out[2] == 7 && out[3] == 8);
}

TEST(texture3D)
{
    GLint in[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    GLint out[4];
    ptr<Texture> t = new Texture3D(2, 2, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isampler3D tex;\n\
        layout(location=0) out ivec4 color;\n\
        void main() { ivec2 uv = ivec2(floor(gl_FragCoord.xy)); color = texture(tex, vec3(uv, 0.75)); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 2, 2);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 2, 2, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 5 && out[1] == 6 && out[2] == 7 && out[3] == 8);
}

TEST(textureCube)
{
    GLint in[6] = { 1, 2, 3, 4, 5, 6 };
    GLint out[6];
    Buffer::Parameters params[6] = {
        Buffer::Parameters(),
        Buffer::Parameters(),
        Buffer::Parameters(),
        Buffer::Parameters(),
        Buffer::Parameters(),
        Buffer::Parameters()
    };
    ptr<Buffer> buffers[6] = {
        new CPUBuffer(in),
        new CPUBuffer(in + 1),
        new CPUBuffer(in + 2),
        new CPUBuffer(in + 3),
        new CPUBuffer(in + 4),
        new CPUBuffer(in + 5)
    };
    ptr<Texture> t = new TextureCube(1, 1, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST), params, buffers);
    ptr<Program> p = new Program(new Module(330, NULL, "\
        uniform isamplerCube tex;\n\
        layout(location=0) out ivec4 color;\n\
        const vec3 dir[6] = vec3[](vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0));\n\
        void main() { color = texture(tex, dir[int(floor(gl_FragCoord.x))]); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 6, 1);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 6, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 4 && out[4] == 5 && out[5] == 6);
}

TEST4(textureCubeArray)
{
    GLint in[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    GLint out[6];
    ptr<Texture> t = new TextureCubeArray(1, 1, 2, R8I, RED_INTEGER, INT,
        Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(in));
    ptr<Program> p = new Program(new Module(400, NULL, "\
        #extension GL_ARB_texture_cube_map_array : enable\n\
        uniform isamplerCubeArray tex;\n\
        layout(location=0) out ivec4 color;\n\
        const vec3 dir[6] = vec3[](vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0));\n\
        void main() { color = texture(tex, vec4(dir[int(floor(gl_FragCoord.x))], 1.0)); }\n"));
    p->getUniformSampler("tex")->set(t);
    ptr<FrameBuffer> fb = getFrameBuffer(RenderBuffer::R32I, 6, 1);
    fb->drawQuad(p);
    fb->readPixels(0, 0, 6, 1, RED_INTEGER, INT, Buffer::Parameters(), CPUBuffer(out));
    ASSERT(out[0] == 7 && out[1] == 8 && out[2] == 9 && out[3] == 10 && out[4] == 11 && out[5] == 12);
}

TEST(automaticTextureBinding)
{
    vector< ptr<Texture2D> > textures;
    for (int i = 0; i < 128; ++i) {
        textures.push_back(new Texture2D(1, 1, R32I, RED_INTEGER, INT,
            Texture::Parameters().mag(NEAREST),  Buffer::Parameters(), CPUBuffer(&i)));
    }
    vector< ptr<Program> > programs;
    for (int i = 0; i < 48; ++i) {
        ptr<Program> p = new Program(new Module(330, NULL, "\
            uniform isampler2D tex1;\n\
            uniform isampler2D tex2;\n\
            uniform isampler2D tex3;\n\
            uniform isampler2D tex4;\n\
            layout(location=0) out ivec4 color;\n\
            void main() { color = texture(tex1, vec2(0.0)) + texture(tex2, vec2(0.0)) + texture(tex3, vec2(0.0)) + texture(tex4, vec2(0.0)); }\n"));
        p->getUniformSampler("tex1")->set(textures[(3 * i) % 128]);
        p->getUniformSampler("tex2")->set(textures[(3 * i + 1) % 128]);
        p->getUniformSampler("tex3")->set(textures[(3 * i + 2) % 128]);
        p->getUniformSampler("tex4")->set(textures[(3 * i + 3) % 128]);
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

TEST(automaticSamplerBinding)
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
        ptr<Program> p = new Program(new Module(330, NULL, "\
            uniform isampler2D tex1;\n\
            uniform isampler2D tex2;\n\
            uniform isampler2D tex3;\n\
            uniform isampler2D tex4;\n\
            layout(location=0) out ivec4 color;\n\
            void main() { color = texture(tex1, vec2(-1.0)) + texture(tex2, vec2(-1.0)) + texture(tex3, vec2(-1.0)) + texture(tex4, vec2(-1.0)); }\n"));
        p->getUniformSampler("tex1")->set(textures[i % 8]);
        p->getUniformSampler("tex2")->set(textures[(i + 1) % 8]);
        p->getUniformSampler("tex3")->set(textures[(i + 2) % 8]);
        p->getUniformSampler("tex4")->set(textures[(i + 3) % 8]);
        p->getUniformSampler("tex1")->setSampler(samplers[(3 * i) % 128]);
        p->getUniformSampler("tex2")->setSampler(samplers[(3 * i + 1) % 128]);
        p->getUniformSampler("tex3")->setSampler(samplers[(3 * i + 2) % 128]);
        p->getUniformSampler("tex4")->setSampler(samplers[(3 * i + 3) % 128]);
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
