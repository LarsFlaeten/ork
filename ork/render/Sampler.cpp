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

#include "ork/render/Sampler.h"

#include <GL/glew.h>

#include "ork/math/vec4.h"
#include "ork/render/Texture.h"

using namespace std;

namespace ork
{

GLenum getTextureWrap(TextureWrap w);

GLenum getTextureFilter(TextureFilter f);

GLenum getFunction(Function f);

Sampler::Parameters::Parameters() :
        _wrapS(CLAMP_TO_EDGE), _wrapT(CLAMP_TO_EDGE), _wrapR(CLAMP_TO_EDGE),
        _min(NEAREST), _mag(LINEAR), _borderType(0),
        _lodMin(-1000.0f), _lodMax(1000.0f), _lodBias(0.0f),
        _maxAnisotropy(1.0f), _compareFunc(ALWAYS)
{
    _border.i[0] = 0;
    _border.i[1] = 0;
    _border.i[2] = 0;
    _border.i[3] = 0;
}

Sampler::Parameters::~Parameters()
{
}

TextureWrap Sampler::Parameters::wrapS() const
{
    return _wrapS;
}

TextureWrap Sampler::Parameters::wrapT() const
{
    return _wrapT;
}

TextureWrap Sampler::Parameters::wrapR() const
{
    return _wrapR;
}

TextureFilter Sampler::Parameters::min() const
{
    return _min;
}

TextureFilter Sampler::Parameters::mag() const
{
    return _mag;
}

unsigned int Sampler::Parameters::borderType() const
{
    return _borderType;
}

const GLint* Sampler::Parameters::borderi() const
{
    return _border.i;
}

const GLfloat* Sampler::Parameters::borderf() const
{
    return _border.f;
}

const GLint* Sampler::Parameters::borderIi() const
{
    return _border.i;
}

const GLuint* Sampler::Parameters::borderIui() const
{
    return _border.ui;
}

GLfloat Sampler::Parameters::lodMin() const
{
    return _lodMin;
}

GLfloat Sampler::Parameters::lodMax() const
{
    return _lodMax;
}

GLfloat Sampler::Parameters::lodBias() const
{
    return _lodBias;
}

Function Sampler::Parameters::compareFunc() const
{
    return _compareFunc;
}

GLfloat Sampler::Parameters::maxAnisotropyEXT() const
{
    return _maxAnisotropy;
}

Sampler::Parameters& Sampler::Parameters::wrapS(TextureWrap wrapS)
{
    _wrapS = wrapS;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::wrapT(TextureWrap wrapT)
{
    _wrapT = wrapT;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::wrapR(TextureWrap wrapR)
{
    _wrapR = wrapR;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::min(TextureFilter min)
{
    _min = min;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::mag(TextureFilter mag)
{
    _mag = mag;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::borderi(GLint r, GLint g, GLint b, GLint a)
{
    _border.i[0] = r;
    _border.i[1] = g;
    _border.i[2] = b;
    _border.i[3] = a;
    _borderType = 0;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::borderf(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    _border.f[0] = r;
    _border.f[1] = g;
    _border.f[2] = b;
    _border.f[3] = a;
    _borderType = 1;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::borderIi(GLint r, GLint g, GLint b, GLint a)
{
    _border.i[0] = r;
    _border.i[1] = g;
    _border.i[2] = b;
    _border.i[3] = a;
    _borderType = 2;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::borderIui(GLuint r, GLuint g, GLuint b, GLuint a)
{
    _border.ui[0] = r;
    _border.ui[1] = g;
    _border.ui[2] = b;
    _border.ui[3] = a;
    _borderType = 3;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::lodMin(GLfloat lodMin)
{
    _lodMin = lodMin;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::lodMax(GLfloat lodMax)
{
    _lodMax = lodMax;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::lodBias(GLfloat lodBias)
{
    _lodBias = lodBias;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::maxAnisotropyEXT(GLfloat maxAnisotropy)
{
    _maxAnisotropy = maxAnisotropy;
    return *this;
}

Sampler::Parameters& Sampler::Parameters::compareFunc(Function compareFunc)
{
    _compareFunc = compareFunc;
    return *this;
}

bool Sampler::Parameters::operator<(const Parameters &v) const
{
    if (_wrapS < v._wrapS) {
        return true;
    }
    if (_wrapS > v._wrapS) {
        return false;
    }
    if (_wrapT < v._wrapT) {
        return true;
    }
    if (_wrapT > v._wrapT) {
        return false;
    }
    if (_wrapR < v._wrapR) {
        return true;
    }
    if (_wrapR > v._wrapR) {
        return false;
    }
    if (_min < v._min) {
        return true;
    }
    if (_mag > v._mag) {
        return false;
    }
    if (_borderType < v._borderType) {
        return true;
    }
    if (_borderType > v._borderType) {
        return false;
    }
    switch (_borderType) {
    case 0: // i
    case 2: { // Ii
        vec4<GLint> ub(_border.i[0], _border.i[1], _border.i[2], _border.i[3]);
        vec4<GLint> vb(v._border.i[0], v._border.i[1], v._border.i[2], v._border.i[3]);
        if (ub < vb) {
            return true;
        }
        if (vb < ub) {
            return false;
        }
        break;
    }
    case 3: { // Iui
        vec4<GLuint> ub(_border.ui[0], _border.ui[1], _border.ui[2], _border.ui[3]);
        vec4<GLuint> vb(v._border.ui[0], v._border.ui[1], v._border.ui[2], v._border.ui[3]);
        if (ub < vb) {
            return true;
        }
        if (vb < ub) {
            return false;
        }
        break;
    }
    case 1: { // f
        vec4<GLfloat> ub(_border.f[0], _border.f[1], _border.f[2], _border.f[3]);
        vec4<GLfloat> vb(v._border.f[0], v._border.f[1], v._border.f[2], v._border.f[3]);
        if (ub < vb) {
            return true;
        }
        if (vb < ub) {
            return false;
        }
        break;
    }
    default:
        assert(false);
    }
    if (_lodMin < v._lodMin) {
        return true;
    }
    if (_lodMin > v._lodMin) {
        return false;
    }
    if (_lodMax < v._lodMax) {
        return true;
    }
    if (_lodMax > v._lodMax) {
        return false;
    }
    if (_lodBias < v._lodBias) {
        return true;
    }
    if (_lodBias > v._lodBias) {
        return false;
    }
    if (_maxAnisotropy < v._maxAnisotropy) {
        return true;
    }
    if (_maxAnisotropy > v._maxAnisotropy) {
        return false;
    }
    return _compareFunc < v._compareFunc;
}

Sampler::Sampler(const Parameters &params) : Object("Sampler"), params(params)
{
    map<Parameters, pair<GLuint, unsigned int> >::iterator i = INSTANCES.find(params);
    if (i == INSTANCES.end()) {
        glGenSamplers(1, &samplerId);
        assert(samplerId > 0);
        glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, getTextureWrap(params.wrapS()));
        glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, getTextureWrap(params.wrapT()));
        glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_R, getTextureWrap(params.wrapR()));
        glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, getTextureFilter(params.min()));
        glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, getTextureFilter(params.mag()));
        switch (params.borderType()) {
        case 0: // i
            glSamplerParameteriv(samplerId, GL_TEXTURE_BORDER_COLOR, params.borderi());
            break;
        case 1: // f
            glSamplerParameterfv(samplerId, GL_TEXTURE_BORDER_COLOR, params.borderf());
            break;
        case 2: // Ii
            glSamplerParameterIiv(samplerId, GL_TEXTURE_BORDER_COLOR, params.borderIi());
            break;
        case 3: // Iui
            glSamplerParameterIuiv(samplerId, GL_TEXTURE_BORDER_COLOR, params.borderIui());
            break;
        default:
            assert(false);
        }
        glSamplerParameterf(samplerId, GL_TEXTURE_MIN_LOD, params.lodMin());
        glSamplerParameterf(samplerId, GL_TEXTURE_MAX_LOD, params.lodMax());
        glSamplerParameterf(samplerId, GL_TEXTURE_LOD_BIAS, params.lodBias());
        if (params.compareFunc() != ALWAYS) {
            glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glSamplerParameteri(samplerId, GL_TEXTURE_COMPARE_FUNC, getFunction(params.compareFunc()));
        }
        glSamplerParameterf(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.maxAnisotropyEXT());
        INSTANCES[params] = make_pair(samplerId, 1);
    } else {
        samplerId = i->second.first;
        INSTANCES[params] = make_pair(samplerId, i->second.second + 1);
    }
}

Sampler::~Sampler()
{
    Texture::unbindSampler(this);
    map<Parameters, pair<GLuint, unsigned int> >::iterator i = INSTANCES.find(params);
    assert(i != INSTANCES.end());
    assert(i->second.first == samplerId);
    assert(i->second.second >= 1);
    if (i->second.second == 1) {
        glDeleteSamplers(1, &samplerId);
        INSTANCES.erase(i);
    } else {
        INSTANCES[params] = make_pair(samplerId, i->second.second - 1);
    }
}

GLuint Sampler::getId() const
{
    return samplerId;
}

map<Sampler::Parameters, pair<GLuint, unsigned int> > Sampler::INSTANCES;

}
