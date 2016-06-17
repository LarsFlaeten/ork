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

#include "ork/render/TextureCube.h"

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

GLenum getCubeFace(CubeFace f);

GLenum getTextureInternalFormat(TextureInternalFormat f);

GLenum getTextureFormat(TextureFormat f);

GLenum getPixelType(PixelType t);

unsigned int getFormatSize(TextureFormat f, PixelType t);

TextureCube::TextureCube() : Texture("TextureCube", GL_TEXTURE_CUBE_MAP)
{
}

TextureCube::TextureCube(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, Buffer::Parameters s[6], ptr<Buffer> pixels[6]) : Texture("TextureCube", GL_TEXTURE_CUBE_MAP)
{
    init(w, h, tf, f, t, params, s, pixels);
}

void TextureCube::init(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, Buffer::Parameters s[6], ptr<Buffer> pixels[6])
{
    Texture::init(tf, params);
    this->w = w;
    this->h = h;

    const GLenum FACES[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    if (isCompressed()) {
        for (int i = 0; i < 6; ++i) {
            pixels[i]->bind(GL_PIXEL_UNPACK_BUFFER);
            if (s[i].compressedSize() > 0) {
                glCompressedTexImage2D(FACES[i], 0, getTextureInternalFormat(internalFormat), w, h, 0, s[i].compressedSize(), pixels[i]->data(0));
            } else {
                s[i].set();
                glTexImage2D(FACES[i], 0, getTextureInternalFormat(internalFormat), w, h, 0, getTextureFormat(f), getPixelType(t), pixels[i]->data(0));
                s[i].unset();
            }
            pixels[i]->unbind(GL_PIXEL_UNPACK_BUFFER);
        }
    } else {
        for (int i = 0; i < 6; ++i) {
            pixels[i]->bind(GL_PIXEL_UNPACK_BUFFER);
            s[i].set();
            glTexImage2D(FACES[i], 0, getTextureInternalFormat(internalFormat), w, h, 0, getTextureFormat(f), getPixelType(t), pixels[i]->data(0));
            s[i].unset();
            pixels[i]->unbind(GL_PIXEL_UNPACK_BUFFER);
        }
    }

    generateMipMap();

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

TextureCube::~TextureCube()
{
}

int TextureCube::getWidth()
{
    return w;
}

int TextureCube::getHeight()
{
    return h;
}

void TextureCube::setSubImage(CubeFace cf, int level, int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage2D(getCubeFace(cf), level, x, y, w, h, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void TextureCube::setCompressedSubImage(CubeFace cf, int level, int x, int y, int w, int h, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage2D(getCubeFace(cf), level, x, y, w, h, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void TextureCube::swap(ptr<Texture> t)
{
    Texture::swap(t);
    std::swap(w, t.cast<TextureCube>()->w);
    std::swap(h, t.cast<TextureCube>()->h);
}

/// @cond RESOURCES

class TextureCubeResource : public ResourceTemplate<0, TextureCube>
{
public:
    TextureCubeResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, TextureCube>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        TextureInternalFormat tf;
        TextureFormat f;
        PixelType t;
        Texture::Parameters params;
        Buffer::Parameters s[6];
        int w;
        int h;
        try {
            checkParameters(desc, e, "name,source,internalformat,format,type,min,mag,wraps,wrapt,width,height,");
            getIntParameter(desc, e, "width", &w);
            getIntParameter(desc, e, "height", &h);
            if (h != 6 * w) {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, e, "Inconsistent 'width' and 'height' attributes");
                }
                throw exception();
            }
            getParameters(desc, e, tf, f, t);
            getParameters(desc, e, params);

            int bpp = getFormatSize(f, t);

            ptr<Buffer> pixels[6];
            for (int i = 0; i < 6; ++i) {
                pixels[i] = new CPUBuffer(desc->getData() + i * w * w * bpp);
            }

            try {
                init(w, w, tf, f, t, params, s, pixels);
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

extern const char textureCube[] = "textureCube";

static ResourceFactory::Type<textureCube, TextureCubeResource> TextureCubeType;

/// @endcond

}
