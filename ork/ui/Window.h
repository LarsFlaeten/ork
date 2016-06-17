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
        string name() const;

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
        Parameters &name(const string name);

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
        string _name;

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
