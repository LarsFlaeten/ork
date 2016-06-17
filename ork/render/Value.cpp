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

#include "ork/render/Value.h"

using namespace std;

namespace ork
{

Value::Value(const char *type, const string &name) :
    Object(type), name(name)
{
}

Value::~Value()
{
}

string Value::getName() const
{
    return name;
}

const char value1f[] = "Value1f";

const char value1d[] = "Value1d";

const char value1i[] = "Value1i";

const char value1ui[] = "Value1ui";

const char value1b[] = "Value1b";

const char value2f[] = "Value2f";

const char value2d[] = "Value2d";

const char value2i[] = "Value2i";

const char value2ui[] = "Value2ui";

const char value2b[] = "Value2b";

const char value3f[] = "Value3f";

const char value3d[] = "Value3d";

const char value3i[] = "Value3i";

const char value3ui[] = "Value3ui";

const char value3b[] = "Value3b";

const char value4f[] = "Value4f";

const char value4d[] = "Value4d";

const char value4i[] = "Value4i";

const char value4ui[] = "Value4ui";

const char value4b[] = "Value4b";

const char valueMatrix2f[] = "valueMatrix2f";

const char valueMatrix3f[] = "valueMatrix3f";

const char valueMatrix4f[] = "valueMatrix4f";

const char valueMatrix2x3f[] = "valueMatrix2x3f";

const char valueMatrix2x4f[] = "valueMatrix2x4f";

const char valueMatrix3x2f[] = "valueMatrix3x2f";

const char valueMatrix3x4f[] = "valueMatrix3x4f";

const char valueMatrix4x2f[] = "valueMatrix4x2f";

const char valueMatrix4x3f[] = "valueMatrix4x3f";

const char valueMatrix2d[] = "valueMatrix2d";

const char valueMatrix3d[] = "valueMatrix3d";

const char valueMatrix4d[] = "valueMatrix4d";

const char valueMatrix2x3d[] = "valueMatrix2x3d";

const char valueMatrix2x4d[] = "valueMatrix2x4d";

const char valueMatrix3x2d[] = "valueMatrix3x2d";

const char valueMatrix3x4d[] = "valueMatrix3x4d";

const char valueMatrix4x2d[] = "valueMatrix4x2d";

const char valueMatrix4x3d[] = "valueMatrix4x3d";

ValueSampler::ValueSampler(UniformType type, const string &name) :
    Value("UniformSampler", name), type(type)
{
}

ValueSampler::ValueSampler(UniformType type, const string &name, ptr<Texture> value) :
    Value("UniformSampler", name), type(type), value(value)
{
}

ValueSampler::~ValueSampler()
{
}

UniformType ValueSampler::getType() const
{
    return type;
}

ptr<Texture> ValueSampler::get() const
{
    return value;
}

void ValueSampler::set(const ptr<Texture> value)
{
    this->value = value;
}

ValueSubroutine::ValueSubroutine(Stage stage, const string &name) :
    Value("UniformSubroutine", name), stage(stage)
{
}

ValueSubroutine::ValueSubroutine(Stage stage, const string &name, const string &value) :
    Value("UniformSubroutine", name), stage(stage), value(value)
{
}

ValueSubroutine::~ValueSubroutine()
{
}

UniformType ValueSubroutine::getType() const
{
    return SUBROUTINE;
}

Stage ValueSubroutine::getStage() const
{
    return stage;
}

string ValueSubroutine::get() const
{
    return value;
}

void ValueSubroutine::set(const string &value)
{
    this->value = value;
}

}
