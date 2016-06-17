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

#include "ork/render/Types.h"

#include <cassert>
#include <exception>

#include <GL/glew.h>

using namespace std;

namespace ork
{

GLenum getBufferUsage(BufferUsage u)
{
    switch (u) {
    case STREAM_DRAW:
        return GL_STREAM_DRAW;
    case STREAM_READ:
        return GL_STREAM_READ;
    case STREAM_COPY:
        return GL_STREAM_COPY;
    case STATIC_DRAW:
        return GL_STATIC_DRAW;
    case STATIC_READ:
        return GL_STATIC_READ;
    case STATIC_COPY:
        return GL_STATIC_COPY;
    case DYNAMIC_DRAW:
        return GL_DYNAMIC_DRAW;
    case DYNAMIC_READ:
        return GL_DYNAMIC_READ;
    case DYNAMIC_COPY:
        return GL_DYNAMIC_COPY;
    }
    assert(false);
    throw exception();
}

GLenum getBufferAccess(BufferAccess a)
{
    switch (a) {
    case READ_ONLY:
        return GL_READ_ONLY;
    case WRITE_ONLY:
        return GL_WRITE_ONLY;
        break;
    case READ_WRITE:
        return GL_READ_WRITE;
    }
    assert(false);
    throw exception();
}

GLenum getAttributeType(AttributeType t)
{
    switch (t) {
    case A8I:
        return GL_BYTE;
    case A8UI:
        return GL_UNSIGNED_BYTE;
    case A16I:
        return GL_SHORT;
    case A16UI:
        return GL_UNSIGNED_SHORT;
    case A32I:
        return GL_INT;
    case A32UI:
        return GL_UNSIGNED_INT;
    case A16F:
        return GL_HALF_FLOAT;
    case A32F:
        return GL_FLOAT;
    case A64F:
        return GL_DOUBLE;
    case A32I_2_10_10_10_REV:
        return GL_INT_2_10_10_10_REV;
    case A32UI_2_10_10_10_REV:
        return GL_UNSIGNED_INT_2_10_10_10_REV;
    case A32I_FIXED:
        return GL_FIXED;
    }
    assert(false);
    throw exception();
}

GLenum getMeshMode(MeshMode m)
{
    switch (m) {
    case POINTS:
        return GL_POINTS;
    case LINE_STRIP:
        return GL_LINE_STRIP;
    case LINE_LOOP:
        return GL_LINE_LOOP;
    case LINES:
        return GL_LINES;
    case TRIANGLE_STRIP:
        return GL_TRIANGLE_STRIP;
    case TRIANGLE_FAN:
        return GL_TRIANGLE_FAN;
    case TRIANGLES:
        return GL_TRIANGLES;
    case LINES_ADJACENCY:
        return GL_LINES_ADJACENCY;
    case LINE_STRIP_ADJACENCY:
        return GL_LINE_STRIP_ADJACENCY;
    case TRIANGLES_ADJACENCY:
        return GL_TRIANGLES_ADJACENCY;
    case TRIANGLE_STRIP_ADJACENCY:
        return GL_TRIANGLE_STRIP_ADJACENCY;
    case PATCHES:
        return GL_PATCHES;
    }
    assert(false);
    throw exception();
}

GLenum getCubeFace(CubeFace f)
{
    switch (f) {
    case POSITIVE_X:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    case NEGATIVE_X:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    case POSITIVE_Y:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    case NEGATIVE_Y:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    case POSITIVE_Z:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    case NEGATIVE_Z:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    }
    assert(false);
    throw exception();
}

const char *getTextureInternalFormatName(TextureInternalFormat f)
{
    switch (f) {
    case R8:
        return "R8";
    case R8_SNORM:
        return "R8_SNORM";
    case R16:
        return "R16";
    case R16_SNORM:
        return "R16_SNORM";
    case RG8:
        return "RG8";
    case RG8_SNORM:
        return "RG8_SNORM";
    case RG16:
        return "RG16";
    case RG16_SNORM:
        return "RG16_SNORM";
    case R3_G3_B2:
        return "R3_G3_B2";
    case RGB4:
        return "RGB4";
    case RGB5:
        return "RGB5";
    case RGB8:
        return "RGB8";
    case RGB8_SNORM:
        return "RGB8_SNORM";
    case RGB10:
        return "RGB10";
    case RGB12:
        return "RGB12";
    case RGB16:
        return "RGB16";
    case RGB16_SNORM:
        return "RGB16_SNORM";
    case RGBA2:
        return "RGBA2";
    case RGBA4:
        return "RGBA4";
    case RGB5_A1:
        return "RGB5_A1";
    case RGBA8:
        return "RGBA8";
    case RGBA8_SNORM:
        return "RGBA8_SNORM";
    case RGB10_A2:
        return "RGB10_A2";
    case RGB10_A2UI:
        return "RGB10_A2UI";
    case RGBA12:
        return "RGBA12";
    case RGBA16:
        return "RGBA16";
    case RGBA16_SNORM:
        return "RGBA16_SNORM";
    case SRGB8:
        return "SRGB8";
    case SRGB8_ALPHA8:
        return "SRGB8_ALPHA8";
    case R16F:
        return "R16F";
    case RG16F:
        return "RG16F";
    case RGB16F:
        return "RGB16F";
    case RGBA16F:
        return "RGBA16F";
    case R32F:
        return "R32F";
    case RG32F:
        return "RG32F";
    case RGB32F:
        return "RGB32F";
    case RGBA32F:
        return "RGBA32F";
    case R11F_G11F_B10F:
        return "R11F_G11F_B10F";
    case RGB9_E5:
        return "RGB9_E5";
    case R8I:
        return "R8I";
    case R8UI:
        return "R8UI";
    case R16I:
        return "R16I";
    case R16UI:
        return "R16UI";
    case R32I:
        return "R32I";
    case R32UI:
        return "R32UI";
    case RG8I:
        return "RG8I";
    case RG8UI:
        return "RG8UI";
    case RG16I:
        return "RG16I";
    case RG16UI:
        return "RG16UI";
    case RG32I:
        return "RG32I";
    case RG32UI:
        return "RG32UI";
    case RGB8I:
        return "RGB8I";
    case RGB8UI:
        return "RGB8UI";
    case RGB16I:
        return "RGB16I";
    case RGB16UI:
        return "RGB16UI";
    case RGB32I:
        return "RGB32I";
    case RGB32UI:
        return "RGB32UI";
    case RGBA8I:
        return "RGBA8I";
    case RGBA8UI:
        return "RGBA8UI";
    case RGBA16I:
        return "RGBA16I";
    case RGBA16UI:
        return "RGBA16UI";
    case RGBA32I:
        return "RGBA32I";
    case RGBA32UI:
        return "RGBA32UI";
    case DEPTH_COMPONENT16:
        return "DEPTH_COMPONENT16";
    case DEPTH_COMPONENT24:
        return "DEPTH_COMPONENT24";
    case DEPTH_COMPONENT32F:
        return "DEPTH_COMPONENT32F";
    case DEPTH24_STENCIL8:
        return "DEPTH24_STENCIL8";
    case DEPTH32F_STENCIL8:
        return "DEPTH32F_STENCIL8";
    case COMPRESSED_RED:
        return "COMPRESSED_RED";
    case COMPRESSED_RG:
        return "COMPRESSED_RG";
    case COMPRESSED_RGB:
        return "COMPRESSED_RGB";
    case COMPRESSED_RGBA:
        return "COMPRESSED_RGBA";
    case COMPRESSED_SRGB:
        return "COMPRESSED_SRGB";
    case COMPRESSED_RED_RGTC1:
        return "COMPRESSED_RED_RGTC1";
    case COMPRESSED_SIGNED_RED_RGTC1:
        return "COMPRESSED_SIGNED_RED_RGTC1";
    case COMPRESSED_RG_RGTC2:
        return "COMPRESSED_RG_RGTC2";
    case COMPRESSED_SIGNED_RG_RGTC2:
        return "COMPRESSED_SIGNED_RG_RGTC2";
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
        return "COMPRESSED_RGBA_BPTC_UNORM_ARB";
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
        return "COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB";
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
        return "COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB";
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
        return "COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB";
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
        return "COMPRESSED_RGB_S3TC_DXT1_EXT";
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
        return "COMPRESSED_RGBA_S3TC_DXT1_EXT";
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
        return "COMPRESSED_RGBA_S3TC_DXT3_EXT";
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return "COMPRESSED_RGBA_S3TC_DXT5_EXT";
    }
    assert(false);
    throw exception();
}

GLenum getTextureInternalFormat(TextureInternalFormat f)
{
    switch (f) {
    case R8:
        return GL_R8;
    case R8_SNORM:
        return GL_R8_SNORM;
    case R16:
        return GL_R16;
    case R16_SNORM:
        return GL_R16_SNORM;
    case RG8:
        return GL_RG8;
    case RG8_SNORM:
        return GL_RG8_SNORM;
    case RG16:
        return GL_RG16;
    case RG16_SNORM:
        return GL_RG16_SNORM;
    case R3_G3_B2:
        return GL_R3_G3_B2;
    case RGB4:
        return GL_RGB4;
    case RGB5:
        return GL_RGB5;
    case RGB8:
        return GL_RGB8;
    case RGB8_SNORM:
        return GL_RGB8_SNORM;
    case RGB10:
        return GL_RGB10;
    case RGB12:
        return GL_RGB12;
    case RGB16:
        return GL_RGB16;
    case RGB16_SNORM:
        return GL_RGB16_SNORM;
    case RGBA2:
        return GL_RGBA2;
    case RGBA4:
        return GL_RGBA4;
    case RGB5_A1:
        return GL_RGB5_A1;
    case RGBA8:
        return GL_RGBA8;
    case RGBA8_SNORM:
        return GL_RGBA8_SNORM;
    case RGB10_A2:
        return GL_RGB10_A2;
    case RGB10_A2UI:
        return GL_RGB10_A2UI;
    case RGBA12:
        return GL_RGBA12;
    case RGBA16:
        return GL_RGBA16;
    case RGBA16_SNORM:
        return GL_RGBA16_SNORM;
    case SRGB8:
        return GL_SRGB8;
    case SRGB8_ALPHA8:
        return GL_SRGB8_ALPHA8;
    case R16F:
        return GL_R16F;
    case RG16F:
        return GL_RG16F;
    case RGB16F:
        return GL_RGB16F;
    case RGBA16F:
        return GL_RGBA16F;
    case R32F:
        return GL_R32F;
    case RG32F:
        return GL_RG32F;
    case RGB32F:
        return GL_RGB32F;
    case RGBA32F:
        return GL_RGBA32F;
    case R11F_G11F_B10F:
        return GL_R11F_G11F_B10F;
    case RGB9_E5:
        return GL_RGB9_E5;
    case R8I:
        return GL_R8I;
    case R8UI:
        return GL_R8UI;
    case R16I:
        return GL_R16I;
    case R16UI:
        return GL_R16UI;
    case R32I:
        return GL_R32I;
    case R32UI:
        return GL_R32UI;
    case RG8I:
        return GL_RG8I;
    case RG8UI:
        return GL_RG8UI;
    case RG16I:
        return GL_RG16I;
    case RG16UI:
        return GL_RG16UI;
    case RG32I:
        return GL_RG32I;
    case RG32UI:
        return GL_RG32UI;
    case RGB8I:
        return GL_RGB8I;
    case RGB8UI:
        return GL_RGB8UI;
    case RGB16I:
        return GL_RGB16I;
    case RGB16UI:
        return GL_RGB16UI;
    case RGB32I:
        return GL_RGB32I;
    case RGB32UI:
        return GL_RGB32UI;
    case RGBA8I:
        return GL_RGBA8I;
    case RGBA8UI:
        return GL_RGBA8UI;
    case RGBA16I:
        return GL_RGBA16I;
    case RGBA16UI:
        return GL_RGBA16UI;
    case RGBA32I:
        return GL_RGBA32I;
    case RGBA32UI:
        return GL_RGBA32UI;
    case DEPTH_COMPONENT16:
        return GL_DEPTH_COMPONENT16;
    case DEPTH_COMPONENT24:
        return GL_DEPTH_COMPONENT24;
    case DEPTH_COMPONENT32F:
        return GL_DEPTH_COMPONENT32F;
    case DEPTH32F_STENCIL8:
        return GL_DEPTH32F_STENCIL8;
    case DEPTH24_STENCIL8:
        return GL_DEPTH24_STENCIL8;
    case COMPRESSED_RED:
        return GL_COMPRESSED_RED;
    case COMPRESSED_RG:
        return GL_COMPRESSED_RG;
    case COMPRESSED_RGB:
        return GL_COMPRESSED_RGB;
    case COMPRESSED_RGBA:
        return GL_COMPRESSED_RGBA;
    case COMPRESSED_SRGB:
        return GL_COMPRESSED_SRGB;
    case COMPRESSED_RED_RGTC1:
        return GL_COMPRESSED_RED_RGTC1;
    case COMPRESSED_SIGNED_RED_RGTC1:
        return GL_COMPRESSED_SIGNED_RED_RGTC1;
    case COMPRESSED_RG_RGTC2:
        return GL_COMPRESSED_RG_RGTC2;
    case COMPRESSED_SIGNED_RG_RGTC2:
        return GL_COMPRESSED_SIGNED_RG_RGTC2;
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
        return GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
        return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB;
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
        return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
        return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
        return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
        return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
        return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    }
    assert(false);
    throw exception();
}

GLenum getTextureFormat(TextureFormat f)
{
    switch (f) {
    case STENCIL_INDEX:
        return GL_STENCIL_INDEX;
    case DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;
    case RED:
        return GL_RED;
    case GREEN:
        return GL_GREEN;
    case BLUE:
        return GL_BLUE;
    case RG:
        return GL_RG;
    case RGB:
        return GL_RGB;
    case RGBA:
        return GL_RGBA;
    case BGR:
        return GL_BGR;
    case BGRA:
        return GL_BGRA;
    case RED_INTEGER:
        return GL_RED_INTEGER;
    case BLUE_INTEGER:
        return GL_BLUE_INTEGER;
    case GREEN_INTEGER:
        return GL_GREEN_INTEGER;
    case RG_INTEGER:
        return GL_RG_INTEGER;
    case RGB_INTEGER:
        return GL_RGB_INTEGER;
    case RGBA_INTEGER:
        return GL_RGBA_INTEGER;
    case BGR_INTEGER:
        return GL_BGR_INTEGER;
    case BGRA_INTEGER:
        return GL_BGRA_INTEGER;
    }
    assert(false);
    throw exception();
}

GLenum getPixelType(PixelType t)
{
    switch (t) {
    case UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
    case BYTE:
        return GL_BYTE;
    case UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
    case SHORT:
        return GL_SHORT;
    case UNSIGNED_INT:
        return GL_UNSIGNED_INT;
    case INT:
        return GL_INT;
    case HALF:
        return GL_HALF_FLOAT;
    case FLOAT:
        return GL_FLOAT;
    case UNSIGNED_BYTE_3_3_2:
        return GL_UNSIGNED_BYTE_3_3_2;
    case UNSIGNED_BYTE_2_3_3_REV:
        return GL_UNSIGNED_BYTE_2_3_3_REV;
    case UNSIGNED_SHORT_5_6_5:
        return GL_UNSIGNED_SHORT_5_6_5;
    case UNSIGNED_SHORT_5_6_5_REV:
        return GL_UNSIGNED_SHORT_5_6_5_REV;
    case UNSIGNED_SHORT_4_4_4_4:
        return GL_UNSIGNED_SHORT_4_4_4_4;
    case UNSIGNED_SHORT_4_4_4_4_REV:
        return GL_UNSIGNED_SHORT_4_4_4_4_REV;
    case UNSIGNED_SHORT_5_5_5_1:
        return GL_UNSIGNED_SHORT_5_5_5_1;
    case UNSIGNED_SHORT_1_5_5_5_REV:
        return GL_UNSIGNED_SHORT_1_5_5_5_REV;
    case UNSIGNED_INT_8_8_8_8:
        return GL_UNSIGNED_INT_8_8_8_8;
    case UNSIGNED_INT_8_8_8_8_REV:
        return GL_UNSIGNED_INT_8_8_8_8_REV;
    case UNSIGNED_INT_10_10_10_2:
        return GL_UNSIGNED_INT_10_10_10_2;
    case UNSIGNED_INT_2_10_10_10_REV:
        return GL_UNSIGNED_INT_2_10_10_10_REV;
    case UNSIGNED_INT_24_8:
        return GL_UNSIGNED_INT_24_8;
    case UNSIGNED_INT_10F_11F_11F_REV:
        return GL_UNSIGNED_INT_10F_11F_11F_REV;
    case UNSIGNED_INT_5_9_9_9_REV:
        return GL_UNSIGNED_INT_5_9_9_9_REV;
    case FLOAT_32_UNSIGNED_INT_24_8_REV:
        return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
    }
    assert(false);
    throw exception();
}

unsigned int getTextureComponents(TextureFormat f)
{
    switch (f) {
    case STENCIL_INDEX:
    case DEPTH_COMPONENT:
    case RED:
    case GREEN:
    case BLUE:
    case RED_INTEGER:
    case BLUE_INTEGER:
    case GREEN_INTEGER:
        return 1;
    case DEPTH_STENCIL:
    case RG:
    case RG_INTEGER:
        return 2;
    case RGB:
    case BGR:
    case RGB_INTEGER:
    case BGR_INTEGER:
        return 3;
    case RGBA:
    case BGRA:
    case RGBA_INTEGER:
    case BGRA_INTEGER:
        return 4;
    }
    assert(false);
    throw exception();
}

unsigned int getFormatSize(TextureFormat f, PixelType t)
{
    int components = getTextureComponents(f);

    switch (t) {
    case UNSIGNED_BYTE:
    case BYTE:
        return components;
    case UNSIGNED_SHORT:
    case SHORT:
    case HALF:
        return 2 * components;
    case UNSIGNED_INT:
    case INT:
    case FLOAT:
        return 4 * components;
    case UNSIGNED_BYTE_3_3_2:
    case UNSIGNED_BYTE_2_3_3_REV:
        return 1;
    case UNSIGNED_SHORT_5_6_5:
    case UNSIGNED_SHORT_5_6_5_REV:
    case UNSIGNED_SHORT_4_4_4_4:
    case UNSIGNED_SHORT_4_4_4_4_REV:
    case UNSIGNED_SHORT_5_5_5_1:
    case UNSIGNED_SHORT_1_5_5_5_REV:
        return 2;
    case UNSIGNED_INT_8_8_8_8:
    case UNSIGNED_INT_8_8_8_8_REV:
    case UNSIGNED_INT_10_10_10_2:
    case UNSIGNED_INT_2_10_10_10_REV:
    case UNSIGNED_INT_24_8:
    case UNSIGNED_INT_10F_11F_11F_REV:
    case UNSIGNED_INT_5_9_9_9_REV:
        return 4;
    case FLOAT_32_UNSIGNED_INT_24_8_REV:
        return 8;
    }

    assert(false);
    throw exception();
}

GLenum getTextureSwizzle(char c)
{
    switch (c) {
    case 'r':
        return GL_RED;
    case 'g':
        return GL_GREEN;
    case 'b':
        return GL_BLUE;
    case 'a':
        return GL_ALPHA;
    case '0':
        return GL_ZERO;
    case '1':
        return GL_ONE;
    }
    assert(false);
    throw exception();
}

GLenum getTextureWrap(TextureWrap w)
{
    switch (w) {
    case CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    case REPEAT:
        return GL_REPEAT;
    case MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    }
    assert(false);
    throw exception();
}

GLenum getTextureFilter(TextureFilter f)
{
    switch (f) {
    case NEAREST:
        return GL_NEAREST;
    case LINEAR:
        return GL_LINEAR;
    case NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
    case LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
    assert(false);
    throw exception();
}

GLenum getFunction(Function f)
{
    switch (f) {
    case LEQUAL:
        return GL_LEQUAL;
    case GEQUAL:
        return GL_GEQUAL;
    case LESS:
        return GL_LESS;
    case GREATER:
        return GL_GREATER;
    case EQUAL:
        return GL_EQUAL;
    case NOTEQUAL:
        return GL_NOTEQUAL;
    case ALWAYS:
        return GL_ALWAYS;
    case NEVER:
        return GL_NEVER;
    }
    assert(false);
    throw exception();
}

int getBufferId(BufferId b)
{
    switch (b) {
    case COLOR0:
        return 0;
    case COLOR1:
        return 1;
    case COLOR2:
        return 2;
    case COLOR3:
        return 3;
    case STENCIL:
        return 4;
    case DEPTH:
        return 5;
    }
    assert(false);
    throw exception();
}

GLenum getStencilOperation(StencilOperation o)
{
    switch (o) {
    case KEEP:
        return GL_KEEP;
    case RESET:
        return GL_ZERO;
    case REPLACE:
        return GL_REPLACE;
    case INVERT:
        return GL_INVERT;
    case INCR:
        return GL_INCR;
    case DECR:
        return GL_DECR;
    case INCR_WRAP:
        return GL_INCR_WRAP;
    case DECR_WRAP:
        return GL_DECR_WRAP;
    }
    assert(false);
    throw exception();
}

GLenum getBlendEquation(BlendEquation e)
{
    switch (e) {
    case ADD:
        return GL_FUNC_ADD;
    case SUBTRACT:
        return GL_FUNC_SUBTRACT;
    case REVERSE_SUBTRACT:
        return GL_FUNC_REVERSE_SUBTRACT;
    case MIN:
        return GL_MIN;
    case MAX:
        return GL_MAX;
    }
    assert(false);
    throw exception();
}

GLenum getBlendArgument(BlendArgument a)
{
    switch (a) {
    case ZERO:
        return GL_ZERO;
    case ONE:
        return GL_ONE;
    case SRC_COLOR:
        return GL_SRC_COLOR;
    case ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    case DST_COLOR:
        return GL_DST_COLOR;
    case ONE_MINUS_DST_COLOR:
        return GL_ONE_MINUS_DST_COLOR;
    case SRC_ALPHA:
        return GL_SRC_ALPHA;
    case ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case DST_ALPHA:
        return GL_DST_ALPHA;
    case ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;
    case CONSTANT_COLOR:
        return GL_CONSTANT_COLOR;
    case ONE_MINUS_CONSTANT_COLOR:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case CONSTANT_ALPHA:
        return GL_CONSTANT_ALPHA;
    case ONE_MINUS_CONSTANT_ALPHA:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
    assert(false);
    throw exception();
}

GLenum getLogicOperation(LogicOperation o)
{
    switch (o) {
    case CLEAR:
        return GL_CLEAR;
    case AND:
        return GL_AND;
    case AND_REVERSE:
        return GL_AND_REVERSE;
    case COPY:
        return GL_COPY;
    case AND_INVERTED:
        return GL_AND_INVERTED;
    case NOOP:
        return GL_NOOP;
    case XOR:
        return GL_XOR;
    case OR:
        return GL_OR;
    case NOR:
        return GL_NOR;
    case EQUIV:
        return GL_EQUIV;
    case NOT:
        return GL_INVERT;
    case OR_REVERSE:
        return GL_OR_REVERSE;
    case COPY_INVERTED:
        return GL_COPY_INVERTED;
    case OR_INVERTED:
        return GL_OR_INVERTED;
    case NAND:
        return GL_NAND;
    case SET:
        return GL_SET;
    }
    assert(false);
    throw exception();
}

GLenum getStage(Stage s)
{
    switch (s) {
    case VERTEX:
        return GL_VERTEX_SHADER;
    case TESSELATION_CONTROL:
        return GL_TESS_CONTROL_SHADER;
    case TESSELATION_EVALUATION:
        return GL_TESS_EVALUATION_SHADER;
    case GEOMETRY:
        return GL_GEOMETRY_SHADER;
    case FRAGMENT:
        return GL_FRAGMENT_SHADER;
    }
    assert(false);
    throw exception();
}

GLenum getQueryMode(QueryMode m)
{
    switch (m) {
    case WAIT:
        return GL_QUERY_WAIT;
    case NO_WAIT:
        return GL_QUERY_NO_WAIT;
    case REGION_WAIT:
        return GL_QUERY_BY_REGION_WAIT;
    case REGION_NO_WAIT:
        return GL_QUERY_BY_REGION_NO_WAIT;
    }
    assert(false);
    throw exception();
}

}
