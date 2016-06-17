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

#include "ork/render/Texture2D.h"

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

Texture2D::Texture2D() : Texture("Texture2D", GL_TEXTURE_2D)
{
}

Texture2D::Texture2D(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("Texture2D", GL_TEXTURE_2D)
{
    init(w, h, tf, f, t, params, s, pixels);
}

void Texture2D::init(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);

    this->w = w;
    this->h = h;

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);

    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, h, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, h, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture2D::~Texture2D()
{
}

int Texture2D::getWidth()
{
    return w;
}

int Texture2D::getHeight()
{
    return h;
}

void Texture2D::setImage(int w, int h, TextureFormat f, PixelType t, const Buffer &pixels)
{
    this->w = w;
    this->h = h;
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, h, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    generateMipMap();

    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture2D::setSubImage(int level, int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage2D(textureTarget, level, x, y, w, h, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture2D::setCompressedSubImage(int level, int x, int y, int w, int h, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage2D(textureTarget, level, x, y, w, h, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture2D::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<Texture2D>()->w);
    std::swap(h, t.cast<Texture2D>()->h);
}

/// @cond RESOURCES

class Texture2DResource : public ResourceTemplate<0, Texture2D>
{
public:
    Texture2DResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, Texture2D>(manager, name, desc)
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
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,wrapt,maxAniso,width,height,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &h);
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);
            s.compressedSize(desc->getSize());
            init(w, h, tf, f, t, params, s, CPUBuffer(desc->getData()));
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char texture2D[] = "texture2D";

static ResourceFactory::Type<texture2D, Texture2DResource> Texture2DType;

/// @endcond

}
