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

#include "ork/render/FrameBuffer.h"

#include <sstream>
#include <GL/glew.h>

#if defined( _WIN64 ) || defined( _WIN32 )
#include <windows.h>
#else
#include <GL/glx.h>
#endif

#include "ork/core/Logger.h"
#include "ork/render/Module.h"
#include "ork/render/Texture.h"

inline void glEnable(GLenum p, bool b)
{
    if (b) {
        ::glEnable(p);
    } else {
        glDisable(p);
    }
}

namespace ork
{

GLenum getMeshMode(MeshMode m);

GLenum getCubeFace(CubeFace f);

GLenum getFunction(Function f);

GLenum getStencilOperation(StencilOperation o);

GLenum getBlendEquation(BlendEquation e);

GLenum getBlendArgument(BlendArgument a);

GLenum getLogicOperation(LogicOperation o);

int getBufferId(BufferId b);

GLenum getTextureFormat(TextureFormat f);

GLenum getPixelType(PixelType t);

GLenum getQueryMode(QueryMode m);

int PARAMETER_ID = 0;

void checkExtensions()
{
    static bool checked = false;
    if (!checked) {
        // if one of these assertions fails, try with adding EXT to all
        // glProgramUniform calls (including in Uniform.cpp).
        // if this fails too, use the ORK_NO_GLPROGRAMUNIFORM preprocessor
        // directive (little slower at runtime).
#ifndef ORK_NO_GLPROGRAMUNIFORM
        assert(glProgramUniform1iEXT != NULL);
        assert(glProgramUniform1fEXT != NULL);
        assert(glProgramUniform1d != NULL);
        assert(glProgramUniform2iEXT != NULL);
        assert(glProgramUniform2fEXT != NULL);
        assert(glProgramUniform2d != NULL);
        assert(glProgramUniform3iEXT != NULL);
        assert(glProgramUniform3fEXT != NULL);
        assert(glProgramUniform3d != NULL);
        assert(glProgramUniform4iEXT != NULL);
        assert(glProgramUniform4fEXT != NULL);
        assert(glProgramUniform4d != NULL);
        assert(glProgramUniform1uiEXT != NULL);
        assert(glProgramUniform2uiEXT != NULL);
        assert(glProgramUniform3uiEXT != NULL);
        assert(glProgramUniform4uiEXT != NULL);
        assert(glProgramUniformMatrix2fvEXT != NULL);
        assert(glProgramUniformMatrix2dv != NULL);
        assert(glProgramUniformMatrix3fvEXT != NULL);
        assert(glProgramUniformMatrix3dv != NULL);
        assert(glProgramUniformMatrix4fvEXT != NULL);
        assert(glProgramUniformMatrix4dv != NULL);
        assert(glProgramUniformMatrix2x3fvEXT != NULL);
        assert(glProgramUniformMatrix2x3dv != NULL);
        assert(glProgramUniformMatrix2x4fvEXT != NULL);
        assert(glProgramUniformMatrix2x4dv != NULL);
        assert(glProgramUniformMatrix3x2fvEXT != NULL);
        assert(glProgramUniformMatrix3x2dv != NULL);
        assert(glProgramUniformMatrix3x4fvEXT != NULL);
        assert(glProgramUniformMatrix3x4dv != NULL);
        assert(glProgramUniformMatrix4x2fvEXT != NULL);
        assert(glProgramUniformMatrix4x2dv != NULL);
        assert(glProgramUniformMatrix4x3fvEXT != NULL);
        assert(glProgramUniformMatrix4x3dv != NULL);
#else
        assert(glUniform1i != NULL);
        assert(glUniform1f != NULL);
        assert(glUniform1d != NULL);
        assert(glUniform2i != NULL);
        assert(glUniform2f != NULL);
        assert(glUniform2d != NULL);
        assert(glUniform3i != NULL);
        assert(glUniform3f != NULL);
        assert(glUniform3d != NULL);
        assert(glUniform4i != NULL);
        assert(glUniform4f != NULL);
        assert(glUniform4d != NULL);
        assert(glUniform1ui != NULL);
        assert(glUniform2ui != NULL);
        assert(glUniform3ui != NULL);
        assert(glUniform4ui != NULL);
        assert(glUniformMatrix2fv != NULL);
        assert(glUniformMatrix2dv != NULL);
        assert(glUniformMatrix3fv != NULL);
        assert(glUniformMatrix3dv != NULL);
        assert(glUniformMatrix4fv != NULL);
        assert(glUniformMatrix4dv != NULL);
        assert(glUniformMatrix2x3fv != NULL);
        assert(glUniformMatrix2x3dv != NULL);
        assert(glUniformMatrix2x4fv != NULL);
        assert(glUniformMatrix2x4dv != NULL);
        assert(glUniformMatrix3x2fv != NULL);
        assert(glUniformMatrix3x2dv != NULL);
        assert(glUniformMatrix3x4fv != NULL);
        assert(glUniformMatrix3x4dv != NULL);
        assert(glUniformMatrix4x2fv != NULL);
        assert(glUniformMatrix4x2dv != NULL);
        assert(glUniformMatrix4x3fv != NULL);
        assert(glUniformMatrix4x3dv != NULL);
#endif
        checked = true;
    }
}

FrameBuffer::Parameters::Parameters() :
    viewport(0, 0, 0, 0), depthRange(0.0f, 1.0f), clipDistances(0), transformId(0),
    clearColor(0.0f, 0.0f, 0.0f, 0.0f), clearDepth(1.0f), clearStencil(0), clearId(0),
    pointSize(1.0f), pointFadeThresholdSize(1.0f), pointLowerLeftOrigin(false), pointId(0),
    lineWidth(1.0f), lineSmooth(false),
    frontFaceCW(false), polygonFront(FILL), polygonBack(FILL),
    polygonSmooth(false), polygonOffset(0.0f, 0.0f), polygonOffsets(false, false, false), polygonId(0),
    multiSample(true), sampleAlphaToCoverage(false), sampleAlphaToOne(false),
        sampleCoverage(1.0f), sampleMask(0xFFFFFFFF), multiSampleId(0),
    occlusionQuery(NULL), occlusionMode(WAIT),
    enableScissor(false), scissor(0, 0, 0, 0),
    enableStencil(false), ffunc(ALWAYS), fref(0), fmask(0xFFFFFFFF), ffail(KEEP), fdpfail(KEEP), fdppass(KEEP),
        bfunc(ALWAYS), bref(0), bmask(0xFFFFFFFF), bfail(KEEP), bdpfail(KEEP), bdppass(KEEP), stencilId(0),
    enableDepth(false), depth(LESS),
    multiBlendEnable(false), multiBlendEq(false), color(0.0f, 0.0f, 0.0f, 0.0f), blendId(0),
    enableDither(false),
    enableLogic(false), logicOp(COPY),
    multiColorMask(false), depthMask(true), stencilMaskFront(0xFFFFFFFF), stencilMaskBack(0xFFFFFFFF), maskId(0)
{
    for (int i = 0; i < 4; ++i) {
        enableBlend[i] = false;
        rgb[i] = ADD;
        srgb[i] = ONE;
        drgb[i] = ZERO;
        alpha[i] = ADD;
        salpha[i] = ONE;
        dalpha[i] = ZERO;
        colorMask[i] = vec4<bool>(true, true, true, true);
    }
}

void FrameBuffer::Parameters::set(const Parameters &p)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "Set FrameBuffer Parameters");
    }
    GLint version = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &version);

    // TRANSFORM -------------
    if (transformId != p.transformId)
    {
        glViewport(p.viewport.x, p.viewport.y, p.viewport.z, p.viewport.w);
        glDepthRange(p.depthRange.x, p.depthRange.y);
        for (int i = 0; i < 6; ++i) {
            glEnable(GL_CLIP_DISTANCE0 + i, (p.clipDistances & (1 << i)) != 0);
        }
    }
    // CLEAR -------------
    if (clearId != p.clearId)
    {
        glClearColor(p.clearColor.x, p.clearColor.y, p.clearColor.z, p.clearColor.w);
        glClearDepth(p.clearDepth);
        glClearStencil(p.clearStencil);
    }
    // POINTS -------------
    if (pointId != p.pointId)
    {
        glEnable(GL_PROGRAM_POINT_SIZE, p.pointSize <= 0.0f);
        glPointSize(p.pointSize);
        glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, p.pointFadeThresholdSize);
        glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, p.pointLowerLeftOrigin ? GL_LOWER_LEFT : GL_UPPER_LEFT);
    }
    // LINES -------------
    if (lineWidth != p.lineWidth ||
        lineSmooth != p.lineSmooth)
    {
        glEnable(GL_LINE_SMOOTH, p.lineSmooth);
        glLineWidth(p.lineWidth);
    }
    // POLYGONS -------------
    if (polygonId != p.polygonId)
    {
        glFrontFace(p.frontFaceCW ? GL_CW : GL_CCW);

        if (p.polygonFront == CULL || p.polygonBack == CULL) {
            glEnable(GL_CULL_FACE);
            if (p.polygonFront == CULL && p.polygonBack == CULL) {
                glCullFace(GL_FRONT_AND_BACK);
            } else if (p.polygonFront == CULL) {
                glCullFace(GL_FRONT);
            } else {
                glCullFace(GL_BACK);
            }
        } else {
            glDisable(GL_CULL_FACE);
        }
        switch (p.polygonFront) {
        case CULL:
            switch (p.polygonBack) {
            case CULL:
                break;
            case POINT:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            case LINE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case FILL:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
            break;
        case POINT:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case LINE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FILL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
        assert(getError() == 0);
        glEnable(GL_POLYGON_SMOOTH, p.polygonSmooth);
        glPolygonOffset(p.polygonOffset.x, p.polygonOffset.y);
        glEnable(GL_POLYGON_OFFSET_POINT, p.polygonOffsets.x);
        glEnable(GL_POLYGON_OFFSET_LINE, p.polygonOffsets.y);
        glEnable(GL_POLYGON_OFFSET_FILL, p.polygonOffsets.z);
    }
    // MULTISAMPLING -------------
    if (multiSampleId != p.multiSampleId)
    {
        glEnable(GL_MULTISAMPLE, p.multiSample);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE, p.sampleAlphaToCoverage);
        glEnable(GL_SAMPLE_ALPHA_TO_ONE, p.sampleAlphaToOne);
        glEnable(GL_SAMPLE_COVERAGE, p.sampleCoverage < 1.0f);
        glSampleCoverage(abs(p.sampleCoverage), p.sampleCoverage < 0.0f);
        glEnable(GL_SAMPLE_MASK, p.sampleMask != (GLuint) 0xFFFFFFFF);
        glSampleMaski(0, p.sampleMask);
        if (version >= 4) {
            glEnable(GL_SAMPLE_SHADING,p.sampleShading);
            glMinSampleShading(p.samplesMin);
        }
    }
    // SCISSOR TEST -------------
    if (enableScissor != p.enableScissor ||
        scissor != p.scissor)
    {
        glEnable(GL_SCISSOR_TEST, p.enableScissor);
        glScissor(p.scissor.x, p.scissor.y, p.scissor.z, p.scissor.w);
    }
    // STENCIL TEST -------------
    if (stencilId != p.stencilId)
    {
        glEnable(GL_STENCIL_TEST, p.enableStencil);
        glStencilFuncSeparate(GL_FRONT, getFunction(p.ffunc), p.fref, p.fmask);
        glStencilFuncSeparate(GL_BACK, getFunction(p.bfunc), p.bref, p.bmask);
        glStencilOpSeparate(GL_FRONT, getStencilOperation(p.ffail), getStencilOperation(p.fdpfail), getStencilOperation(p.fdppass));
        glStencilOpSeparate(GL_BACK, getStencilOperation(p.bfail), getStencilOperation(p.bdpfail), getStencilOperation(p.bdppass));
    }
    // DEPTH TEST -------------
    if (enableDepth != p.enableDepth ||
        depth != p.depth)
    {
        glEnable(GL_DEPTH_TEST, p.enableDepth);
        glDepthFunc(getFunction(p.depth));
    }
    // BLENDING --------------
    if (blendId != p.blendId)
    {
        if (p.multiBlendEnable) {
            for (int i = 0; i < 4; ++i) {
                if (p.enableBlend[i]) {
                    glEnablei(GL_BLEND, i);
                } else {
                    glDisablei(GL_BLEND, i);
                }
            }
        } else {
            glEnable(GL_BLEND, p.enableBlend[0]);
        }
        if (p.multiBlendEq && version >= 4) {
            for (int i = 0; i < 4; ++i) {
                glBlendEquationSeparatei(i, getBlendEquation(p.rgb[i]), getBlendEquation(p.alpha[i]));
                glBlendFuncSeparatei(i, getBlendArgument(p.srgb[i]), getBlendArgument(p.drgb[i]), getBlendArgument(p.salpha[i]), getBlendArgument(p.dalpha[i]));
            }
        } else {
            glBlendEquationSeparate(getBlendEquation(p.rgb[0]), getBlendEquation(p.alpha[0]));
            glBlendFuncSeparate(getBlendArgument(p.srgb[0]), getBlendArgument(p.drgb[0]), getBlendArgument(p.salpha[0]), getBlendArgument(p.dalpha[0]));
        }
        glBlendColor(p.color.x, p.color.y, p.color.z, p.color.w);
    }
    // DITHERING --------------
    if (enableDither != p.enableDither)
    {
        glEnable(GL_DITHER, p.enableDither);
    }
    // LOGIC OP --------------
    if (enableLogic != p.enableLogic ||
        logicOp != p.logicOp)
    {
        glEnable(GL_COLOR_LOGIC_OP, p.enableDither);
        glLogicOp(getLogicOperation(p.logicOp));
    }
    // WRITE MASKS --------------
    if (maskId != p.maskId)
    {
        if (p.multiColorMask) {
            for (int i = 0; i < 4; ++i) {
                glColorMaski(i, p.colorMask[i].x, p.colorMask[i].y, p.colorMask[i].z, p.colorMask[i].w);
            }
        } else {
            glColorMask(p.colorMask[0].x, p.colorMask[0].y, p.colorMask[0].z, p.colorMask[0].w);
        }
        glDepthMask(p.depthMask);
        glStencilMaskSeparate(GL_FRONT, p.stencilMaskFront);
        glStencilMaskSeparate(GL_BACK, p.stencilMaskBack);
    }
    assert(getError() == 0);
    *this = p;
}

FrameBuffer::FrameBuffer() : Object("FrameBuffer"),
    attachmentsChanged(false), readDrawChanged(false), parametersChanged(false)
{
    checkExtensions();
    glGenFramebuffers(1, &framebufferId);
    assert(getError() == 0);

    for (int i = 0; i < 6; ++i) {
        textures[i] = NULL;
        levels[i] = 0;
        layers[i] = 0;
    }

    readBuffer = COLOR0;
    drawBufferCount = 1;
    drawBuffers[0] = COLOR0;
}

FrameBuffer::FrameBuffer(bool main) : Object("FrameBuffer"),
    attachmentsChanged(false), readDrawChanged(false), parametersChanged(false)
{
    checkExtensions();
    if (main) {
        framebufferId = 0;
    } else {
        glGenFramebuffers(1, &framebufferId);
        assert(getError() == 0);
    }
    readBuffer = COLOR0;
    drawBufferCount = 1;
    drawBuffers[0] = COLOR0;
}

FrameBuffer::~FrameBuffer()
{
    if (CURRENT == this) {
        CURRENT = NULL;
    }
    if (framebufferId != 0) {
        glDeleteFramebuffers(1, &framebufferId);
        assert(getError() == 0);
    }
}

ptr<FrameBuffer> FrameBuffer::getDefault()
{
    void* context;
#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
    context = wglGetCurrentContext();
#else
    context = glXGetCurrentContext();
#endif
    ptr<FrameBuffer> fb;
    map< void*, ptr<FrameBuffer> >::iterator i = DEFAULT->buffers.find(context);
    if (i == DEFAULT->buffers.end()) {
        fb = new FrameBuffer(true);
        DEFAULT->buffers.insert(make_pair(context, fb));
    } else {
        fb = i->second;
    }
    return fb;
}

ptr<RenderBuffer> FrameBuffer::getRenderBuffer(BufferId b)
{
    return textures[getBufferId(b)].cast<RenderBuffer>();
}

ptr<Texture> FrameBuffer::getTextureBuffer(BufferId b)
{
   return textures[getBufferId(b)].cast<Texture>();
}

void FrameBuffer::setRenderBuffer(BufferId b, const ptr<RenderBuffer> t)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture1D> t, int level)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture1DArray> t, int level, int layer)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    layers[i] = layer;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture2D> t, int level)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<TextureRectangle> t, int level)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture2DMultisample> t)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = 0;
    attachmentsChanged = true;
}


void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture2DArray> t, int level, int layer)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    layers[i] = layer;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture2DMultisampleArray> t, int layer)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = 0;
    layers[i] = layer;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<Texture3D> t, int level, int layer)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    layers[i] = layer;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<TextureCube> t, int level, CubeFace cf)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    layers[i] = (int) cf;
    attachmentsChanged = true;
}

void FrameBuffer::setTextureBuffer(BufferId b, const ptr<TextureCubeArray> t, int level, CubeFace cf)
{
    assert(framebufferId != 0);
    int i = getBufferId(b);
    textures[i] = t;
    levels[i] = level;
    layers[i] = (int) cf;
    attachmentsChanged = true;
}

void FrameBuffer::setReadBuffer(BufferId b)
{
    readBuffer = b;
    readDrawChanged = true;
}

void FrameBuffer::setDrawBuffer(BufferId b)
{
    drawBufferCount = 1;
    drawBuffers[0] = b;
    readDrawChanged = true;
}

void FrameBuffer::setDrawBuffers(BufferId b)
{
    drawBufferCount = 0;
    if ((b & COLOR0) != 0) {
        drawBuffers[drawBufferCount++] = COLOR0;
    }
    if ((b & COLOR1) != 0) {
        drawBuffers[drawBufferCount++] = COLOR1;
    }
    if ((b & COLOR2) != 0) {
        drawBuffers[drawBufferCount++] = COLOR2;
    }
    if ((b & COLOR3) != 0) {
        drawBuffers[drawBufferCount++] = COLOR3;
    }
    readDrawChanged = true;
}

FrameBuffer::Parameters FrameBuffer::getParameters()
{
    return parameters;
}

vec4<GLint> FrameBuffer::getViewport()
{
    return parameters.viewport;
}

vec2<GLfloat> FrameBuffer::getDepthRange()
{
    return parameters.depthRange;
}

GLint FrameBuffer::getClipDistances()
{
    return parameters.clipDistances;
}

vec4<GLfloat> FrameBuffer::getClearColor()
{
    return parameters.clearColor;
}

GLfloat FrameBuffer::getClearDepth()
{
    return parameters.clearDepth;
}

GLint FrameBuffer::getClearStencil()
{
    return parameters.clearStencil;
}

GLfloat FrameBuffer::getPointSize()
{
    return parameters.pointSize;
}

GLfloat FrameBuffer::getPointFadeThresholdSize()
{
    return parameters.pointFadeThresholdSize;
}

bool FrameBuffer::getPointLowerLeftOrigin()
{
    return parameters.pointLowerLeftOrigin;
}

GLfloat FrameBuffer::getLineWidth()
{
    return parameters.lineWidth;
}

bool FrameBuffer::getLineSmooth()
{
    return parameters.lineSmooth;
}

bool FrameBuffer::getFrontFaceCW()
{
    return parameters.frontFaceCW;
}

vec2<PolygonMode> FrameBuffer::getPolygonMode()
{
    return vec2<PolygonMode>(parameters.polygonFront, parameters.polygonBack);
}

bool FrameBuffer::getPolygonSmooth()
{
    return parameters.polygonSmooth;
}

vec2<GLfloat> FrameBuffer::getPolygonOffset()
{
    return parameters.polygonOffset;
}

vec3<bool> FrameBuffer::getPolygonOffsets()
{
    return parameters.polygonOffsets;
}

bool FrameBuffer::getMultiSample()
{
    return parameters.multiSample;
}

vec2<bool> FrameBuffer::getSampleAlpha()
{
    return vec2<bool>(parameters.sampleAlphaToCoverage, parameters.sampleAlphaToOne);
}

GLfloat FrameBuffer::getSampleCoverage()
{
    return parameters.sampleCoverage;
}

GLint FrameBuffer::getSampleMask()
{
    return parameters.sampleMask;
}

bool FrameBuffer::getSampleShading(GLfloat &minSamples)
{
    minSamples = parameters.samplesMin;
    return parameters.sampleShading;
}

ptr<Query> FrameBuffer::getOcclusionTest(QueryMode &occlusionMode)
{
    occlusionMode = parameters.occlusionMode;
    return parameters.occlusionQuery;
}

bool FrameBuffer::getScissorTest()
{
    return parameters.enableScissor;
}

bool FrameBuffer::getScissorTest(vec4<GLint> &scissor)
{
    scissor = parameters.scissor;
    return parameters.enableScissor;
}

bool FrameBuffer::getStencilTest()
{
    return parameters.enableStencil;
}

bool FrameBuffer::getStencilTest(Function &ff, GLint &fref, GLint &fmask, StencilOperation &ffail, StencilOperation &fdpfail, StencilOperation &fdppass)
{
    ff = parameters.ffunc;
    fref = parameters.fref;
    fmask = parameters.fmask;
    ffail = parameters.ffail;
    fdpfail = parameters.fdpfail;
    fdppass = parameters.fdppass;

    return parameters.enableStencil;
}

bool FrameBuffer::getStencilTest(Function &ff, GLint &fref, GLint &fmask, StencilOperation &ffail, StencilOperation &fdpfail, StencilOperation &fdppass,
                                 Function &bf, GLint &bref, GLint &bmask, StencilOperation &bfail, StencilOperation &bdpfail, StencilOperation &bdppass)
{
    ff = parameters.ffunc;
    fref = parameters.fref;
    fmask = parameters.fmask;
    ffail = parameters.ffail;
    fdpfail = parameters.fdpfail;
    fdppass = parameters.fdppass;
    bf = parameters.bfunc;
    bref = parameters.bref;
    bmask = parameters.bmask;
    bfail = parameters.bfail;
    bdpfail = parameters.bdpfail;
    bdppass = parameters.bdppass;

    return parameters.enableStencil;
}

bool FrameBuffer::getDepthTest()
{
    return parameters.enableDepth;
}

bool FrameBuffer::getDepthTest(Function &depth)
{
    depth = parameters.depth;
    return parameters.enableDepth;
}

bool FrameBuffer::getBlend(BufferId buffer)
{
    int id = buffer < 0 ? 0 : getBufferId(buffer);
    return parameters.enableBlend[id];
}

bool FrameBuffer::getBlend(BufferId buffer, BlendEquation &rgb, BlendArgument &srgb, BlendArgument &drgb)
{
    int id = buffer < 0 ? 0 : getBufferId(buffer);
    rgb = parameters.rgb[id];
    srgb = parameters.srgb[id];
    drgb = parameters.drgb[id];
    return parameters.enableBlend[id];
}

bool FrameBuffer::getBlend(BufferId buffer, BlendEquation &rgb, BlendArgument &srgb, BlendArgument &drgb,
                                            BlendEquation &alpha, BlendArgument &salpha, BlendArgument &dalpha)
{
    int id = buffer < 0 ? 0 : getBufferId(buffer);
    rgb = parameters.rgb[id];
    srgb = parameters.srgb[id];
    drgb = parameters.drgb[id];
    alpha = parameters.alpha[id];
    salpha = parameters.salpha[id];
    dalpha = parameters.dalpha[id];
    return parameters.enableBlend[id];
}

vec4<GLfloat> FrameBuffer::getBlendColor()
{
    return parameters.color;
}

bool FrameBuffer::getDither()
{
    return parameters.enableDither;
}

bool FrameBuffer::getLogicOp()
{
    return parameters.enableLogic;
}

bool FrameBuffer::getLogicOp(LogicOperation &logicOp)
{
    logicOp = parameters.logicOp;
    return parameters.enableLogic;
}

vec4<bool> FrameBuffer::getColorMask(BufferId buffer)
{
    return parameters.colorMask[getBufferId(buffer)];
}

bool FrameBuffer::getDepthMask()
{
    return parameters.depthMask;
}

vec2<GLint> FrameBuffer::getStencilMask()
{
    return vec2<GLint>(parameters.stencilMaskFront, parameters.stencilMaskBack);
}

void FrameBuffer::setParameters(const Parameters &p)
{
    parameters = p;
    parametersChanged = true;
}

void FrameBuffer::setViewport(const vec4<GLint> &viewport)
{
    parameters.viewport = viewport;
    parameters.transformId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setDepthRange(GLfloat n, GLfloat f)
{
    parameters.depthRange = vec2<GLfloat>(n, f);
    parameters.transformId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setClipDistances(int clipDistances)
{
    parameters.clipDistances = clipDistances;
    parameters.transformId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setClearColor(const vec4<GLfloat> &clearColor)
{
    parameters.clearColor = clearColor;
    parameters.clearId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setClearDepth(GLfloat clearDepth)
{
    parameters.clearDepth = clearDepth;
    parameters.clearId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setClearStencil(GLint clearStencil)
{
    parameters.clearStencil = clearStencil;
    parameters.clearId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPointSize(GLfloat pointSize)
{
    parameters.pointSize = pointSize;
    parameters.pointId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPointFadeThresholdSize(GLfloat pointFadeThresholdSize)
{
    parameters.pointFadeThresholdSize = pointFadeThresholdSize;
    parameters.pointId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPointLowerLeftOrigin(bool pointLowerLeftOrigin)
{
    parameters.pointLowerLeftOrigin = pointLowerLeftOrigin;
    parameters.pointId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setLineWidth(GLfloat lineWidth)
{
    parameters.lineWidth = lineWidth;
    parametersChanged = true;
}

void FrameBuffer::setLineSmooth(bool lineSmooth)
{
    parameters.lineSmooth = lineSmooth;
    parametersChanged = true;
}

void FrameBuffer::setFrontFaceCW(bool frontFaceCW)
{
    parameters.frontFaceCW = frontFaceCW;
    parameters.polygonId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPolygonMode(PolygonMode polygonFront, PolygonMode polygonBack)
{
    parameters.polygonFront = polygonFront;
    parameters.polygonBack = polygonBack;
    parameters.polygonId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPolygonSmooth(bool polygonSmooth)
{
    parameters.polygonSmooth = polygonSmooth;
    parameters.polygonId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPolygonOffset(GLfloat factor, GLfloat units)
{
    parameters.polygonOffset = vec2<GLfloat>(factor, units);
    parameters.polygonId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setPolygonOffset(bool pointOffset, bool lineOffset, bool polygonOffset)
{
    parameters.polygonOffsets = vec3<bool>(pointOffset, lineOffset, polygonOffset);
    parameters.polygonId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setMultisample(bool multiSample)
{
    parameters.multiSample = multiSample;
    parameters.multiSampleId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setSampleAlpha(bool sampleAlphaToCoverage, bool sampleAlphaToOne)
{
    parameters.sampleAlphaToCoverage = sampleAlphaToCoverage;
    parameters.sampleAlphaToOne = sampleAlphaToOne;
    parameters.multiSampleId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setSampleCoverage(GLfloat sampleCoverage)
{
    parameters.sampleCoverage = sampleCoverage;
    parameters.multiSampleId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setSampleMask(GLbitfield sampleMask)
{
    parameters.sampleMask = sampleMask;
    parameters.multiSampleId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setSampleShading(bool sampleShading, GLfloat minSamples)
{
    parameters.sampleShading = sampleShading;
    parameters.samplesMin = minSamples;
    parameters.multiSampleId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setOcclusionTest(ptr<Query> occlusionQuery, QueryMode occlusionMode)
{
    assert(occlusionQuery == NULL ||
        occlusionQuery->getType() == SAMPLES_PASSED ||
        occlusionQuery->getType() == ANY_SAMPLES_PASSED);
    parameters.occlusionQuery = occlusionQuery;
    parameters.occlusionMode = occlusionMode;
    parametersChanged = true;
}

void FrameBuffer::setScissorTest(bool enableScissor)
{
    parameters.enableScissor = enableScissor;
    parametersChanged = true;
}

void FrameBuffer::setScissorTest(bool enableScissor, const vec4<GLint> &scissor)
{
    parameters.enableScissor = enableScissor;
    parameters.scissor = scissor;
    parametersChanged = true;
}

void FrameBuffer::setStencilTest(bool enableStencil)
{
    parameters.enableStencil = enableStencil;
    parameters.stencilId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setStencilTest(bool enableStencil,
    Function f, GLint ref, GLuint mask, StencilOperation sfail, StencilOperation dpfail, StencilOperation dppass)
{
    parameters.enableStencil = enableStencil;
    parameters.ffunc = f;
    parameters.fref = ref;
    parameters.fmask = mask;
    parameters.ffail = sfail;
    parameters.fdpfail = dpfail;
    parameters.fdppass = dppass;
    parameters.bfunc = f;
    parameters.bref = ref;
    parameters.bmask = mask;
    parameters.bfail = sfail;
    parameters.bdpfail = dpfail;
    parameters.bdppass = dppass;
    parameters.stencilId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setStencilTest(bool enableStencil,
    Function ff, GLint fref, GLuint fmask, StencilOperation ffail, StencilOperation fdpfail, StencilOperation fdppass,
    Function bf, GLint bref, GLuint bmask, StencilOperation bfail, StencilOperation bdpfail, StencilOperation bdppass)
{
    parameters.enableStencil = enableStencil;
    parameters.ffunc = ff;
    parameters.fref = fref;
    parameters.fmask = fmask;
    parameters.ffail = ffail;
    parameters.fdpfail = fdpfail;
    parameters.fdppass = fdppass;
    parameters.bfunc = bf;
    parameters.bref = bref;
    parameters.bmask = bmask;
    parameters.bfail = bfail;
    parameters.bdpfail = bdpfail;
    parameters.bdppass = bdppass;
    parameters.stencilId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setDepthTest(bool enableDepth)
{
    parameters.enableDepth = enableDepth;
    parametersChanged = true;
}

void FrameBuffer::setDepthTest(bool enableDepth, Function depth)
{
    parameters.enableDepth = enableDepth;
    parameters.depth = depth;
    parametersChanged = true;
}

void FrameBuffer::setBlend(bool enableBlend)
{
    parameters.multiBlendEnable = false;
    parameters.enableBlend[0] = enableBlend;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlend(BufferId buffer, bool enableBlend)
{
    parameters.multiBlendEnable = true;
    parameters.enableBlend[getBufferId(buffer)] = enableBlend;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlend(bool enableBlend,
    BlendEquation e, BlendArgument src, BlendArgument dst)
{
    parameters.multiBlendEnable = false;
    parameters.multiBlendEq = false;
    parameters.enableBlend[0] = enableBlend;
    parameters.rgb[0] = e;
    parameters.srgb[0] = src;
    parameters.drgb[0] = dst;
    parameters.alpha[0] = e;
    parameters.salpha[0] = src;
    parameters.dalpha[0] = dst;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlend(BufferId buffer, bool enableBlend,
    BlendEquation e, BlendArgument src, BlendArgument dst)
{
    int b = getBufferId(buffer);
    parameters.multiBlendEnable = false;
    parameters.multiBlendEq = false;
    parameters.enableBlend[b] = enableBlend;
    parameters.rgb[b] = e;
    parameters.srgb[b] = src;
    parameters.drgb[b] = dst;
    parameters.alpha[b] = e;
    parameters.salpha[b] = src;
    parameters.dalpha[b] = dst;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlend(bool enableBlend,
    BlendEquation rgb, BlendArgument srgb, BlendArgument drgb,
    BlendEquation alpha, BlendArgument salpha, BlendArgument dalpha)
{
    parameters.multiBlendEnable = false;
    parameters.multiBlendEq = false;
    parameters.enableBlend[0] = enableBlend;
    parameters.rgb[0] = rgb;
    parameters.srgb[0] = srgb;
    parameters.drgb[0] = drgb;
    parameters.alpha[0] = alpha;
    parameters.salpha[0] = salpha;
    parameters.dalpha[0] = dalpha;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlend(BufferId buffer, bool enableBlend,
    BlendEquation rgb, BlendArgument srgb, BlendArgument drgb,
    BlendEquation alpha, BlendArgument salpha, BlendArgument dalpha)
{
    int b = getBufferId(buffer);
    parameters.multiBlendEnable = true;
    parameters.multiBlendEq = true;
    parameters.enableBlend[b] = enableBlend;
    parameters.rgb[b] = rgb;
    parameters.srgb[b] = srgb;
    parameters.drgb[b] = drgb;
    parameters.alpha[b] = alpha;
    parameters.salpha[b] = salpha;
    parameters.dalpha[b] = dalpha;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setBlendColor(const vec4<GLfloat> &color)
{
    parameters.color = color;
    parameters.blendId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setDither(bool enableDither)
{
    parameters.enableDither = enableDither;
    parametersChanged = true;
}

void FrameBuffer::setLogicOp(bool enableLogic)
{
    parameters.enableLogic = enableLogic;
    parametersChanged = true;
}

void FrameBuffer::setLogicOp(bool enableLogic, LogicOperation logicOp)
{
    parameters.enableLogic = enableLogic;
    parameters.logicOp = logicOp;
    parametersChanged = true;
}

void FrameBuffer::setColorMask(bool r, bool g, bool b, bool a)
{
    parameters.multiColorMask = false;
    parameters.colorMask[0] = vec4<bool>(r, g, b, a);
    parameters.maskId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setColorMask(BufferId buffer, bool r, bool g, bool b, bool a)
{
    parameters.multiColorMask = true;
    parameters.colorMask[getBufferId(buffer)] = vec4<bool>(r, g, b, a);
    parameters.maskId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setDepthMask(bool d)
{
    parameters.depthMask = d;
    parameters.maskId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::setStencilMask(GLuint frontMask, GLuint backMask)
{
    parameters.stencilMaskFront = frontMask;
    parameters.stencilMaskBack = backMask;
    parameters.maskId = ++PARAMETER_ID;
    parametersChanged = true;
}

void FrameBuffer::clear(bool color, bool stencil, bool depth)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "Clear FrameBuffer");
    }
    set();
    int buffers = 0;
    if (color) {
        buffers |= GL_COLOR_BUFFER_BIT;
    }
    if (stencil) {
        buffers |= GL_STENCIL_BUFFER_BIT;
    }
    if (depth) {
        buffers |= GL_DEPTH_BUFFER_BIT;
    }
    beginConditionalRender();
    glClear(buffers);
    endConditionalRender();
    assert(getError() == 0);
}

void FrameBuffer::draw(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, GLint first, GLsizei count, GLsizei primCount, GLint base)
{
    assert(TransformFeedback::TRANSFORM == NULL);
    set();
    p->set();
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->logf("RENDER", "Draw Mesh (%d vertices)", count);
    }
    beginConditionalRender();
    mesh.draw(m, first, count, primCount, base);
    endConditionalRender();
}

void FrameBuffer::multiDraw(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint* bases)
{
    assert(TransformFeedback::TRANSFORM == NULL);
    set();
    p->set();
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->logf("RENDER", "MultiDraw (%d instances)", primCount);
    }
    beginConditionalRender();
    mesh.multiDraw(m, firsts, counts, primCount, bases);
    endConditionalRender();
}

void FrameBuffer::drawIndirect(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, const Buffer &buf)
{
    assert(TransformFeedback::TRANSFORM == NULL);
    set();
    p->set();
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "DrawIndirect");
    }
    beginConditionalRender();
    mesh.drawIndirect(m, buf);
    endConditionalRender();
}

void FrameBuffer::drawFeedback(ptr<Program> p, MeshMode m, const TransformFeedback &tfb, int stream)
{
    assert(TransformFeedback::TRANSFORM == NULL);
    set();
    p->set();
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "DrawFeedBack");
    }
    beginConditionalRender();
    glDrawTransformFeedbackStream(getMeshMode(m), tfb.id, stream);
    endConditionalRender();
}

void FrameBuffer::drawQuad(ptr<Program> p)
{
    if (QUAD == NULL) {
        Mesh<vec4f, unsigned int> *quad = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
        quad->addAttributeType(0, 4, A32F, false);
        quad->addVertex(vec4f(-1, -1, 0, 1));
        quad->addVertex(vec4f(1, -1, 0, 1));
        quad->addVertex(vec4f(-1, 1, 0, 1));
        quad->addVertex(vec4f(1, 1, 0, 1));
        QUAD = quad;
    }
    draw(p, *QUAD);
}

void FrameBuffer::readPixels(int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &dstBuf, bool clamp)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->logf("RENDER", "read %d pixels", w * h);
    }
    set();
    dstBuf.bind(GL_PIXEL_PACK_BUFFER);
    s.set();
    glClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);
    glReadPixels(x, y, w, h, getTextureFormat(f), getPixelType(t), dstBuf.data(0));
    s.unset();
    dstBuf.unbind(GL_PIXEL_PACK_BUFFER);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int x, int y, int w, const Texture1D &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage1D(GL_TEXTURE_1D, level, xoff, x, y, w);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int layer, int x, int y, int w, int d, const Texture1DArray &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage2D(GL_TEXTURE_1D, level, xoff, layer, x, y, w, d);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int x, int y, int w, int h, const Texture2D &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoff, yoff, x, y, w, h);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int layer, int x, int y, int w, int h, const Texture2DArray &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoff, yoff, layer, x, y, w, h);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int zoff, int x, int y, int w, int h, const Texture3D &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage3D(GL_TEXTURE_3D, level, xoff, yoff, zoff, x, y, w, h);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int x, int y, int w, int h, const TextureCube &dst, int level, CubeFace cf)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage2D(cf, level, xoff, yoff, x, y, w, h);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int layer, int x, int y, int w, int h, const TextureCubeArray &dst, int level, CubeFace cf)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage3D(cf, level, xoff, yoff, layer, x, y, w, h);
    assert(getError() == 0);
}

void FrameBuffer::copyPixels(int xoff, int yoff, int x, int y, int w, int h, const TextureRectangle &dst, int level)
{
    set();
    dst.bindToTextureUnit();
    glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE, level, xoff, yoff, x, y, w, h);
    assert(getError() == 0);
}

GLint FrameBuffer::getMajorVersion()
{
    GLint v;
    glGetIntegerv(GL_MAJOR_VERSION, &v);
    return v;
}

GLint FrameBuffer::getMinorVersion()
{
    GLint v;
    glGetIntegerv(GL_MINOR_VERSION, &v);
    return v;
}

GLenum FrameBuffer::getError()
{
    GLenum error = glGetError();
    if ((error != GL_NO_ERROR) && (Logger::ERROR_LOGGER != NULL)) {
        ostringstream oss;
        oss << "OpenGL error " << error << ", returned string '" << gluErrorString(error) << "'";
        Logger::ERROR_LOGGER->log("RENDER", oss.str());
        Logger::ERROR_LOGGER->flush();
    }
    if (error != GL_NO_ERROR) {
        printf("%s\n", gluErrorString(error));
    }
    return error;
}

void FrameBuffer::resetAllStates()
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "Reset GL STATES");
    }
    if (MeshBuffers::CURRENT != NULL) {
        MeshBuffers::CURRENT->reset();
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    FrameBuffer::CURRENT = NULL;
    Program::CURRENT = NULL;

    Texture::unbindAll();
}

void FrameBuffer::set()
{
    bool framebufferChanged = false;
    if (CURRENT != this) {
        if (Logger::DEBUG_LOGGER != NULL) {
            Logger::DEBUG_LOGGER->log("RENDER", "Changing Current Framebuffer");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
        CURRENT = this;
        framebufferChanged = true;
    }
    if (framebufferChanged || parametersChanged != 0) {
        PARAMETERS.set(parameters);
        parametersChanged = false;
    }
    if (framebufferId != 0 && attachmentsChanged) {
        setAttachments();
        checkAttachments();
        attachmentsChanged = false;
    }
    if (framebufferId != 0 && readDrawChanged) {
        glReadBuffer(getBuffer(readBuffer));
        if (drawBufferCount == 1) {
            glDrawBuffer(getBuffer(drawBuffers[0]));
        } else {
            GLenum drawBufs[4];
            for (int i = 0; i < drawBufferCount; ++i) {
                drawBufs[i] = getBuffer(drawBuffers[i]);
            }
            glDrawBuffers(drawBufferCount, drawBufs);
        }
        readDrawChanged = false;
        assert(getError() == 0);
    }
}

void FrameBuffer::setAttachments()
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("RENDER", "Setting Framebuffer attachments");
    }
    const int ATTACHMENTS[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
        GL_STENCIL_ATTACHMENT,
        GL_DEPTH_ATTACHMENT
    };

    for (int i = 0; i < 6; ++i) {
        if (textures[i] == NULL) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_RENDERBUFFER, 0);
            continue;
        }
        if (textures[i].cast<RenderBuffer>() != NULL) {
            int id = textures[i].cast<RenderBuffer>()->getId();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_RENDERBUFFER, id);
            continue;
        }

        int id = textures[i].cast<Texture>()->getId();
        if (textures[i].cast<Texture1DArray>() != NULL ||
            textures[i].cast<Texture2DArray>() != NULL ||
            textures[i].cast<Texture2DMultisampleArray>() != NULL ||
            textures[i].cast<Texture3D>() != NULL)
        {
            if (layers[i] == -1) {
                glFramebufferTexture(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i]);
            } else {
                glFramebufferTextureLayer(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i], layers[i]);
            }
        } else if (textures[i].cast<Texture1D>() != NULL) {
            glFramebufferTexture1D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_1D, id, levels[i]);
        } else if (textures[i].cast<Texture2D>() != NULL) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_2D, id, levels[i]);
        } else if (textures[i].cast<TextureRectangle>() != NULL) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_RECTANGLE, id, levels[i]);
        } else if (textures[i].cast<Texture2DMultisample>() != NULL) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], GL_TEXTURE_2D_MULTISAMPLE, id, levels[i]);
        } else if (textures[i].cast<TextureCube>() != NULL) {
            if (layers[i] == -1) {
                glFramebufferTexture(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i]);
            } else {
                glFramebufferTexture2D(GL_FRAMEBUFFER, ATTACHMENTS[i], getCubeFace((CubeFace) layers[i]), id, levels[i]);
            }
        } else if (textures[i].cast<TextureCubeArray>() != NULL) {
            glFramebufferTextureLayer(GL_FRAMEBUFFER, ATTACHMENTS[i], id, levels[i], layers[i]);
        }
        assert(getError() == 0);
    }
}

void FrameBuffer::checkAttachments()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE:
        return;
    case GL_FRAMEBUFFER_UNDEFINED:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: undefined");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete attachement");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete missing attachement");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete draw buffer");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete read buffer");
        }
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: unsupported");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete multisample");
        }
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("OPENGL", "Framebuffer object: incomplete layer targets");
        }
        break;
    default:
        assert(false);
        break;
    }
    assert(false);
}

void FrameBuffer::beginConditionalRender()
{
    if (parameters.occlusionQuery != NULL) {
        glBeginConditionalRender(parameters.occlusionQuery->getId(), getQueryMode(parameters.occlusionMode));
    }
}

void FrameBuffer::endConditionalRender()
{
    if (parameters.occlusionQuery != NULL) {
        glEndConditionalRender();
    }
}

GLenum FrameBuffer::getBuffer(BufferId b) const
{
    switch (b & (COLOR0 | COLOR1 | COLOR2 | COLOR3)) {
    case 0:
        return GL_NONE;
    case COLOR0:
        return framebufferId == 0 ? GL_FRONT_LEFT : GL_COLOR_ATTACHMENT0;
    case COLOR1:
        return framebufferId == 0 ? GL_FRONT_RIGHT : GL_COLOR_ATTACHMENT1;
    case COLOR2:
        return framebufferId == 0 ? GL_BACK_LEFT : GL_COLOR_ATTACHMENT2;
    case COLOR3:
        return framebufferId == 0 ? GL_BACK_RIGHT : GL_COLOR_ATTACHMENT3;
    }
    assert(false);
    throw exception();
}

static_ptr<FrameBuffer::FrameBufferMap> FrameBuffer::DEFAULT(new FrameBuffer::FrameBufferMap());

FrameBuffer* FrameBuffer::CURRENT = NULL;

FrameBuffer::Parameters FrameBuffer::PARAMETERS;

static_ptr< Mesh<vec4f, unsigned int> > FrameBuffer::QUAD;

}
