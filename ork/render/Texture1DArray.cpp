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

#include "ork/render/Texture1DArray.h"

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

Texture1DArray::Texture1DArray() : Texture("Texture1DArray", GL_TEXTURE_1D_ARRAY)
{
}

Texture1DArray::Texture1DArray(int w, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("Texture1DArray", GL_TEXTURE_1D_ARRAY)
{
    init(w, l, tf, f, t, params, s, pixels);
}

void Texture1DArray::init(int w, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;
    this->l = l;

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, l, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, l, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture1DArray::~Texture1DArray()
{
}

int Texture1DArray::getWidth()
{
    return w;
}

int Texture1DArray::getLayers()
{
    return l;
}

void Texture1DArray::setSubImage(int level, int x, int l, int w, int d, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage2D(GL_TEXTURE_2D, level, x, l, w, d, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture1DArray::setCompressedSubImage(int level, int x, int l, int w, int d, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage2D(GL_TEXTURE_2D, level, x, l, w, d, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture1DArray::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<Texture1DArray>()->w);
    std::swap(l, t.cast<Texture1DArray>()->l);
}

/// @cond RESOURCES

class Texture1DArrayResource : public ResourceTemplate<0, Texture1DArray>
{
public:
    Texture1DArrayResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, Texture1DArray>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s;
        int w;
        int l;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,width,height,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &l);
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);
            s.compressedSize(desc->getSize());
            init(w, l, tf, f, t, params, s, CPUBuffer(desc->getData()));
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char texture1DArray[] = "texture1DArray";

static ResourceFactory::Type<texture1DArray, Texture1DArrayResource> Texture1DArrayType;

/// @endcond

}
