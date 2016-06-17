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

#ifndef _ORK_WINDOW_H_
#define _ORK_WINDOW_H_

#include <string>

#include "ork/math/vec2.h"
#include "ork/ui/EventHandler.h"

namespace ork
{

/**
 * @defgroup ui ui
 * @ingroup ork
 * Provides a minimal graphical user interface framework for Ork applications.
 */

/**
 * A window, displays the default FrameBuffer and receives user interface events.
 * @ingroup ui
 */
class ORK_API Window : public EventHandler
{
public:
    /**
     * A set of Window parameters.
     */
    class ORK_API Parameters {
    public:
        /**
         * Creates a new set of Window parameters.
         */
        Parameters();

        /**
         * Returns the window name.
         */
        std::string name() const;

        /**
         * Returns the window width.
         */
        int width() const;

        /**
         * Returns the window height.
         */
        int height() const;

        /**
         * Returns the OpenGL version to use.
         */
        vec2<int> version() const;

        /**
         * Returns true if a debug OpenGL context must be used.
         */
        bool debug() const;

        /**
         * Returns true if the default framebuffer must have an alpha channel.
         */
        bool alpha() const;

        /**
         * Returns true if the default framebuffer must have a depth buffer.
         */
        bool depth() const;

        /**
         * Returns true if the default framebuffer must have as stencil buffer.
         */
        bool stencil() const;

        /**
         * Returns true if the default framebuffer must use multisampling.
         */
        bool multiSample() const;

        /**
         * Sets the window name.
         *
         * @param name the window name.
         */
        Parameters &name(const std::string name);

        /**
         * Sets the window size.
         *
         * @param width the window width.
         * @param height the window height.
         */
        Parameters &size(int width, int height);

        /**
         * Sets the OpenGL context options.
         *
         * @param major the major version to use.
         * @param minor the minor version to use.
         * @param debug true to use a debug context.
         */
        Parameters &version(int major, int minor, bool debug = false);

        /**
         * Sets the alpha channel option for the default framebuffer.
         *
         * @param alpha true to use an alpha channel.
         */
        Parameters &alpha(bool alpha = true);

        /**
         * Sets the depth buffer option for the default framebuffer.
         *
         * @param depth true to use a depth buffer.
         */
        Parameters &depth(bool depth = true);

        /**
         * Sets the stencil buffer option for the default framebuffer.
         *
         * @param stencil true to use a stencil buffer.
         */
        Parameters &stencil(bool stencil = true);

        /**
         * Sets the multisampling option for the default framebuffer.
         *
         * @param multiSample true to use multisampling.
         */
        Parameters &multiSample(bool multiSample = true);

    private:
        std::string _name;

        int _width;

        int _height;

        int _major;

        int _minor;

        bool _debug;

        bool _alpha;

        bool _depth;

        bool _stencil;

        bool _multiSample;
    };

    /**
     * Creates a new window. If the window size is 0,0 a full screen window
     * is created.
     *
     * @param params the parameters of the window.
     */
    Window(const Parameters &params);

    /**
     * Deletes this window.
     */
    virtual ~Window();

    /**
     * Returns the width of the window.
     */
    virtual int getWidth() const = 0;

    /**
     * Returns the height of the window.
     */
    virtual int getHeight() const = 0;

    /**
     * Starts the user interface event processing loop.
     */
    virtual void start() = 0;
};

}

#endif
