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

#ifndef _ORK_TEXTURE_H_
#define _ORK_TEXTURE_H_

#include <vector>

#include "ork/render/Sampler.h"

using namespace std;

namespace ork
{

// conservative estimation of the true maximum number of texture units,
// used to allocate static arrays for storing texture unit states
#define MAX_TEXTURE_UNITS 64

/**
 * An abstract texture.
 * @ingroup render
 */
class ORK_API Texture : public Object
{
public:
    /**
     * A set of texture parameters.
     */
    class ORK_API Parameters : public Sampler::Parameters
    {
    public:
        /**
         * Creates a new set of texture parameters with default values.
         */
        Parameters();

        /**
         * Deletes this set of texture parameters.
         */
        virtual ~Parameters();

        TextureWrap wrapS() const;

        TextureWrap wrapT() const;

        TextureWrap wrapR() const;

        TextureFilter min() const;

        TextureFilter mag() const;

        unsigned int borderType() const;

        const GLint* borderi() const;

        const GLfloat* borderf() const;

        const GLint* borderIi() const;

        const GLuint* borderIui() const;

        GLfloat lodMin() const;

        GLfloat lodMax() const;

        GLfloat lodBias() const;

        Function compareFunc() const;

        GLfloat maxAnisotropyEXT() const;

        /**
         * Returns the current texture swizzling order.
         */
        const char* swizzle() const;

        /**
         * Returns the index of the lowest defined mipmap level.
         */
        GLint minLevel() const;

        /**
         * Returns the index of the highest defined mipmap level.
         */
        GLint maxLevel() const;

        virtual Parameters& wrapS(TextureWrap wrapS);

        virtual Parameters& wrapT(TextureWrap wrapT);

        virtual Parameters& wrapR(TextureWrap wrapR);

        virtual Parameters& min(TextureFilter min);

        virtual Parameters& mag(TextureFilter mag);

        virtual Parameters& borderi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

        virtual Parameters& borderf(GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 0.0f);

        virtual Parameters& borderIi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

        virtual Parameters& borderIui(GLuint r = 0, GLuint g = 0, GLuint b = 0, GLuint a = 0);

        virtual Parameters& lodMin(GLfloat lodMin);

        virtual Parameters& lodMax(GLfloat lodMax);

        virtual Parameters& lodBias(GLfloat lodBias);

        virtual Parameters& compareFunc(Function compareFunc);

        virtual Parameters& maxAnisotropyEXT(GLfloat maxAnisotropy);

        /**
         * Sets the swizzling order for this Texture.
         * Enables the user to reorder the components of a texture before it is sent to the GPU.
         * Each parameter can be either 'r', 'g', 'b', or 'a'.
         *
         * @param r new component for red channel.
         * @param g new component for green channel.
         * @param b new component for blue channel.
         * @param a new component for alpha channel.
         */
        virtual Parameters& swizzle(char r, char g, char b, char a);

        /**
         * Sets the index of the lowest defined mipmap level.
         */
        virtual Parameters& minLevel(GLint minLevel);

        /**
         * Sets the index of the highest defined mipmap level.
         */
        virtual Parameters& maxLevel(GLint maxLevel);

    private:
        char _swizzle[4];

        GLint _minLevel;

        GLint _maxLevel;

        friend class Texture;
    };

    /**
     * Destroys this texture.
     */
    virtual ~Texture();

    /**
     * Returns the identifier of this texture.
     */
    GLuint getId() const;

    /**
     * Returns the number of components in the texture's internal format.
     */
    unsigned int getComponents() const;

    /**
     * Returns the name of the texture's internal format.
     */
    const char *getInternalFormatName() const;

    /**
     * Returns the internal format of this texture.
     */
    TextureInternalFormat getInternalFormat() const;

    /**
     * Returns a Format compatible with the internal format of this Texture.
     */
    TextureFormat getFormat() const;

    /**
     * True if this texture has any mipmap level above 0.
     * False if this texture has only a base level.
     */
    bool hasMipmaps() const;

    /**
     * Returns true if this texture is in a compressed format on GPU.
     */
    bool isCompressed() const;

    /**
     * Returns the size of the compressed data of this texture. Must be used
     * only for a compressed texture (see #isCompressed).
     */
    GLsizei getCompressedSize(int level) const;

    /**
     * Returns the texture pixels in the specified format.
     *
     * @param level the texture LOD level to be read.
     * @param f the format in which data must be returned.
     * @param t the type in which pixel components must be returned.
     * @param[out] pixels the returned data.
     */
    void getImage(int level, TextureFormat f, PixelType t, void *pixels);

    /**
     * Returns the compressed data of this texture. Must be used only for a
     * compressed texture (see #isCompressed).
     *
     * @param level the texture LOD level to be read.
     * @param[out] pixels the returned compressed data.
     */
    void getCompressedImage(int level, void *pixels) const;

    /**
     * Generates the mipmap levels for this texture.
     */
    void generateMipMap();

protected:
    /**
     * Creates a new unitialized texture.
     *
     * @param type the texture class name.
     * @param target a texture type (1D, 2D, 3D, etc).
     */
    Texture(const char *type, int target);

    /**
     * Initializes this texture.
     *
     * @param tf texture data format on GPU.
     * @param params the texture parameters.
     */
    void init(TextureInternalFormat tf, const Parameters &params);

    /**
     * Binds this texture and the given sampler to a texture unit, for the
     * given program. If there is a texture unit to which no texture is
     * currently bound the texture is bound to this unit. Otherwise the
     * least recently used texture unit that is not used by the given program
     * is unbound, and this texture is bound instead.
     *
     * @param s a sampler object to sample this texture. May be NULL.
     * @param programId the program for which this texture must be bound.
     * @return the texture unit to which the texture has been bound, or -1
     *      if the binding failed (all units already used by the program).
     */
    GLint bindToTextureUnit(ptr<Sampler> s, GLuint programId) const;

    /**
     * Binds this texture to a texture unit. If there is a texture unit
     * to which no texture is currently bound the texture is bound to this
     * unit. Otherwise the least recently used texture unit is unbound, and
     * this texture is bound instead.
     *
     * @return the texture unit to which the texture has been bound.
     */
    GLint bindToTextureUnit() const;

    /**
     * Swaps this texture with the given one.
     */
    virtual void swap(ptr<Texture> t);

private:
    /**
     * The OpenGL texture identifier (as returned by glGenTextures).
     */
    GLuint textureId;

    /**
     * The OpenGL texture target type of this texture (GL_TEXTURE_1D, GL_TEXTURE_2D, etc...).
     */
    GLuint textureTarget;

    /**
     * The OpenGL texture internal format (GL_RGB8, GL_RGBA8, etc).
     */
    TextureInternalFormat internalFormat;

    /**
     * The texture parameters.
     */
    Parameters params;

    /**
     * The OpenGL texture units where this texture is currently bound.
     * There is one possible binding per sampler object (a texture can be
     * bound to several units with different sampler objects).
     */
    mutable map<GLuint, GLuint> currentTextureUnits;

    /**
     * Identifiers of the programs that use this texture.
     */
    mutable vector<GLuint> programIds;

    /**
     * Adds the given program as a user of this texture.
     */
    void addUser(GLuint programId) const;

    /**
     * Removes the given program as a user of this texture.
     */
    void removeUser(GLuint programId) const;

    /**
     * Returns true if the given program uses this texture.
     */
    bool isUsedBy(GLuint programId) const;

    /**
     * Returns the actual maximum number of texture units.
     */
    static unsigned int getMaxTextureUnits();

    /**
     * Unbinds the texture units using the given Sampler.
     */
    static void unbindSampler(Sampler *sampler);

    /**
     * Unbinds all the texture units.
     */
    static void unbindAll();

    friend class Sampler;

    friend class Texture1D;

    friend class Texture1DArray;

    friend class Texture2D;

    friend class Texture2DArray;

    friend class Texture2DMultisample;

    friend class Texture2DMultisampleArray;

    friend class Texture3D;

    friend class TextureCube;

    friend class TextureCubeArray;

    friend class TextureRectangle;

    friend class TextureBuffer;

    friend class TextureUnit;

    friend class TextureUnitManager;

    friend class Program;

    friend class UniformSampler;

    friend class FrameBuffer;

};

}

#endif
