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

#include "ork/ui/Window.h"

using namespace std;

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
