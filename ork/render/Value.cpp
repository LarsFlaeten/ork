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

#include "ork/render/Value.h"

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

}
