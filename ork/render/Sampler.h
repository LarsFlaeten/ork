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

#ifndef _ORK_SAMPLER_H_
#define _ORK_SAMPLER_H_

#include <vector>
#include <map>

#include "ork/core/Object.h"
#include "ork/render/Types.h"

// Visual Studio need this or there is a conflict between with a min macro
// defined somewhere and the min() getter
#undef min

namespace ork
{

/**
 * A texture sampler. A texture sampler can be used in a UniformSampler to
 * change the default sampling parameters of a texture.
 *
 * @ingroup render
 */
class ORK_API Sampler : public Object
{
public:
    /**
     * A set of texture sampling parameters.
     */
    class ORK_API Parameters
    {
    public:
        /**
         * Creates a new set of texture sampling parameters with default values.
         */
        Parameters();

        /**
         * Deletes these texture sampling parameters.
         */
        virtual ~Parameters();

        /**
         * Returns the wrap parameter for texture coordinate s. Determines the behavior of the sampler
         * when sampling out of the borders of the texture.
         */
        TextureWrap wrapS() const;

        /**
         * Returns the wrap parameter for texture coordinate t. Determines the behavior of the sampler
         * when sampling out of the borders of the texture.
         */
        TextureWrap wrapT() const;

        /**
         * Returns the wrap parameter for texture coordinate r (only for 3D textures or arrays). Determines
         * the behavior of the sampler when sampling out of the borders of the texture.
         */
        TextureWrap wrapR() const;

        /**
         * Returns the minifying function used whenever the pixel being textured maps to an area
         * greated than one texture element.
         */
        TextureFilter min() const;

        /**
         * Returns the magnifying function used whenever the pixel being textured maps to an area
         * less than or equal to one texture element.
         */
        TextureFilter mag() const;

        /**
         * Returns the type of data used to set the border color.
         * Can be int, float, Iint, or unsigned int.
         */
        unsigned int borderType() const;

        /**
         * Returns the border color as integers.
         */
        const GLint* borderi() const;

        /**
         * Returns the border color as floats.
         */
        const GLfloat* borderf() const;

        /**
         * Returns the border color as Iintegers.
         */
        const GLint* borderIi() const;

        /**
         * Returns the border color as unsigned integers.
         */
        const GLuint* borderIui() const;

        /**
         * Returns the minimum level of detail used in this sampler.
         */
        GLfloat lodMin() const;

        /**
         * Returns the maximum level of detail used in this sampler.
         */
        GLfloat lodMax() const;

        /**
         * Returns the bias applied to the level of details. The bias is the starting value
         * when looking inside a texture, and will be added to the computed level displayed.
         */
        GLfloat lodBias() const;

        /**
         * Returns the comparison operator used for depth tests.
         */
        Function compareFunc() const;

        /**
         * Returns the anisotropic maximum value parameter.
         */
        GLfloat maxAnisotropyEXT() const;

        /**
         * Sets the wrap parameter for texture coordinate s. Determines the behavior of the sampler
         * when sampling out of the borders of the texture.
         *
         * @param wrapS the wrap parameter for coordinate s.
         */
        virtual Parameters& wrapS(TextureWrap wrapS);

        /**
         * Sets the wrap parameter for texture coordinate t. Determines the behavior of the sampler
         * when sampling out of the borders of the texture.
         *
         * @param wrapT the wrap parameter for coordinate t.
         */
        virtual Parameters& wrapT(TextureWrap wrapT);

        /**
         * Sets the wrap parameter for texture coordinate r. Determines the behavior of the sampler
         * when sampling out of the borders of the texture.
         *
         * @param wrapR the wrap parameter for coordinate r.
         */
        virtual Parameters& wrapR(TextureWrap wrapR);

        /**
         * Sets the minifying function used whenever the pixel being textured maps to an area
         * greater than one texture element.
         *
         * @param min the minifying function.
         */
        virtual Parameters& min(TextureFilter min);

        /**
         * Sets the magnifying function used whenever the pixel being textured maps to an area
         * less than or equal to one texture element.
         *
         * @param mag the magnifying function.
         */
        virtual Parameters& mag(TextureFilter mag);

        /**
         * Sets the border color as integers.
         *
         * @param r red component.
         * @param g green component.
         * @param b blue component.
         * @param a alpha component.
         */
        virtual Parameters& borderi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

        /**
         * Sets the border color as floats.
         *
         * @param r red component.
         * @param g green component.
         * @param b blue component.
         * @param a alpha component.
         */
        virtual Parameters& borderf(GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 0.0f);

        /**
         * Sets the border color as Lintegers.
         *
         * @param r red component.
         * @param g green component.
         * @param b blue component.
         * @param a alpha component.
         */
        virtual Parameters& borderIi(GLint r = 0, GLint g = 0, GLint b = 0, GLint a = 0);

        /**
         * Sets the border color as unsigned integers.
         *
         * @param r red component.
         * @param g green component.
         * @param b blue component.
         * @param a alpha component.
         */
        virtual Parameters& borderIui(GLuint r = 0, GLuint g = 0, GLuint b = 0, GLuint a = 0);

        /**
         * Sets the minimum level of detail used in this sampler.
         *
         * @param lodMin the minimum level of detail.
         */
        virtual Parameters& lodMin(GLfloat lodMin);

        /**
         * Sets the maximum level of detail used in this sampler.
         *
         * @param lodMax the maximum level of detail.
         */
        virtual Parameters& lodMax(GLfloat lodMax);

        /**
         * Sets the bias applied to the level of details. The bias is the starting value
         * when looking inside a texture, and will be added to the computed level displayed.
         *
         * @param lodBias the bias applied to every lod access.
         */
        virtual Parameters& lodBias(GLfloat lodBias);

        /**
         * Sets the comparison operator used for depth tests.
         *
         * @param compareFunc the comparison operator.
         */
        virtual Parameters& compareFunc(Function compareFunc);

        /**
         * Sets the anisotropic maximum value parameter.
         *
         * @param maxAnisotropy the anisotropy maximum value.
         */
        virtual Parameters& maxAnisotropyEXT(GLfloat maxAnisotropy);

        /**
         * Less Operator for Sampler::Parameters. Mostly here to determine if the operators are
         * not the same.
         */
        virtual bool operator<(const Parameters &v) const;

    private:
        TextureWrap _wrapS;

        TextureWrap _wrapT;

        TextureWrap _wrapR;

        TextureFilter _min;

        TextureFilter _mag;

        union {
            GLint i[4];
            GLuint ui[4];
            GLfloat f[4];
        } _border;

        unsigned int _borderType;

        GLfloat _lodMin;

        GLfloat _lodMax;

        GLfloat _lodBias;

        GLfloat _maxAnisotropy;

        Function _compareFunc;
    };

    /**
     * Creates a new sampler with the given parameters.
     *
     * @param value the sampler parameters.
     */
    Sampler(const Parameters &value);

    /**
     * Deletes this sampler.
     */
    ~Sampler();

    /**
     * Returns the id of this sampler.
     */
    GLuint getId() const;

private:
    /**
     * The id of this sampler.
     */
    GLuint samplerId;

    /**
     * The parameters of this sampler.
     */
    const Parameters params;

    /**
     * The shared texture sampler instances. All the sampler objects with
     * the same options are represented with the same OpenGL instance. This
     * map associates the shared sampler id and the corresponding number of
     * Sampler object to each possible value for the sampler parameters.
     */
    static std::map<Sampler::Parameters, std::pair<GLuint, unsigned int> > INSTANCES;
};

}

#endif
