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

#include "ork/render/TextureCubeArray.h"

#include <exception>

#include <GL/glew.h>

#include "ork/render/CPUBuffer.h"
#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"

using namespace std;

namespace ork
{

void getParameters(const ptr<ResourceDescriptor> desc, const TiXmlElement *e, TextureInternalFormat &ff, TextureFormat &f, PixelType &t);

void getParameters(const ptr<ResourceDescriptor> desc, const TiXmlElement *e, Texture::Parameters &params);

GLenum getTextureInternalFormat(TextureInternalFormat f);

GLenum getTextureFormat(TextureFormat f);

GLenum getPixelType(PixelType t);

unsigned int getFormatSize(TextureFormat f, PixelType t);

TextureCubeArray::TextureCubeArray() : Texture("TextureCube", GL_TEXTURE_CUBE_MAP_ARRAY)
{
}

TextureCubeArray::TextureCubeArray(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, Buffer::Parameters s, const Buffer &pixels) : Texture("TextureCube", GL_TEXTURE_CUBE_MAP_ARRAY)
{
    init(w, h, l, tf, f, t, params, s, pixels);
}

void TextureCubeArray::init(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, Buffer::Parameters s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;
    this->h = h;
    this->l = l;

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    if (isCompressed()) {
        glCompressedTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, getTextureInternalFormat(internalFormat), w, h, 6 * l, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, getTextureInternalFormat(internalFormat), w, h, 6 * l, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

TextureCubeArray::~TextureCubeArray()
{
}

int TextureCubeArray::getWidth()
{
    return w;
}

int TextureCubeArray::getHeight()
{
    return h;
}

int TextureCubeArray::getLayers()
{
    return l;
}

void TextureCubeArray::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<TextureCubeArray>()->w);
    std::swap(h, t.cast<TextureCubeArray>()->h);
    std::swap(l, t.cast<TextureCubeArray>()->l);
}

/// @cond RESOURCES

class TextureCubeArrayResource : public ResourceTemplate<0, TextureCubeArray>
{
public:
    TextureCubeArrayResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, TextureCubeArray>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s;
        int w;
        int h;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,wrapt,minLod,maxLod,width,height,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &h);
            if (h % (6 * w) != 0) {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, e, "Inconsistent 'width' and 'height' attributes");
                }
                throw exception();
            }
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);

            try {
                init(w, w, h / (6 * w), tf, f, t, params, s, CPUBuffer(desc->getData()));
            } catch (...) {
                throw exception();
            }
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char textureCubeArray[] = "textureCubeArray";

static ResourceFactory::Type<textureCubeArray, TextureCubeArrayResource> TextureCubeArrayType;

/// @endcond

}
