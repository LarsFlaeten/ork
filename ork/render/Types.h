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

#ifndef _ORK_TYPES_H_
#define _ORK_TYPES_H_

#include <stdint.h>

/**
 * An enum value.
 */
typedef unsigned int GLenum;

/**
 * A boolean value.
 */
typedef unsigned char GLboolean;

/**
 * A bitfield value.
 */
typedef unsigned int GLbitfield;

/**
 * No value.
 */
typedef void GLvoid;

/**
 * A one byte signed value.
 */
typedef signed char GLbyte;

/**
 * A two bytes signed value.
 */
typedef short GLshort;

/**
 * A four bytes signed value.
 */
typedef int GLint;

/**
 * A signed 2’s complement 16.16 scaled integer.
 */
typedef int GLfixed;

/**
 * A one byte unsigned value.
 */
typedef unsigned char GLubyte;

/**
 * A two bytes unsigned value.
 */
typedef unsigned short GLushort;

/**
 * A four bytes unsigned value.
 */
typedef unsigned int GLuint;

#if defined(_MSC_VER)
#  if _MSC_VER < 1400

/**
 * An eight bytes signed value.
 */
typedef __int64 GLint64;

/**
 * An eight bytes unsigned value.
 */
typedef unsigned __int64 GLuint64;

#  else

/**
 * An eight bytes signed value.
 */
typedef signed long long GLint64;

/**
 * An eight bytes unsigned value.
 */
typedef unsigned long long GLuint64;

#  endif
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif

/**
 * An eight bytes signed value.
 */
typedef int64_t GLint64;

/**
 * An eight bytes unsigned value.
 */
typedef uint64_t GLuint64;

#endif

/**
 * A data structure size.
 */
typedef int GLsizei;

/**
 * A half precision float value.
 */
typedef short unsigned int GLhalf;

/**
 * A single precision float value.
 */
typedef float GLfloat;

/**
 * A single precision float value clamped to 0..1.
 */
typedef float GLclampf;

/**
 * A double precision float value.
 */
typedef double GLdouble;

/**
 * A double precision float value clamped to 0..1.
 */
typedef double GLclampd;

namespace ork
{

/**
 * A GPU buffer usage.
 */
enum BufferUsage {
    STREAM_DRAW, ///< &nbsp;
    STREAM_READ, ///< &nbsp;
    STREAM_COPY, ///< &nbsp;
    STATIC_DRAW, ///< &nbsp;
    STATIC_READ, ///< &nbsp;
    STATIC_COPY, ///< &nbsp;
    DYNAMIC_DRAW, ///< &nbsp;
    DYNAMIC_READ, ///< &nbsp;
    DYNAMIC_COPY ///< &nbsp;
};

/**
 * A GPU buffer access mode.
 */
enum BufferAccess {
    READ_ONLY, ///< &nbsp;
    WRITE_ONLY, ///< &nbsp;
    READ_WRITE ///< &nbsp;
};

/**
 * A vertex attribute component type.
 */
enum AttributeType {
    A8I, ///< &nbsp;
    A8UI, ///< &nbsp;
    A16I, ///< &nbsp;
    A16UI, ///< &nbsp;
    A32I, ///< &nbsp;
    A32UI, ///< &nbsp;
    A16F, ///< &nbsp;
    A32F, ///< &nbsp;
    A64F, ///< &nbsp;
    A32I_2_10_10_10_REV, ///< &nbsp;
    A32UI_2_10_10_10_REV, ///< &nbsp;
    A32I_FIXED ///< &nbsp;
};

/**
 * A mesh topology.
 */
enum MeshMode {
    POINTS, ///< &nbsp;
    LINE_STRIP, ///< &nbsp;
    LINE_LOOP, ///< &nbsp;
    LINES, ///< &nbsp;
    TRIANGLE_STRIP, ///< &nbsp;
    TRIANGLE_FAN, ///< &nbsp;
    TRIANGLES, ///< &nbsp;
    LINES_ADJACENCY, ///< &nbsp;
    LINE_STRIP_ADJACENCY, ///< &nbsp;
    TRIANGLES_ADJACENCY, ///< &nbsp;
    TRIANGLE_STRIP_ADJACENCY, ///< &nbsp;
    PATCHES ///< &nbsp;
};

/**
 * A mesh usage.
 */
enum MeshUsage
{
    CPU, ///< &nbsp;
    GPU_STATIC, ///< &nbsp;
    GPU_DYNAMIC, ///< &nbsp;
    GPU_STREAM ///< &nbsp;
};

/**
 * A cube face identifier.
 */
enum CubeFace {
    POSITIVE_X = 0, ///< &nbsp;
    NEGATIVE_X = 1, ///< &nbsp;
    POSITIVE_Y = 2, ///< &nbsp;
    NEGATIVE_Y = 3, ///< &nbsp;
    POSITIVE_Z = 4, ///< &nbsp;
    NEGATIVE_Z = 5 ///< &nbsp;
};

/**
 * A texture internal format on GPU. Specifies pixel components and their type.
 */
enum TextureInternalFormat {
    R8, ///< &nbsp;
    R8_SNORM, ///< &nbsp;
    R16, ///< &nbsp;
    R16_SNORM, ///< &nbsp;
    RG8, ///< &nbsp;
    RG8_SNORM, ///< &nbsp;
    RG16, ///< &nbsp;
    RG16_SNORM, ///< &nbsp;
    R3_G3_B2, ///< &nbsp;
    RGB4, ///< &nbsp;
    RGB5, ///< &nbsp;
    RGB8, ///< &nbsp;
    RGB8_SNORM, ///< &nbsp;
    RGB10, ///< &nbsp;
    RGB12, ///< &nbsp;
    RGB16, ///< &nbsp;
    RGB16_SNORM, ///< &nbsp;
    RGBA2, ///< &nbsp;
    RGBA4, ///< &nbsp;
    RGB5_A1, ///< &nbsp;
    RGBA8, ///< &nbsp;
    RGBA8_SNORM, ///< &nbsp;
    RGB10_A2, ///< &nbsp;
    RGB10_A2UI, ///< &nbsp;
    RGBA12, ///< &nbsp;
    RGBA16, ///< &nbsp;
    RGBA16_SNORM, ///< &nbsp;
    SRGB8, ///< &nbsp;
    SRGB8_ALPHA8, ///< &nbsp;
    R16F, ///< &nbsp;
    RG16F, ///< &nbsp;
    RGB16F, ///< &nbsp;
    RGBA16F, ///< &nbsp;
    R32F, ///< &nbsp;
    RG32F, ///< &nbsp;
    RGB32F, ///< &nbsp;
    RGBA32F, ///< &nbsp;
    R11F_G11F_B10F, ///< &nbsp;
    RGB9_E5, ///< &nbsp;
    R8I, ///< &nbsp;
    R8UI, ///< &nbsp;
    R16I, ///< &nbsp;
    R16UI, ///< &nbsp;
    R32I, ///< &nbsp;
    R32UI, ///< &nbsp;
    RG8I, ///< &nbsp;
    RG8UI, ///< &nbsp;
    RG16I, ///< &nbsp;
    RG16UI, ///< &nbsp;
    RG32I, ///< &nbsp;
    RG32UI, ///< &nbsp;
    RGB8I, ///< &nbsp;
    RGB8UI, ///< &nbsp;
    RGB16I, ///< &nbsp;
    RGB16UI, ///< &nbsp;
    RGB32I, ///< &nbsp;
    RGB32UI, ///< &nbsp;
    RGBA8I, ///< &nbsp;
    RGBA8UI, ///< &nbsp;
    RGBA16I, ///< &nbsp;
    RGBA16UI, ///< &nbsp;
    RGBA32I, ///< &nbsp;
    RGBA32UI, ///< &nbsp;
    DEPTH_COMPONENT16, ///< &nbsp;
    DEPTH_COMPONENT24, ///< &nbsp;
    DEPTH_COMPONENT32F, ///< &nbsp;
    DEPTH32F_STENCIL8, ///< &nbsp;
    DEPTH24_STENCIL8, ///< &nbsp;
    COMPRESSED_RED, ///< &nbsp;
    COMPRESSED_RG, ///< &nbsp;
    COMPRESSED_RGB, ///< &nbsp;
    COMPRESSED_RGBA, ///< &nbsp;
    COMPRESSED_SRGB, ///< &nbsp;
    COMPRESSED_RED_RGTC1, ///< &nbsp;
    COMPRESSED_SIGNED_RED_RGTC1, ///< &nbsp;
    COMPRESSED_RG_RGTC2, ///< &nbsp;
    COMPRESSED_SIGNED_RG_RGTC2, ///< &nbsp;
    COMPRESSED_RGBA_BPTC_UNORM_ARB, ///< &nbsp;
    COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB, ///< &nbsp;
    COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB, ///< &nbsp;
    COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, ///< &nbsp;
    COMPRESSED_RGB_S3TC_DXT1_EXT, ///< &nbsp;
    COMPRESSED_RGBA_S3TC_DXT1_EXT, ///< &nbsp;
    COMPRESSED_RGBA_S3TC_DXT3_EXT, ///< &nbsp;
    COMPRESSED_RGBA_S3TC_DXT5_EXT ///< &nbsp;
};

/**
 * A texture format. Specifies texture components to be read or written.
 */
enum TextureFormat {
    STENCIL_INDEX, ///< &nbsp;
    DEPTH_COMPONENT, ///< &nbsp;
    DEPTH_STENCIL, ///< &nbsp;
    RED, ///< &nbsp;
    GREEN, ///< &nbsp;
    BLUE, ///< &nbsp;
    RG, ///< &nbsp;
    RGB, ///< &nbsp;
    RGBA, ///< &nbsp;
    BGR, ///< &nbsp;
    BGRA, ///< &nbsp;
    RED_INTEGER, ///< &nbsp;
    BLUE_INTEGER, ///< &nbsp;
    GREEN_INTEGER, ///< &nbsp;
    RG_INTEGER, ///< &nbsp;
    RGB_INTEGER, ///< &nbsp;
    RGBA_INTEGER, ///< &nbsp;
    BGR_INTEGER, ///< &nbsp;
    BGRA_INTEGER ///< &nbsp;
};

/**
 * A pixel component type. Specifies a type for reading or writing texture data.
 */
enum PixelType {
    UNSIGNED_BYTE, ///< &nbsp;
    BYTE, ///< &nbsp;
    UNSIGNED_SHORT, ///< &nbsp;
    SHORT, ///< &nbsp;
    UNSIGNED_INT, ///< &nbsp;
    INT, ///< &nbsp;
    HALF, ///< &nbsp;
    FLOAT, ///< &nbsp;
    UNSIGNED_BYTE_3_3_2, ///< &nbsp;
    UNSIGNED_BYTE_2_3_3_REV, ///< &nbsp;
    UNSIGNED_SHORT_5_6_5, ///< &nbsp;
    UNSIGNED_SHORT_5_6_5_REV, ///< &nbsp;
    UNSIGNED_SHORT_4_4_4_4, ///< &nbsp;
    UNSIGNED_SHORT_4_4_4_4_REV, ///< &nbsp;
    UNSIGNED_SHORT_5_5_5_1, ///< &nbsp;
    UNSIGNED_SHORT_1_5_5_5_REV, ///< &nbsp;
    UNSIGNED_INT_8_8_8_8, ///< &nbsp;
    UNSIGNED_INT_8_8_8_8_REV, ///< &nbsp;
    UNSIGNED_INT_10_10_10_2, ///< &nbsp;
    UNSIGNED_INT_2_10_10_10_REV, ///< &nbsp;
    UNSIGNED_INT_24_8, ///< &nbsp;
    UNSIGNED_INT_10F_11F_11F_REV, ///< &nbsp;
    UNSIGNED_INT_5_9_9_9_REV, ///< &nbsp;
    FLOAT_32_UNSIGNED_INT_24_8_REV ///< &nbsp;
};

/**
 * A texture wrap mode for texture access.
 */
enum TextureWrap {
    CLAMP_TO_EDGE, ///< &nbsp;
    CLAMP_TO_BORDER, ///< &nbsp;
    REPEAT, ///< &nbsp;
    MIRRORED_REPEAT ///< &nbsp;
};

/**
 * A texture filter mode for texture minification and magnification.
 */
enum TextureFilter {
    NEAREST, ///< &nbsp;
    LINEAR, ///< &nbsp;
    NEAREST_MIPMAP_NEAREST, ///< &nbsp;
    NEAREST_MIPMAP_LINEAR, ///< &nbsp;
    LINEAR_MIPMAP_NEAREST, ///< &nbsp;
    LINEAR_MIPMAP_LINEAR ///< &nbsp;
};

/**
 * A comparison function for texture compare, stencil test or depth test.
 */
enum Function {
    LEQUAL, ///< &nbsp;
    GEQUAL, ///< &nbsp;
    LESS, ///< &nbsp;
    GREATER, ///< &nbsp;
    EQUAL, ///< &nbsp;
    NOTEQUAL, ///< &nbsp;
    ALWAYS, ///< &nbsp;
    NEVER ///< &nbsp;
};

/**
 * A framebuffer attachment point.
 */
enum BufferId {
    COLOR0 = 1, ///< &nbsp;
    COLOR1 = 2, ///< &nbsp;
    COLOR2 = 4, ///< &nbsp;
    COLOR3 = 8, ///< &nbsp;
    COLOR4 = 16, ///< &nbsp;
    COLOR5 = 32, ///< &nbsp;
    COLOR6 = 64, ///< &nbsp;
    COLOR7 = 128, ///< &nbsp;
    STENCIL = 256, ///< &nbsp;
    DEPTH = 512 ///< depth
};

/**
 * A polygon drawing mode.
 */
enum PolygonMode {
    CULL, ///< &nbsp;
    POINT, ///< &nbsp;
    LINE, ///< &nbsp;
    FILL ///< &nbsp;
};

/**
 * A stencil operation to perform if the stencil test passes or fails.
 */
enum StencilOperation {
    KEEP, ///< &nbsp;
    RESET, ///< &nbsp;
    REPLACE, ///< &nbsp;
    INCR, ///< &nbsp;
    DECR, ///< &nbsp;
    INVERT, ///< &nbsp;
    INCR_WRAP, ///< &nbsp;
    DECR_WRAP ///< &nbsp;
};

/**
 * An equation to blend the fragment colors with the framebuffer colors.
 */
enum BlendEquation {
    ADD, ///< &nbsp;
    SUBTRACT, ///< &nbsp;
    REVERSE_SUBTRACT, ///< &nbsp;
    MIN, ///< &nbsp;
    MAX ///< &nbsp;
};

/**
 * A blend equation argument for blending pixels.
 */
enum BlendArgument {
    ZERO, ///< &nbsp;
    ONE, ///< &nbsp;
    SRC_COLOR, ///< &nbsp;
    ONE_MINUS_SRC_COLOR, ///< &nbsp;
    DST_COLOR, ///< &nbsp;
    ONE_MINUS_DST_COLOR, ///< &nbsp;
    SRC_ALPHA, ///< &nbsp;
    ONE_MINUS_SRC_ALPHA, ///< &nbsp;
    DST_ALPHA, ///< &nbsp;
    ONE_MINUS_DST_ALPHA, ///< &nbsp;
    CONSTANT_COLOR, ///< &nbsp;
    ONE_MINUS_CONSTANT_COLOR, ///< &nbsp;
    CONSTANT_ALPHA, ///< &nbsp;
    ONE_MINUS_CONSTANT_ALPHA ///< &nbsp;
};

/**
 * A logical operation to combine fragment colors with the framebuffer colors.
 */
enum LogicOperation {
    CLEAR, ///< &nbsp;
    AND, ///< &nbsp;
    AND_REVERSE, ///< &nbsp;
    COPY, ///< &nbsp;
    AND_INVERTED, ///< &nbsp;
    NOOP , ///< &nbsp;
    XOR, ///< &nbsp;
    OR, ///< &nbsp;
    NOR, ///< &nbsp;
    EQUIV, ///< &nbsp;
    NOT, ///< &nbsp;
    OR_REVERSE, ///< &nbsp;
    COPY_INVERTED, ///< &nbsp;
    OR_INVERTED, ///< &nbsp;
    NAND, ///< &nbsp;
    SET ///< &nbsp;
};

/**
 * A shader type.
 */
enum Stage {
    VERTEX = 0, ///< &nbsp;
    TESSELATION_CONTROL = 1, ///< &nbsp;
    TESSELATION_EVALUATION = 2, ///< &nbsp;
    GEOMETRY = 3, ///< &nbsp;
    FRAGMENT = 4 ///< &nbsp;
};

/**
 * A uniform type.
 */
enum UniformType {
    VEC1F, ///< &nbsp;
    VEC2F, ///< &nbsp;
    VEC3F, ///< &nbsp;
    VEC4F, ///< &nbsp;
    VEC1D, ///< &nbsp;
    VEC2D, ///< &nbsp;
    VEC3D, ///< &nbsp;
    VEC4D, ///< &nbsp;
    VEC1I, ///< &nbsp;
    VEC2I, ///< &nbsp;
    VEC3I, ///< &nbsp;
    VEC4I, ///< &nbsp;
    VEC1UI, ///< &nbsp;
    VEC2UI, ///< &nbsp;
    VEC3UI, ///< &nbsp;
    VEC4UI, ///< &nbsp;
    VEC1B, ///< &nbsp;
    VEC2B, ///< &nbsp;
    VEC3B, ///< &nbsp;
    VEC4B, ///< &nbsp;
    MAT2F, ///< &nbsp;
    MAT3F, ///< &nbsp;
    MAT4F, ///< &nbsp;
    MAT2x3F, ///< &nbsp;
    MAT2x4F, ///< &nbsp;
    MAT3x2F, ///< &nbsp;
    MAT3x4F, ///< &nbsp;
    MAT4x2F, ///< &nbsp;
    MAT4x3F, ///< &nbsp;
    MAT2D, ///< &nbsp;
    MAT3D, ///< &nbsp;
    MAT4D, ///< &nbsp;
    MAT2x3D, ///< &nbsp;
    MAT2x4D, ///< &nbsp;
    MAT3x2D, ///< &nbsp;
    MAT3x4D, ///< &nbsp;
    MAT4x2D, ///< &nbsp;
    MAT4x3D, ///< &nbsp;
    SAMPLER_1D, ///< &nbsp;
    SAMPLER_2D, ///< &nbsp;
    SAMPLER_3D, ///< &nbsp;
    SAMPLER_CUBE, ///< &nbsp;
    SAMPLER_1D_ARRAY, ///< &nbsp;
    SAMPLER_2D_ARRAY, ///< &nbsp;
    SAMPLER_CUBE_MAP_ARRAY, ///< &nbsp;
    SAMPLER_2D_MULTISAMPLE, ///< &nbsp;
    SAMPLER_2D_MULTISAMPLE_ARRAY, ///< &nbsp;
    SAMPLER_BUFFER, ///< &nbsp;
    SAMPLER_2D_RECT, ///< &nbsp;
    INT_SAMPLER_1D, ///< &nbsp;
    INT_SAMPLER_2D, ///< &nbsp;
    INT_SAMPLER_3D, ///< &nbsp;
    INT_SAMPLER_CUBE, ///< &nbsp;
    INT_SAMPLER_1D_ARRAY, ///< &nbsp;
    INT_SAMPLER_2D_ARRAY, ///< &nbsp;
    INT_SAMPLER_CUBE_MAP_ARRAY, ///< &nbsp;
    INT_SAMPLER_2D_MULTISAMPLE, ///< &nbsp;
    INT_SAMPLER_2D_MULTISAMPLE_ARRAY, ///< &nbsp;
    INT_SAMPLER_BUFFER, ///< &nbsp;
    INT_SAMPLER_2D_RECT, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_1D, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_2D, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_3D, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_CUBE, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_1D_ARRAY, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_2D_ARRAY, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_BUFFER, ///< &nbsp;
    UNSIGNED_INT_SAMPLER_2D_RECT, ///< &nbsp;
    SUBROUTINE ///< &nbsp;
};

/**
 * An asynchronous query type.
 */
enum QueryType {
    PRIMITIVES_GENERATED, ///< &nbsp;
    TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, ///< &nbsp;
    SAMPLES_PASSED, ///< &nbsp;
    ANY_SAMPLES_PASSED, ///< &nbsp;
    TIME_STAMP ///< &nbsp;
};

/**
 * An asynchronous query mode.
 */
enum QueryMode {
    WAIT, ///< &nbsp;
    NO_WAIT, ///< &nbsp;
    REGION_WAIT, ///< &nbsp;
    REGION_NO_WAIT ///< &nbsp;
};

}

#endif
