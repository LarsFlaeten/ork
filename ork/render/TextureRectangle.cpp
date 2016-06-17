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

#include "ork/render/TextureRectangle.h"

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

TextureRectangle::TextureRectangle() : Texture("TextureRectangle", GL_TEXTURE_RECTANGLE)
{
}

TextureRectangle::TextureRectangle(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels) : Texture("TextureRectangle", GL_TEXTURE_RECTANGLE)
{
    init(w, h, tf, f, t, params, s, pixels);
}

void TextureRectangle::init(int w, int h, TextureInternalFormat tf, TextureFormat f, PixelType t,
    const Parameters &params, const Buffer::Parameters &s, const Buffer &pixels)
{
    Texture::init(tf, params);
    this->w = w;
    this->h = h;

    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    bool needToGenerateMipmaps = true;
    if (isCompressed() && s.compressedSize() > 0) {
        glCompressedTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, h, 0, s.compressedSize(), pixels.data(0));
    } else {
        s.set();
        glTexImage2D(textureTarget, 0, getTextureInternalFormat(internalFormat), w, h, 0, getTextureFormat(f), getPixelType(t), pixels.data(0));
        s.unset();

        GLsizei size = s.compressedSize(); // should work because size is retrieved from file descriptor.
        int pixelSize = getFormatSize(f, t);
        if (size > w * h * pixelSize) {
            // get the other levels from the same buffer
            int offset = w * h * pixelSize;
            int level = 0;
            int wl = w;
            int hl = h;
            while (wl % 2 == 0 && hl % 2 == 0 && size - offset >= (wl * hl / 4) * pixelSize) {
                level += 1;
                wl = wl / 2;
                hl = hl / 2;
                glTexImage2D(textureTarget, level, getTextureInternalFormat(internalFormat), wl, hl, 0, getTextureFormat(f), getPixelType(t), pixels.data(offset));
                offset += wl * hl * pixelSize;
                needToGenerateMipmaps = false;
            }
            this->params.lodMax(clamp(params.lodMax(), GLfloat(0.0f), GLfloat(level)));
        }
    }
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);

    if (needToGenerateMipmaps) {
        generateMipMap();
    }

    if (FrameBuffer::getError() != 0) {
        throw exception();
    }
}

TextureRectangle::~TextureRectangle()
{
}

int TextureRectangle::getWidth()
{
    return w;
}

int TextureRectangle::getHeight()
{
    return h;
}

void TextureRectangle::setSubImage(int level, int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    s.set();
    glTexSubImage2D(GL_TEXTURE_RECTANGLE, level, x, y, w, h, getTextureFormat(f), getPixelType(t), pixels.data(0));
    s.unset();
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void TextureRectangle::setCompressedSubImage(int level, int x, int y, int w, int h, int s, const Buffer &pixels)
{
    bindToTextureUnit();
    pixels.bind(GL_PIXEL_UNPACK_BUFFER);
    glCompressedTexSubImage2D(GL_TEXTURE_RECTANGLE, level, x, y, w, h, getTextureInternalFormat(internalFormat), s, pixels.data(0));
    pixels.unbind(GL_PIXEL_UNPACK_BUFFER);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

/// @cond RESOURCES

class TextureRectangleResource : public ResourceTemplate<0, TextureRectangle>
{
public:
    TextureRectangleResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<0, TextureRectangle>(manager, name, desc)
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

extern const char textureRectangle[] = "textureRectangle";

static ResourceFactory::Type<textureRectangle, TextureRectangleResource> TextureRectangleType;

/// @endcond

}
