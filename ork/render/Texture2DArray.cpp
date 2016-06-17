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

#include "ork/render/Texture2DArray.h"

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

Texture2DArray::Texture2DArray() : Texture("Texture2DArray", GL_TEXTURE_2D_ARRAY)
{
}

Texture2DArray::Texture2DArray(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("Texture2DArray", GL_TEXTURE_2D_ARRAY)
{
    init(w, h, l, tf, f, t, params, s, pixels);
}

void Texture2DArray::init(int w, int h, int l, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;
    this->h = h;
    this->l = l;
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage3D(GL_TEXTURE_2D_ARRAY, 0, getTextureInternalFormat(internalFormat), w, h, l, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, getTextureInternalFormat(internalFormat), w, h, l, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

Texture2DArray::~Texture2DArray()
{
}

int Texture2DArray::getWidth()
{
    return w;
}

int Texture2DArray::getHeight()
{
    return h;
}

int Texture2DArray::getLayers()
{
    return l;
}

void Texture2DArray::setSubImage(int level, int x, int y, int l, int w, int h, int d, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, x, y, l, w, h, d, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture2DArray::setCompressedSubImage(int level, int x, int y, int l, int w, int h, int d, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, x, y, l, w, h, d, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void Texture2DArray::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(l, t.cast<Texture2DArray>()->l);
}

/// @cond RESOURCES

class Texture2DArrayResource : public ResourceTemplate<0, Texture2DArray>
{
public:
    Texture2DArrayResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, Texture2DArray>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s;
        int w;
        int h;
        int l;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,wrapt,maxAniso,width,height,depth,layers,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &h);
            if (e->Attribute("depth") != NULL) {
                getIntParameter(desc, e, "depth", &l);
            } else {
                getIntParameter(desc, e, "layers", &l);
            }
            if (h % l != 0) {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, e, "Inconsistent 'height' and 'layers' attributes");
                }
                throw exception();
            }
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);
            s.compressedSize(desc->getSize());
            init(w, h / l, l, tf, f, t, params, s, CPUBuffer(desc->getData()));
            desc->clearData();
        } catch (...) {
            desc->clearData();
            throw exception();
        }
    }
};

extern const char texture2DArray[] = "texture2DArray";

static ResourceFactory::Type<texture2DArray, Texture2DArrayResource> Texture2DArrayType;

/// @endcond

}
