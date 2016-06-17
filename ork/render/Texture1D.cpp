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

#include "ork/render/Texture1D.h"

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

Texture1D::Texture1D() : Texture("Texture1D", GL_TEXTURE_1D)
{
}

Texture1D::Texture1D(int w, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("Texture1D", GL_TEXTURE_1D)
{
    init(w, tf, f, t, params, s, pixels);
}

void Texture1D::init(int w, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage1D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage1D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture1D::~Texture1D()
{
}

int Texture1D::getWidth()
{
    return w;
}

void Texture1D::setSubImage(int level, int x, int w, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage1D(GL_TEXTURE_1D, level, x, w, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture1D::setCompressedSubImage(int level, int x, int w, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage1D(GL_TEXTURE_1D, level, x, w, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture1D::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<Texture1D>()->w);
}

/// @cond RESOURCES

class Texture1DResource : public ResourceTemplate<0, Texture1D>
{
public:
    Texture1DResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, Texture1D>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s;
        int w;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,width,height,");
            getIntParameter(desc, e, "width", &w);
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);
            s.compressedSize(desc->getSize());
            init(w, tf, f, t, params, s, CPUBuffer(desc->getData()));
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char texture1D[] = "texture1D";

static ResourceFactory::Type<texture1D, Texture1DResource> Texture1DType;

/// @endcond

}
