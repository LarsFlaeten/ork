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

#include "ork/ui/Window.h"

namespace ork
{

Window::Parameters::Parameters() :
    _name("Untitled"), _width(640), _height(480), _major(3), _minor(3),
    _debug(false), _alpha(false), _depth(false), _stencil(false), _multiSample(false)
{
}

string Window::Parameters::name() const
{
    return _name;
}

int Window::Parameters::width() const
{
    return _width;
}

int Window::Parameters::height() const
{
    return _height;
}

vec2<int> Window::Parameters::version() const
{
    return vec2<int>(_major, _minor);
}

bool Window::Parameters::debug() const
{
    return _debug;
}

bool Window::Parameters::alpha() const
{
    return _alpha;
}

bool Window::Parameters::depth() const
{
    return _depth;
}

bool Window::Parameters::stencil() const
{
    return _stencil;
}

bool Window::Parameters::multiSample() const
{
    return _multiSample;
}

Window::Parameters &Window::Parameters::name(const string name)
{
    _name = name;
    return *this;
}

Window::Parameters &Window::Parameters::size(int width, int height)
{
    _width = width;
    _height = height;
    return *this;
}

Window::Parameters &Window::Parameters::version(int major, int minor, bool debug)
{
    _major = major;
    _minor = minor;
    _debug = debug;
    return *this;
}

Window::Parameters &Window::Parameters::alpha(bool alpha)
{
    _alpha = alpha;
    return *this;
}

Window::Parameters &Window::Parameters::depth(bool depth)
{
    _depth = depth;
    return *this;
}

Window::Parameters &Window::Parameters::stencil(bool stencil)
{
    _stencil = stencil;
    return *this;
}

Window::Parameters &Window::Parameters::multiSample(bool multiSample)
{
    _multiSample = multiSample;
    return *this;
}

Window::Window(const Parameters &params) : EventHandler("Window")
{
}

Window::~Window()
{
}

}
