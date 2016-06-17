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

#include "ork/render/Texture3D.h"

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

Texture3D::Texture3D() : Texture("Texture3D", GL_TEXTURE_3D)
{
}

Texture3D::Texture3D(int w, int h, int d, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("Texture3D", GL_TEXTURE_3D)
{
    init(w, h, d, tf, f, t, params, s, pixels);
}

void Texture3D::init(int w, int h, int d, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;
    this->h = h;
    this->d = d;

    bindToTextureUnit();

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage3D(GL_TEXTURE_3D, 0, getTextureInternalFormat(internalFormat), w, h, d, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage3D(GL_TEXTURE_3D, 0, getTextureInternalFormat(internalFormat), w, h, d, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture3D::~Texture3D()
{
}

int Texture3D::getWidth()
{
    return w;
}

int Texture3D::getHeight()
{
    return h;
}

int Texture3D::getDepth()
{
    return d;
}

void Texture3D::setSubImage(int level, int x, int y, int z, int w, int h, int d, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage3D(GL_TEXTURE_3D, level, x, y, z, w, h, d, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture3D::setCompressedSubImage(int level, int x, int y, int z, int w, int h, int d, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glTexSubImage3D(GL_TEXTURE_3D, level, x, y, z, w, h, d, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture3D::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<Texture3D>()->w);
    std::swap(h, t.cast<Texture3D>()->h);
    std::swap(d, t.cast<Texture3D>()->d);
}

/// @cond RESOURCES

class Texture3DResource : public ResourceTemplate<0, Texture3D>
{
public:
    Texture3DResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, Texture3D>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s;
        int w;
        int h;
        int d;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,wrapt,wrapr,width,height,depth,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &h);
            getIntParameter(desc, e, "depth", &d);
            if (h % d != 0) {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, e, "Inconsistent 'height' and 'depth' attributes");
                }
                throw exception();
            }
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);
            s.compressedSize(desc->getSize());
            init(w, h / d, d, tf, f, t, params, s, CPUBuffer(desc->getData()));
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char texture3D[] = "texture3D";

static ResourceFactory::Type<texture3D, Texture3DResource> Texture3DType;

/// @endcond

}
