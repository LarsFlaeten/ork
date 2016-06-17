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

#include "ork/render/Texture.h"

#include <algorithm>
#include <exception>

#include <GL/glew.h>

#include "ork/resource/ResourceManager.h"
#include "ork/render/FrameBuffer.h"

using namespace std;

namespace ork
{

ORK_API void getParameters(const ptr<ResourceDescriptor> desc, const TiXmlElement *e, TextureInternalFormat &ff, TextureFormat &f, PixelType &t)
{
    const char* v = e->Attribute("internalformat");
    if (v == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Missing 'internalformat' attribute");
        }
        throw exception();
    } else if (strcmp(v, "R8") == 0) {
        ff = R8;
    } else if (strcmp(v, "R8SNORM") == 0) {
        ff = R8_SNORM;
    } else if (strcmp(v, "R16") == 0) {
        ff = R16;
    } else if (strcmp(v, "R16_SNORM") == 0) {
        ff = R16_SNORM;
    } else if (strcmp(v, "RG8") == 0) {
        ff = RG8;
    } else if (strcmp(v, "RG8_SNORM") == 0) {
        ff = RG8_SNORM;
    } else if (strcmp(v, "RG16") == 0) {
        ff = RG16;
    } else if (strcmp(v, "RG16_SNORM") == 0) {
        ff = RG16_SNORM;
    } else if (strcmp(v, "R3_G3_B2") == 0) {
        ff = R3_G3_B2;
    } else if (strcmp(v, "RGB4") == 0) {
        ff = RGB4;
    } else if (strcmp(v, "RGB5") == 0) {
        ff = RGB5;
    } else if (strcmp(v, "RGB8") == 0) {
        ff = RGB8;
    } else if (strcmp(v, "RGB8_SNORM") == 0) {
        ff = RGB8_SNORM;
    } else if (strcmp(v, "RGB10") == 0) {
        ff = RGB10;
    } else if (strcmp(v, "RGB12") == 0) {
        ff = RGB12;
    } else if (strcmp(v, "RGB16") == 0) {
        ff = RGB16;
    } else if (strcmp(v, "RGB16_SNORM") == 0) {
        ff = RGB16_SNORM;
    } else if (strcmp(v, "RGBA2") == 0) {
        ff = RGBA2;
    } else if (strcmp(v, "RGBA4") == 0) {
        ff = RGBA4;
    } else if (strcmp(v, "RGB5_A1") == 0) {
        ff = RGB5_A1;
    } else if (strcmp(v, "RGBA8") == 0) {
        ff = RGBA8;
    } else if (strcmp(v, "RGBA8_SNORM") == 0) {
        ff = RGBA8_SNORM;
    } else if (strcmp(v, "RGB10_A2") == 0) {
        ff = RGB10_A2;
    } else if (strcmp(v, "RGB10_A2UI") == 0) {
        ff = RGB10_A2UI;
    } else if (strcmp(v, "RGBA12") == 0) {
        ff = RGBA12;
    } else if (strcmp(v, "RGBA16") == 0) {
        ff = RGBA16;
    } else if (strcmp(v, "RGBA16_SNORM") == 0) {
        ff = RGBA16_SNORM;
    } else if (strcmp(v, "SRGB8") == 0) {
        ff = SRGB8;
    } else if (strcmp(v, "SRGB8_ALPHA8") == 0) {
        ff = SRGB8_ALPHA8;
    } else if (strcmp(v, "R16F") == 0) {
        ff = R16F;
    } else if (strcmp(v, "RG16F") == 0) {
        ff = RG16F;
    } else if (strcmp(v, "RGB16F") == 0) {
        ff = RGB16F;
    } else if (strcmp(v, "RGBA16F") == 0) {
        ff = RGBA16F;
    } else if (strcmp(v, "R32F") == 0) {
        ff = R32F;
    } else if (strcmp(v, "RG32F") == 0) {
        ff = RG32F;
    } else if (strcmp(v, "RGB32F") == 0) {
        ff = RGB32F;
    } else if (strcmp(v, "RGBA32F") == 0) {
        ff = RGBA32F;
    } else if (strcmp(v, "R11F_G11F_B10F") == 0) {
        ff = R11F_G11F_B10F;
    } else if (strcmp(v, "RGB9_E5") == 0) {
        ff = RGB9_E5;
    } else if (strcmp(v, "R8I") == 0) {
        ff = R8I;
    } else if (strcmp(v, "R8UI") == 0) {
        ff = R8UI;
    } else if (strcmp(v, "R16I") == 0) {
        ff = R16I;
    } else if (strcmp(v, "R16UI") == 0) {
        ff = R16UI;
    } else if (strcmp(v, "R32I") == 0) {
        ff = R32I;
    } else if (strcmp(v, "R32UI") == 0) {
        ff = R32UI;
    } else if (strcmp(v, "RG8I") == 0) {
        ff = RG8I;
    } else if (strcmp(v, "RG8UI") == 0) {
        ff = RG8UI;
    } else if (strcmp(v, "RG16I") == 0) {
        ff = RG16I;
    } else if (strcmp(v, "RG16UI") == 0) {
        ff = RG16UI;
    } else if (strcmp(v, "RG32I") == 0) {
        ff = RG32I;
    } else if (strcmp(v, "RG32UI") == 0) {
        ff = RG32UI;
    } else if (strcmp(v, "RGB8I") == 0) {
        ff = RGB8I;
    } else if (strcmp(v, "RGB8UI") == 0) {
        ff = RGB8UI;
    } else if (strcmp(v, "RGB16I") == 0) {
        ff = RGB16I;
    } else if (strcmp(v, "RGB16UI") == 0) {
        ff = RGB16UI;
    } else if (strcmp(v, "RGB32I") == 0) {
        ff = RGB32I;
    } else if (strcmp(v, "RGB32UI") == 0) {
        ff = RGB32UI;
    } else if (strcmp(v, "RGBA8I") == 0) {
        ff = RGBA8I;
    } else if (strcmp(v, "RGBA8UI") == 0) {
        ff = RGBA8UI;
    } else if (strcmp(v, "RGBA16I") == 0) {
        ff = RGBA16I;
    } else if (strcmp(v, "RGBA16UI") == 0) {
        ff = RGBA16UI;
    } else if (strcmp(v, "RGBA32I") == 0) {
        ff = RGBA32I;
    } else if (strcmp(v, "RGBA32UI") == 0) {
        ff = RGBA32UI;
    } else if (strcmp(v, "DEPTH_COMPONENT16") == 0) {
        ff = DEPTH_COMPONENT16;
    } else if (strcmp(v, "DEPTH_COMPONENT24") == 0) {
        ff = DEPTH_COMPONENT24;
    } else if (strcmp(v, "DEPTH_COMPONENT32F") == 0) {
        ff = DEPTH_COMPONENT32F;
    } else if (strcmp(v, "DEPTH32F_STENCIL8") == 0) {
        ff = DEPTH32F_STENCIL8;
    } else if (strcmp(v, "DEPTH24_STENCIL8") == 0) {
        ff = DEPTH24_STENCIL8;
    } else if (strcmp(v, "COMPRESSED_RED") == 0) {
        ff = COMPRESSED_RED;
    } else if (strcmp(v, "COMPRESSED_RG") == 0) {
        ff = COMPRESSED_RG;
    } else if (strcmp(v, "COMPRESSED_RGB") == 0) {
        ff = COMPRESSED_RGB;
    } else if (strcmp(v, "COMPRESSED_RGBA") == 0) {
        ff = COMPRESSED_RGBA;
    } else if (strcmp(v, "COMPRESSED_SRGB") == 0) {
        ff = COMPRESSED_SRGB;
    } else if (strcmp(v, "COMPRESSED_RED_RGTC1") == 0) {
        ff = COMPRESSED_RED_RGTC1;
    } else if (strcmp(v, "COMPRESSED_SIGNED_RED_RGTC1") == 0) {
        ff = COMPRESSED_SIGNED_RED_RGTC1;
    } else if (strcmp(v, "COMPRESSED_RG_RGTC2") == 0) {
        ff = COMPRESSED_RG_RGTC2;
    } else if (strcmp(v, "COMPRESSED_SIGNED_RG_RGTC2") == 0) {
        ff = COMPRESSED_SIGNED_RG_RGTC2;
    } else if (strcmp(v, "COMPRESSED_RGBA_BPTC_UNORM_ARB") == 0) {
        ff = COMPRESSED_RGBA_BPTC_UNORM_ARB;
    } else if (strcmp(v, "COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB") == 0) {
        ff = COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB;
    } else if (strcmp(v, "COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB") == 0) {
        ff = COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
    } else if (strcmp(v, "COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB") == 0) {
        ff = COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;
    } else if (strcmp(v, "COMPRESSED_RGB_S3TC_DXT1_EXT") == 0) {
        ff = COMPRESSED_RGB_S3TC_DXT1_EXT;
    } else if (strcmp(v, "COMPRESSED_RGBA_S3TC_DXT1_EXT") == 0) {
        ff = COMPRESSED_RGBA_S3TC_DXT1_EXT;
    } else if (strcmp(v, "COMPRESSED_RGBA_S3TC_DXT3_EXT") == 0) {
        ff = COMPRESSED_RGBA_S3TC_DXT3_EXT;
    } else if (strcmp(v, "COMPRESSED_RGBA_S3TC_DXT5_EXT") == 0) {
        ff = COMPRESSED_RGBA_S3TC_DXT5_EXT;
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad 'internalformat' attribute");
        }
        throw exception();
    }
    v = e->Attribute("format");
    if (v == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Missing 'format' attribute");
        }
        throw exception();
    } else if (strcmp(v, "STENCIL_INDEX") == 0) {
        f = STENCIL_INDEX;
    } else if (strcmp(v, "DEPTH_COMPONENT") == 0) {
        f = DEPTH_COMPONENT;
    } else if (strcmp(v, "DEPTH_STENCIL") == 0) {
        f = DEPTH_STENCIL;
    } else if (strcmp(v, "RED") == 0) {
        f = RED;
    } else if (strcmp(v, "GREEN") == 0) {
        f = GREEN;
    } else if (strcmp(v, "BLUE") == 0) {
        f = BLUE;
    } else if (strcmp(v, "RG") == 0) {
        f = RG;
    } else if (strcmp(v, "RGB") == 0) {
        f = RGB;
    } else if (strcmp(v, "RGBA") == 0) {
        f = RGBA;
    } else if (strcmp(v, "BGR") == 0) {
        f = BGR;
    } else if (strcmp(v, "BGRA") == 0) {
        f = BGRA;
    } else if (strcmp(v, "RED_INTEGER") == 0) {
        f = RED_INTEGER;
    } else if (strcmp(v, "BLUE_INTEGER") == 0) {
        f = BLUE_INTEGER;
    } else if (strcmp(v, "GREEN_INTEGER") == 0) {
        f = GREEN_INTEGER;
    } else if (strcmp(v, "RG_INTEGER") == 0) {
        f = RG_INTEGER;
    } else if (strcmp(v, "RGB_INTEGER") == 0) {
        f = RGB_INTEGER;
    } else if (strcmp(v, "RGBA_INTEGER") == 0) {
        f = RGBA_INTEGER;
    } else if (strcmp(v, "BGR_INTEGER") == 0) {
        f = BGR_INTEGER;
    } else if (strcmp(v, "BGRA_INTEGER") == 0) {
        f = BGRA_INTEGER;
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad 'format' attribute");
        }
        throw exception();
    }

    v = e->Attribute("type");
    if (v == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Missing 'type' attribute");
        }
        throw exception();
    } else if (strncmp(v, "UNSIGNED_BYTE", 13) == 0) {
        t = UNSIGNED_BYTE;
    } else if (strcmp(v, "BYTE") == 0) {
        t = BYTE;
    } else if (strncmp(v, "UNSIGNED_SHORT", 14) == 0) {
        t = UNSIGNED_SHORT;
    } else if (strcmp(v, "SHORT") == 0) {
        t = SHORT;
    } else if (strncmp(v, "UNSIGNED_INT", 12) == 0) {
        t = UNSIGNED_INT;
    } else if (strcmp(v, "INT") == 0) {
        t = INT;
    } else if (strcmp(v, "FLOAT") == 0) {
        t = FLOAT;
    } else if (strcmp(v, "HALF") == 0) {
        t = HALF;
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad 'type' attribute");
        }
        throw exception();
    }
}

ORK_API void getParameters(const ptr<ResourceDescriptor> desc, const TiXmlElement *e, Texture::Parameters &params)
{
    const char* v = e->Attribute("min");
    if (v == NULL) {
        // params.min(NEAREST);
    } else if (strcmp(v, "NEAREST") == 0) {
        params.min(NEAREST);
    } else if (strcmp(v, "LINEAR") == 0) {
        params.min(LINEAR);
    } else if (strcmp(v, "NEAREST_MIPMAP_NEAREST") == 0) {
        params.min(NEAREST_MIPMAP_NEAREST);
    } else if (strcmp(v, "NEAREST_MIPMAP_LINEAR") == 0) {
        params.min(NEAREST_MIPMAP_LINEAR);
    } else if (strcmp(v, "LINEAR_MIPMAP_NEAREST") == 0) {
        params.min(LINEAR_MIPMAP_NEAREST);
    } else if (strcmp(v, "LINEAR_MIPMAP_LINEAR") == 0) {
        params.min(LINEAR_MIPMAP_LINEAR);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad minification attribute");
        }
        throw exception();
    }

    v = e->Attribute("mag");
    if (v == NULL) {
        // params.min(NEAREST);
    } else if (strcmp(v, "NEAREST") == 0) {
        params.mag(NEAREST);
    } else if (strcmp(v, "LINEAR") == 0) {
        params.mag(LINEAR);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad magnification attribute");
        }
        throw exception();
    }

    v = e->Attribute("wraps");
    if (v == NULL) {
        // params.wrapS("CLAMP_TO_BORDER");
    } else if (strcmp(v, "CLAMP_TO_EDGE") == 0) {
        params.wrapS(CLAMP_TO_EDGE);
    } else if (strcmp(v, "CLAMP_TO_BORDER") == 0) {
        params.wrapS(CLAMP_TO_BORDER);
    } else if (strcmp(v, "REPEAT") == 0) {
        params.wrapS(REPEAT);
    } else if (strcmp(v, "MIRRORED_REPEAT") == 0) {
        params.wrapS(MIRRORED_REPEAT);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad wrap S attribute");
        }
        throw exception();
    }

    v = e->Attribute("wrapt");
    if (v == NULL) {
        // params.wrapT("CLAMP_TO_BORDER");
    } else if (strcmp(v, "CLAMP_TO_EDGE") == 0) {
        params.wrapT(CLAMP_TO_EDGE);
    } else if (strcmp(v, "CLAMP_TO_BORDER") == 0) {
        params.wrapT(CLAMP_TO_BORDER);
    } else if (strcmp(v, "REPEAT") == 0) {
        params.wrapT(REPEAT);
    } else if (strcmp(v, "MIRRORED_REPEAT") == 0) {
        params.wrapT(MIRRORED_REPEAT);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad wrap T attribute");
        }
        throw exception();
    }

    v = e->Attribute("wrapr");
    if (v == NULL) {
        // params.wrapR("CLAMP_TO_BORDER");
    } else if (strcmp(v, "CLAMP_TO_EDGE") == 0) {
        params.wrapR(CLAMP_TO_EDGE);
    } else if (strcmp(v, "CLAMP_TO_BORDER") == 0) {
        params.wrapR(CLAMP_TO_BORDER);
    } else if (strcmp(v, "REPEAT") == 0) {
        params.wrapR(REPEAT);
    } else if (strcmp(v, "MIRRORED_REPEAT") == 0) {
        params.wrapR(MIRRORED_REPEAT);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad wrap R attribute");
        }
        throw exception();
    }

    v = e->Attribute("borderType");
    int borderType = 0;
    if (v == NULL) {
    } else if (strcmp(v, "INT") == 0) {
        borderType = 0;
    } else if (strcmp(v, "FLOAT") == 0) {
        borderType = 1;
    } else if (strcmp(v, "IINT") == 0) {
        borderType = 2;
    } else if (strcmp(v, "IUNSIGNED_INT") == 0) {
        borderType = 3;
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad border type attribute");
        }
        throw exception();
    }

    if (e->Attribute("borderr") != NULL) {
        float r, g, b, a;
        Resource::getFloatParameter(desc, e, "borderr", &r);
        Resource::getFloatParameter(desc, e, "borderg", &g);
        Resource::getFloatParameter(desc, e, "borderb", &b);
        Resource::getFloatParameter(desc, e, "bordera", &a);
        switch(borderType) {
            case 0:
                params.borderi(GLint(r), GLint(g), GLint(b), GLint(a));
                break;
            case 1:
                params.borderf(GLfloat(r), GLfloat(g), GLfloat(b), GLfloat(a));
                break;
            case 2:
                params.borderIi(GLint(r), GLint(g), GLint(b), GLint(a));
                break;
            case 3:
                params.borderIui(GLuint(r), GLuint(g), GLuint(b), GLuint(a));
                break;
        }
    }

    if (e->Attribute("minLevel") != NULL) {
        float level;
        Resource::getFloatParameter(desc, e, "minLevel", &level);
        params.minLevel(level);
    }
    if (e->Attribute("maxLevel") != NULL) {
        float level;
        Resource::getFloatParameter(desc, e, "maxLevel", &level);
        params.maxLevel(level);
    }
    if (e->Attribute("minLod") != NULL) {
        float lod;
        Resource::getFloatParameter(desc, e, "minLod", &lod);
        params.lodMin(lod);
    }
    if (e->Attribute("maxLod") != NULL) {
        float lod;
        Resource::getFloatParameter(desc, e, "maxLod", &lod);
        params.lodMax(lod);
    }
    if (e->Attribute("bias") != NULL) {
        float lod;
        Resource::getFloatParameter(desc, e, "bias", &lod);
        params.lodBias(lod);
    }

    if (e->Attribute("anisotropy") != NULL) {
        float a;
        Resource::getFloatParameter(desc, e, "anisotropy", &a);
        params.maxAnisotropyEXT(a);
    }

    v = e->Attribute("compare");
    if (v == NULL) {
    } else if (strcmp(v, "LEQUAL") == 0) {
        params.compareFunc(LEQUAL);
    } else if (strcmp(v, "GEQUAL") == 0) {
        params.compareFunc(GEQUAL);
    } else if (strcmp(v, "LESS") == 0) {
        params.compareFunc(LESS);
    } else if (strcmp(v, "GREATER") == 0) {
        params.compareFunc(GREATER);
    } else if (strcmp(v, "EQUAL") == 0) {
        params.compareFunc(EQUAL);
    } else if (strcmp(v, "NOTEQUAL") == 0) {
        params.compareFunc(NOTEQUAL);
    } else if (strcmp(v, "ALWAYS") == 0) {
        params.compareFunc(ALWAYS);
    } else if (strcmp(v, "NEVER") == 0) {
        params.compareFunc(NEVER);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad compare function attribute");
        }
        throw exception();
    }

    v = e->Attribute("swizzle");
    if (v != NULL) {
        if (strlen(v) == 4) {
            params.swizzle(v[0], v[1], v[2], v[3]);
        } else {
            if (Logger::ERROR_LOGGER != NULL) {
                Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad swizzle attribute");
            }
            throw exception();
        }
    }
}

GLenum getTextureFormat(TextureFormat f);

GLenum getPixelType(PixelType t);

GLenum getTextureWrap(TextureWrap w);

GLenum getTextureFilter(TextureFilter f);

GLenum getFunction(Function f);

GLenum getTextureSwizzle(char c);

unsigned int getTextureComponents(TextureFormat f);

const char *getTextureInternalFormatName(TextureInternalFormat f);

/**
 * A texture unit.
 */
class TextureUnit
{
public:
    /**
     * Creates a new texture unit.
     *
     * @param unit the index of this texture unit.
     */
    TextureUnit(GLuint unit) :
        unit(unit), lastBindingTime(0), currentSamplerBinding(NULL), currentTextureBinding(NULL)
    {
    }

    /**
     * Binds a sampler and a texture to this texture unit. This unbinds the
     * previously bound sampler and texture.
     *
     * @param sampler the Sampler to bind to this unit.
     * @param tex the Texture to bind to this unit.
     * @param time the current time.
     */
    void bind(const Sampler* sampler, const Texture* tex, unsigned int time)
    {
        lastBindingTime = time; // always update time, or LRU won't work

        GLuint currentSamplerId = currentSamplerBinding == NULL ? 0 : currentSamplerBinding->getId();
        GLuint samplerId = sampler == NULL ? 0 : sampler->getId();

        glActiveTexture(GL_TEXTURE0 + unit);

        if (sampler != currentSamplerBinding) {
            glBindSampler(unit, samplerId);
            currentSamplerBinding = sampler;
        }

        if (tex != currentTextureBinding) {
            if (currentTextureBinding != NULL) {
                map<GLuint, GLuint>::iterator i = currentTextureBinding->currentTextureUnits.find(currentSamplerId);
                assert(i != currentTextureBinding->currentTextureUnits.end());
                currentTextureBinding->currentTextureUnits.erase(i);
                if (tex == NULL || currentTextureBinding->textureTarget != tex->textureTarget) {
                    glBindTexture(currentTextureBinding->textureTarget, 0);
                }
            }
            if (tex != NULL) {
                tex->currentTextureUnits.insert(make_pair(samplerId, unit));
                glBindTexture(tex->textureTarget, tex->textureId);
            }
            currentTextureBinding = tex;
        }

        assert(FrameBuffer::getError() == 0);
    }

    unsigned int getLastBindingTime() const
    {
        return lastBindingTime;
    }

    bool isFree() const
    {
        return currentTextureBinding == NULL;
    }

    const Sampler* getCurrentSamplerBinding() const
    {
        return currentSamplerBinding;
    }

    const Texture* getCurrentTextureBinding() const
    {
        return currentTextureBinding;
    }

private:
    /**
     * The index of this texture unit (between 0 ... MAX_TEXTURE_UNITS).
     */
    GLuint unit;

    /**
     * The 'time' at which the textures currently bound to the OpenGL texture
     * unit were bound. This time is used to find the last recently bound
     * texture when a texture must be unbound to bind a new one.
     */
    unsigned int lastBindingTime;

    /**
     * The sampler currently bound to this texture unit.
     */
    const Sampler* currentSamplerBinding;

    /**
     * The texture currently bound to this texture unit.
     */
    const Texture* currentTextureBinding;
};

/**
 * Manages texture units.
 */
class TextureUnitManager
{
public:
    /**
     * Creates a new TextureUnitManager.
     */
    TextureUnitManager()
    {
        GLuint maxUnits = getMaxTextureUnits();
        for (GLuint i = 0; i < maxUnits; ++i) {
            units[i] = new TextureUnit(i);
        }

        time = 0;
    }

    /**
     * Finds a free texture unit and return its index. If no texture unit is
     * free, evicts the least recently bound texture not used in the given
     * programs.
     *
     * @return a free texture unit, or -1 if all units are used by the given
     *      program.
     */
    int findFreeTextureUnit(const vector<GLuint> &programIds)
    {
        // we first try to find an unused texture unit
        for (GLuint i = 0; i < maxUnits; ++i) {
            if (units[i]->isFree()) {
                return i;
            }
        }

        // if all the texture units are used we must unbind a texture to free
        // a texture unit; we choose the least recently used unit that is not
        // used by the current program

        int bestUnit = -1;
        unsigned int oldestBindingTime = time;

        for (GLuint i = 0; i < maxUnits; ++i) {
            const Texture *t = units[i]->getCurrentTextureBinding();
            if (!t->isUsedBy(programIds)) {
                unsigned int bindingTime = units[i]->getLastBindingTime();
                if (bestUnit == -1 || bindingTime < oldestBindingTime) {
                    bestUnit = i;
                    oldestBindingTime = bindingTime;
                }
            }
        }

        // if you fail here, there is no more texture unit available
        assert(bestUnit != -1);

        return bestUnit;
    }

    /**
     * Binds a sampler and a texture to a given texture unit.
     */
    void bind(unsigned int i, const Sampler* sampler, const Texture* tex)
    {
        units[i]->bind(sampler, tex, time++);
    }

    void unbind(const Texture *tex)
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            if (units[i]->getCurrentTextureBinding() == tex) {
                units[i]->bind(NULL, NULL, time++);
            }
        }
    }

    void unbind(const Sampler *sampler)
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            if (units[i]->getCurrentSamplerBinding() == sampler) {
                units[i]->bind(NULL, NULL, time++);
            }
        }
    }

    void unbindAll()
    {
        for (GLuint i = 0; i < maxUnits; ++i) {
            units[i]->bind(NULL, NULL, 0);
        }
        time = 0;
    }

    static unsigned int getMaxTextureUnits()
    {
        if (maxUnits == 0) {
            GLint maxVertexTextureImageUnits;
            GLint maxFragmentTextureImageUnits;
            GLint maxCombinedTextureImageUnits;

            glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentTextureImageUnits);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);

            // Ork does not support more than MAX_TEXTURE_UNITS units,
            // Because this state is stored in static array of size MAX_TEXTURE_UNITS.
            maxUnits = std::min(maxCombinedTextureImageUnits, MAX_TEXTURE_UNITS);

            if (Logger::DEBUG_LOGGER != NULL) {
                Logger::DEBUG_LOGGER->logf("OPENGL", "MAX_VERTEX_TEXTURE_IMAGE_UNITS = %d", maxVertexTextureImageUnits);
                Logger::DEBUG_LOGGER->logf("OPENGL", "MAX_FRAGEMNT_TEXTURE_IMAGE_UNITS = %d", maxFragmentTextureImageUnits);
                Logger::DEBUG_LOGGER->logf("OPENGL", "MAX_COMBINED_TEXTURE_IMAGE_UNITS = %d", maxCombinedTextureImageUnits);
            }
        }
        return maxUnits;
    }

private:
    /**
     * The state of all texture units.
     * maxUnits elements.
     */
    TextureUnit *units[MAX_TEXTURE_UNITS];

    /**
     * The 'time' used to measure the texture binding times. This abstract time
     * is an integer that is incremented each time a texture is bound.
     */
    unsigned int time;

    /**
     * Maximum number of texture units on the current graphics card.
     */
    static GLuint maxUnits;

    friend class Texture;
};

GLuint TextureUnitManager::maxUnits = 0;

static TextureUnitManager *TEXTURE_UNIT_MANAGER = NULL;

Texture::Parameters::Parameters() : Sampler::Parameters(),
        _minLevel(0), _maxLevel(1000)
{
    _swizzle[0] = 'r';
    _swizzle[1] = 'g';
    _swizzle[2] = 'b';
    _swizzle[3] = 'a';
}

Texture::Parameters::~Parameters()
{
}

TextureWrap Texture::Parameters::wrapS() const
{
    return Sampler::Parameters::wrapS();
}

TextureWrap Texture::Parameters::wrapT() const
{
    return Sampler::Parameters::wrapT();
}

TextureWrap Texture::Parameters::wrapR() const
{
    return Sampler::Parameters::wrapR();
}

TextureFilter Texture::Parameters::min() const
{
    return Sampler::Parameters::min();
}

TextureFilter Texture::Parameters::mag() const
{
    return Sampler::Parameters::mag();
}

unsigned int Texture::Parameters::borderType() const
{
    return Sampler::Parameters::borderType();
}

const GLint* Texture::Parameters::borderi() const
{
    return Sampler::Parameters::borderi();
}

const GLfloat* Texture::Parameters::borderf() const
{
    return Sampler::Parameters::borderf();
}

const GLint* Texture::Parameters::borderIi() const
{
    return Sampler::Parameters::borderIi();
}

const GLuint* Texture::Parameters::borderIui() const
{
    return Sampler::Parameters::borderIui();
}

GLfloat Texture::Parameters::lodMin() const
{
    return Sampler::Parameters::lodMin();
}

GLfloat Texture::Parameters::lodMax() const
{
    return Sampler::Parameters::lodMax();
}

GLfloat Texture::Parameters::lodBias() const
{
    return Sampler::Parameters::lodBias();
}

Function Texture::Parameters::compareFunc() const
{
    return Sampler::Parameters::compareFunc();
}

GLfloat Texture::Parameters::maxAnisotropyEXT() const
{
    return Sampler::Parameters::maxAnisotropyEXT();
}

const char* Texture::Parameters::swizzle() const
{
    return _swizzle;
}

GLint Texture::Parameters::minLevel() const
{
    return _minLevel;
}

GLint Texture::Parameters::maxLevel() const
{
    return _maxLevel;
}

Texture::Parameters& Texture::Parameters::wrapS(TextureWrap wrapS)
{
    Sampler::Parameters::wrapS(wrapS);
    return *this;
}

Texture::Parameters& Texture::Parameters::wrapT(TextureWrap wrapT)
{
    Sampler::Parameters::wrapT(wrapT);
    return *this;
}

Texture::Parameters& Texture::Parameters::wrapR(TextureWrap wrapR)
{
    Sampler::Parameters::wrapR(wrapR);
    return *this;
}

Texture::Parameters& Texture::Parameters::min(TextureFilter min)
{
    Sampler::Parameters::min(min);
    return *this;
}

Texture::Parameters& Texture::Parameters::mag(TextureFilter mag)
{
    Sampler::Parameters::mag(mag);
    return *this;
}

Texture::Parameters& Texture::Parameters::borderi(GLint r, GLint g, GLint b, GLint a)
{
    Sampler::Parameters::borderi(r, g, b, a);
    return *this;
}

Texture::Parameters& Texture::Parameters::borderf(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    Sampler::Parameters::borderf(r, g, b, a);
    return *this;
}

Texture::Parameters& Texture::Parameters::borderIi(GLint r, GLint g, GLint b, GLint a)
{
    Sampler::Parameters::borderIi(r, g, b, a);
    return *this;
}

Texture::Parameters& Texture::Parameters::borderIui(GLuint r, GLuint g, GLuint b, GLuint a)
{
    Sampler::Parameters::borderIui(r, g, b, a);
    return *this;
}

Texture::Parameters& Texture::Parameters::lodMin(GLfloat lodMin)
{
    Sampler::Parameters::lodMin(lodMin);
    return *this;
}

Texture::Parameters& Texture::Parameters::lodMax(GLfloat lodMax)
{
    Sampler::Parameters::lodMax(lodMax);
    return *this;
}

Texture::Parameters& Texture::Parameters::lodBias(GLfloat lodBias)
{
    Sampler::Parameters::lodBias(lodBias);
    return *this;
}

Texture::Parameters& Texture::Parameters::compareFunc(Function compareFunc)
{
    Sampler::Parameters::compareFunc(compareFunc);
    return *this;
}

Texture::Parameters& Texture::Parameters::maxAnisotropyEXT(GLfloat maxAnisotropy)
{
    Sampler::Parameters::maxAnisotropyEXT(maxAnisotropy);
    return *this;
}

Texture::Parameters& Texture::Parameters::swizzle(char r, char g, char b, char a)
{
    _swizzle[0] = r;
    _swizzle[1] = g;
    _swizzle[2] = b;
    _swizzle[3] = a;
    return *this;
}

Texture::Parameters& Texture::Parameters::minLevel(GLint minLevel)
{
    _minLevel = minLevel;
    return *this;
}

Texture::Parameters& Texture::Parameters::maxLevel(GLint maxLevel)
{
    _maxLevel = maxLevel;
    return *this;
}

Texture::Texture(const char *type, int t) : Object(type), textureTarget(t)
{
    if (TEXTURE_UNIT_MANAGER == NULL) {
        TEXTURE_UNIT_MANAGER = new TextureUnitManager();
    }
}

void Texture::init(TextureInternalFormat tf, const Texture::Parameters &params)
{
    glGenTextures(1, &textureId);
    assert(textureId > 0);

    this->internalFormat = tf;
    this->params = params;

    bindToTextureUnit();

    if (textureTarget == GL_TEXTURE_BUFFER) {
        assert(FrameBuffer::getError() == 0);
        return;
    }

    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, getTextureWrap(params.wrapS()));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, getTextureWrap(params.wrapT()));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, getTextureWrap(params.wrapR()));
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, getTextureFilter(params.min()));
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, getTextureFilter(params.mag()));
    switch (params.borderType()) {
    case 0: // i
        glTexParameteriv(textureTarget, GL_TEXTURE_BORDER_COLOR, params.borderi());
        break;
    case 1: // f
        glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, params.borderf());
        break;
    case 2: // Ii
        glTexParameterIiv(textureTarget, GL_TEXTURE_BORDER_COLOR, params.borderIi());
        break;
    case 3: // Iui
        glTexParameterIuiv(textureTarget, GL_TEXTURE_BORDER_COLOR, params.borderIui());
        break;
    default:
        assert(false);
    }
    if (textureTarget != GL_TEXTURE_RECTANGLE) {
        glTexParameterf(textureTarget, GL_TEXTURE_MIN_LOD, params.lodMin());
        glTexParameterf(textureTarget, GL_TEXTURE_MAX_LOD, params.lodMax());
    }

    glTexParameterf(textureTarget, GL_TEXTURE_LOD_BIAS, params.lodBias());
    if (params.compareFunc() != ALWAYS) {
        glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(textureTarget, GL_TEXTURE_COMPARE_FUNC, getFunction(params.compareFunc()));
    }
    glTexParameterf(textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.maxAnisotropyEXT());
    glTexParameteri(textureTarget, GL_TEXTURE_SWIZZLE_R, getTextureSwizzle(params.swizzle()[0]));
    glTexParameteri(textureTarget, GL_TEXTURE_SWIZZLE_G, getTextureSwizzle(params.swizzle()[1]));
    glTexParameteri(textureTarget, GL_TEXTURE_SWIZZLE_B, getTextureSwizzle(params.swizzle()[2]));
    glTexParameteri(textureTarget, GL_TEXTURE_SWIZZLE_A, getTextureSwizzle(params.swizzle()[3]));
    if (textureTarget != GL_TEXTURE_RECTANGLE) {
        glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, params.minLevel());
        glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, params.maxLevel());
    }

    assert(FrameBuffer::getError() == 0);
}

Texture::~Texture()
{
    TEXTURE_UNIT_MANAGER->unbind(this);

    glDeleteTextures(1, &textureId);
    assert(FrameBuffer::getError() == 0);
}

GLuint Texture::getId() const
{
    return textureId;
}

const char *Texture::getInternalFormatName() const
{
    return getTextureInternalFormatName(internalFormat);
}

unsigned int Texture::getComponents() const
{
    return getTextureComponents(getFormat());
}

TextureInternalFormat Texture::getInternalFormat() const
{
    return internalFormat;
}

TextureFormat Texture::getFormat() const
{
    switch (internalFormat) {
    case R8:
    case R8_SNORM:
    case R16:
    case R16_SNORM:
    case COMPRESSED_RED:
    case R16F:
    case R32F:
    case COMPRESSED_RED_RGTC1:
    case COMPRESSED_SIGNED_RED_RGTC1:
        return RED;
    case R8I:
    case R8UI:
    case R16I:
    case R16UI:
    case R32I:
    case R32UI:
        return RED_INTEGER;
    case RG8:
    case RG8_SNORM:
    case RG16:
    case RG16_SNORM:
    case RG16F:
    case RG32F:
    case COMPRESSED_RG:
    case COMPRESSED_RG_RGTC2:
    case COMPRESSED_SIGNED_RG_RGTC2:
        return RG;
    case RG8I:
    case RG8UI:
    case RG16I:
    case RG16UI:
    case RG32I:
    case RG32UI:
        return RG_INTEGER;
    case R3_G3_B2:
    case RGB4:
    case RGB5:
    case RGB8:
    case RGB8_SNORM:
    case RGB10:
    case RGB12:
    case RGB16:
    case RGB16_SNORM:
    case SRGB8:
    case RGB16F:
    case RGB32F:
    case R11F_G11F_B10F:
    case RGB9_E5:
    case COMPRESSED_RGB:
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
        return RGB;
    case RGB8I:
    case RGB8UI:
    case RGB16I:
    case RGB16UI:
    case RGB32I:
    case RGB32UI:
        return RGB_INTEGER;
    case RGB5_A1:
    case RGBA2:
    case RGBA4:
    case RGBA8:
    case RGBA8_SNORM:
    case RGB10_A2:
    case RGB10_A2UI:
    case RGBA12:
    case RGBA16:
    case RGBA16_SNORM:
    case SRGB8_ALPHA8:
    case RGBA16F:
    case RGBA32F:
        return RGBA;
    case DEPTH_COMPONENT16:
    case DEPTH_COMPONENT24:
    case DEPTH_COMPONENT32F:
        return DEPTH_COMPONENT;
    case DEPTH24_STENCIL8:
    case DEPTH32F_STENCIL8:
        return DEPTH_STENCIL;
    case COMPRESSED_RGBA:
    case COMPRESSED_SRGB:
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return RGBA;
    case RGBA8I:
    case RGBA8UI:
    case RGBA16I:
    case RGBA16UI:
    case RGBA32I:
    case RGBA32UI:
        return RGBA_INTEGER;
    }
    assert(false);
    throw exception();
}

bool Texture::hasMipmaps() const
{
    return params.min() != NEAREST && params.min() != LINEAR;
}

bool Texture::isCompressed() const
{
    switch (internalFormat) {
    case R8:
    case R8_SNORM:
    case R16:
    case R16_SNORM:
    case RG8:
    case RG8_SNORM:
    case RG16:
    case RG16_SNORM:
    case R3_G3_B2:
    case RGB4:
    case RGB5:
    case RGB8:
    case RGB8_SNORM:
    case RGB10:
    case RGB12:
    case RGB16:
    case RGB16_SNORM:
    case RGBA2:
    case RGBA4:
    case RGB5_A1:
    case RGBA8:
    case RGBA8_SNORM:
    case RGB10_A2:
    case RGB10_A2UI:
    case RGBA12:
    case RGBA16:
    case RGBA16_SNORM:
    case SRGB8:
    case SRGB8_ALPHA8:
    case R16F:
    case RG16F:
    case RGB16F:
    case RGBA16F:
    case R32F:
    case RG32F:
    case RGB32F:
    case RGBA32F:
    case R11F_G11F_B10F:
    case RGB9_E5:
    case R8I:
    case R8UI:
    case R16I:
    case R16UI:
    case R32I:
    case R32UI:
    case RG8I:
    case RG8UI:
    case RG16I:
    case RG16UI:
    case RG32I:
    case RG32UI:
    case RGB8I:
    case RGB8UI:
    case RGB16I:
    case RGB16UI:
    case RGB32I:
    case RGB32UI:
    case RGBA8I:
    case RGBA8UI:
    case RGBA16I:
    case RGBA16UI:
    case RGBA32I:
    case RGBA32UI:
    case DEPTH_COMPONENT16:
    case DEPTH_COMPONENT24:
    case DEPTH_COMPONENT32F:
    case DEPTH24_STENCIL8:
    case DEPTH32F_STENCIL8:
        return false;
    case COMPRESSED_RED:
    case COMPRESSED_RG:
    case COMPRESSED_RGB:
    case COMPRESSED_RGBA:
    case COMPRESSED_SRGB:
    case COMPRESSED_RED_RGTC1:
    case COMPRESSED_SIGNED_RED_RGTC1:
    case COMPRESSED_RG_RGTC2:
    case COMPRESSED_SIGNED_RG_RGTC2:
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return true;
    }
    assert(false);
    throw exception();
}

GLsizei Texture::getCompressedSize(int level) const
{
    GLint size;
    bindToTextureUnit();
    glGetTexLevelParameteriv(textureTarget, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
    assert(FrameBuffer::getError() == 0);
    return GLsizei(size);
}

void Texture::getImage(int level, TextureFormat f, PixelType t, void *pixels)
{
    bindToTextureUnit();
    glGetTexImage(textureTarget, level, getTextureFormat(f), getPixelType(t), pixels);
    assert(FrameBuffer::getError() == 0);
}

void Texture::getCompressedImage(int level, void *pixels) const
{
    bindToTextureUnit();
    glGetCompressedTexImage(textureTarget, level, pixels);
    assert(FrameBuffer::getError() == 0);
}

void Texture::generateMipMap()
{
    if (hasMipmaps()) {
        bindToTextureUnit();
        glGenerateMipmap(textureTarget);
        assert(FrameBuffer::getError() == 0);
    } else {
        // do nothing, but this could be an error
    }
}

GLint Texture::bindToTextureUnit(ptr<Sampler> sampler, const vector<GLuint> &programIds) const
{
    GLuint samplerId = sampler == NULL ? 0 : sampler->getId();
    map<GLuint, GLuint>::iterator i = currentTextureUnits.find(samplerId);

    GLint unit;
    if (i == currentTextureUnits.end()) {
        unit = TEXTURE_UNIT_MANAGER->findFreeTextureUnit(programIds);
    } else {
        unit = i->second;
    }

    TEXTURE_UNIT_MANAGER->bind(GLuint(unit), sampler.get(), this);

    return unit;
}

GLint Texture::bindToTextureUnit() const
{
    if (currentTextureUnits.empty()) {
        GLint unit;
        if (Program::CURRENT == NULL) {
            unit = TEXTURE_UNIT_MANAGER->findFreeTextureUnit(vector<GLuint>());
        } else {
            unit = TEXTURE_UNIT_MANAGER->findFreeTextureUnit(Program::CURRENT->programIds);
        }
        assert(unit != -1);
        TEXTURE_UNIT_MANAGER->bind(GLuint(unit), NULL, this);
        return unit;
    } else {
        GLuint unit = currentTextureUnits.begin()->second;
        glActiveTexture(GL_TEXTURE0 + unit);
        return unit;
    }
}

void Texture::swap(ptr<Texture> t)
{
    TEXTURE_UNIT_MANAGER->unbind(this);
    TEXTURE_UNIT_MANAGER->unbind(t.get());
    if (Program::CURRENT != NULL && isUsedBy(Program::CURRENT->programIds)) {
        Program::CURRENT = NULL;
    }
    assert(textureTarget == t->textureTarget);
    std::swap(textureId, t->textureId);
    std::swap(internalFormat, t->internalFormat);
    std::swap(params, t->params);
}

void Texture::addUser(GLuint programId) const
{
    assert(find(programIds.begin(), programIds.end(), programId) == programIds.end());
    programIds.push_back(programId);
}

void Texture::removeUser(GLuint programId) const
{
    vector<GLuint>::iterator i = find(programIds.begin(), programIds.end(), programId);
    assert(i != programIds.end());
    programIds.erase(i);
}

bool Texture::isUsedBy(const vector<GLuint> &programId) const
{
    return find_first_of(programIds.begin(), programIds.end(), programId.begin(), programId.end()) != programIds.end();
}

unsigned int Texture::getMaxTextureUnits()
{
    return TextureUnitManager::getMaxTextureUnits();
}

void Texture::unbindSampler(Sampler *sampler)
{
    TEXTURE_UNIT_MANAGER->unbind(sampler);
}

void Texture::unbindAll()
{
    TEXTURE_UNIT_MANAGER->unbindAll();
}

}
