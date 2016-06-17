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

#ifndef _ORK_FRAME_BUFFER_H_
#define _ORK_FRAME_BUFFER_H_

#include <vector>
#include <map>

#include "ork/math/vec4.h"
#include "ork/render/Mesh.h"
#include "ork/render/Program.h"
#include "ork/render/Query.h"
#include "ork/render/RenderBuffer.h"
#include "ork/render/Texture1D.h"
#include "ork/render/Texture1DArray.h"
#include "ork/render/Texture2D.h"
#include "ork/render/Texture2DArray.h"
#include "ork/render/Texture2DMultisample.h"
#include "ork/render/Texture2DMultisampleArray.h"
#include "ork/render/Texture3D.h"
#include "ork/render/TextureBuffer.h"
#include "ork/render/TextureCube.h"
#include "ork/render/TextureCubeArray.h"
#include "ork/render/TextureRectangle.h"
#include "ork/render/TransformFeedback.h"

namespace ork
{

/**
 * @defgroup render render
 * @ingroup ork
 * Provides the %core classes for 3D rendering.
 */

/**
 * A framebuffer, either the default one or a frame buffer object. Each
 * framebuffer has its own state, made of attachments and fixed functions
 * parameters.
 *
 * @ingroup render
 */
class ORK_API FrameBuffer : public Object
{
public:
    /**
     * The state of a FrameBuffer.
     */
    class ORK_API Parameters
    {
    public:
        /**
         * Creates a new framebuffer state with default parameter values.
         */
        Parameters();

    private:
        /**
         * True if several viewports are specified.
         */
        bool multiViewports;

        /**
         * The viewport that defines the destination area for FrameBuffer#draw.
         * This value is specific to this framebuffer instance and is
         * automatically updated when the framebuffer is activated with
         * FrameBuffer##set.
         * (This corresponds to up, down, left and right planes).
         */
        vec4<GLint> viewport;

        /**
         * The viewports that define the destination areas for FrameBuffer#draw.
         * This value is specific to this framebuffer instance and is
         * automatically updated when the framebuffer is activated with
         * FrameBuffer##set.
         * (This corresponds to up, down, left and right planes).
         */
        vec4<GLfloat> viewports[16];

        /**
         * The depth range that defines the destination area for #draw.
         * Contains far and near planes.
         */
        vec2<GLfloat> depthRange;

        /**
         * The depth ranges that define the destination areas for #draw.
         * Contains far and near planes.
         */
        vec2<GLdouble> depthRanges[16];

        /**
         * Defines which planes must be used for clipping tests.
         * Each bit of clipDistances corresponds to a given plane.
         * Default is all activated.
         */
        int clipDistances;

        /**
         * A unique Id incremented each time multiViewports, viewport,
         * viewports, depthRange depthRanges or clipDistances change.
         */
        int transformId;

        // -------------

        /**
         * The color used to refill the framebuffer when calling glClear.
         */
        vec4<GLfloat> clearColor;

        /**
         * The depth value used to refill the depthbuffer when calling glClear.
         */
        GLfloat clearDepth;

        /**
         * The stencil value used to refill the stencilbuffer when calling glClear.
         */
        GLint clearStencil;

        /**
         * A unique ID incremented each time clearColor, clearDepth or clearStencil
         * change.
         */
        int clearId;

        // -------------

        /**
         * Defines the size of a drawn 'point' primitive.
         */
        GLfloat pointSize; // <= 0.0 means controlled by program

        /**
         * Defines the size above which point sizes are clamped.
         */
        GLfloat pointFadeThresholdSize;

        /**
         * Defines the origin of a drawn point primitive.
         * If true, will be the lower left corner.
         * Otherwise, will be the upper left corner. Default is false.
         */
        bool pointLowerLeftOrigin;

        /**
         * A unique ID incremented each time pointSize, pointFadeThresholdSize or
         * pointLowerLeftOrigin change.
         */
        int pointId;

        // -------------

        /**
         * Defines rasterized width of both aliased and antialiased lines.
         */
        GLfloat lineWidth;

        /**
         * If true, antialiasing will be enabled when drawing lines.
         */
        bool lineSmooth;

        // -------------

        /**
         * Specifies the orientation of front-facing polygons.
         * If false, will be Counter-clockwise. Clockwise otherwise.
         */
        bool frontFaceCW;

        /**
         * Specifies how front-facing polygons will be rasterized.
         */
        PolygonMode polygonFront;

        /**
         * Specifies how back-facing polygons will be rasterized.
         */
        PolygonMode polygonBack;

        /**
         * If true, antialiasing will be enabled when drawing polygons.
         */
        bool polygonSmooth;

        /**
         * The scale and units used to calculate depth values.
         * The x coordinate specifies a scale factor that is used
         * to create a variable depth offset for each polygon.
         * The y coordinate is multiplied by an implementation-specific
         * value to create a constant depth offset.
         */
        vec2<GLfloat> polygonOffset;

        /**
         * Determiens if fragment's depth value will be offset after it is
         * interpolated from the depth values of the appropriate vertices.
         * x is for points, y for lines and z for polygons.
         */
        vec3<bool> polygonOffsets;

        /**
         * A unique ID incremented each time frontFaceCW, polygonFront, polygonBack
         * polygonSmooth, polygonOffset or polygonOffsets change.
         */
        int polygonId;

        // -------------

        /**
         * If enabled, use multiple fragment samples in computing the final
         * color of a pixel.
         */
        bool multiSample;

        /**
         * If enabled, computes a temporary coverage value where each bit is
         * determined by the alpha value at the corresponding sample location.
         * The temporary coverage value is then ANDed with the fragment
         * coverage value.
         */
        bool sampleAlphaToCoverage;

        /**
         * If enabled, each sample alpha value is replaced by the maximum
         * representable alpha value.
         */
        bool sampleAlphaToOne;

        /**
         * If enabled, the fragment's coverage is ANDed with the temporary
         * coverage value. If set to 1.0f, will disable sampleCoverage.
         * Negative values will invert coverage value.
         */
        GLfloat sampleCoverage;

        /**
         * Used to change the coverage of a sample or to exclude some samples
         * from further fragment processing. Will never enable uncovered samples.
         */
        GLbitfield sampleMask; // 0xFFFFFFFF

        /**
         * If enabled, this will explicitly request that an implementation uses
         * a minimum number of unique set of fragment computation inputs when
         * multisampling a pixel (i.e. different samples). This is used to avoid
         * aliasing.
         */
        bool sampleShading;

        /**
         * Minimum number of unique set of fragment computation inputs when
         * multisampling a pixel (i.e. different samples). Only used if
         * #sampleShading is enabled.
         */
        GLfloat samplesMin;

        /**
         * A unique ID incremented each time multiSample, sampleAlphaToCoverage, sampleAlphaToOne
         * sampleCoverage, sampleMask, sampleShading or samplesMin change.
         */
        int multiSampleId;

        // -------------

        /**
         * Occlusion queries use query objects to track the number of fragments
         * or samples that pass the depth test.
         */
        ptr<Query> occlusionQuery;

        /**
         * Defines the comparison mode used for the depth tests when using occlusion
         * queries.
         */
        QueryMode occlusionMode;

        // -------------

        /**
         * True if separate scissor tests are used for each viewport.
         */
        bool multiScissor;

        /**
         * If enabled, only the fragments inside #scissor will not be discarded.
         * If disabled, the scissor test always passes.
         */
        bool enableScissor[16];

        /**
         * The viewports of the scissor test.
         */
        vec4<GLint> scissor[16];

        /**
         * A unique ID incremented each time multiScissor, enableScissor, or scissor change.
         */
        int scissorId;

        // -------------

        /**
         * If enabled, the stencil test conditionnaly discards a fragment based on
         * the outcome of a comparison between the value in the stencil buffer at
         * the fragment's position and a reference value. Different test functions
         * may be used, and front-faces and back-faces are treated separatly.
         */
        bool enableStencil;

        /**
         * The stencil test function for front faces.
         */
        Function ffunc;

        /**
         * The stencil reference value for stencil tests of front faces.
         */
        GLint fref;

        /**
         * The stencil mask value for stencil tests of front faces.
         */
        GLuint fmask;

        /**
         * The action to use when the stencil test fails for front faces.
         */
        StencilOperation ffail;

        /**
         * The action to use when the depth test fails for front faces.
         */
        StencilOperation fdpfail;

        /**
         * The action to use when the depth test passes for front faces.
         */
        StencilOperation fdppass;

        /**
         * The stencil test function for back faces.
         */
        Function bfunc;

        /**
         * The stencil reference value for stencil tests of back faces.
         */
        GLint bref;

        /**
         * The stencil mask value for stencil tests of back faces.
         */
        GLuint bmask;

        /**
         * The action to use when the stencil test fails for back faces.
         */
        StencilOperation bfail;

        /**
         * The action to use when the depth test fails for back faces.
         */
        StencilOperation bdpfail;

        /**
         * The action to use when the depth test passes for back faces.
         */
        StencilOperation bdppass;

        /**
         * A unique ID incremented each time enableStencil, ffunc, fref, fmask, ffail, fdpfail, fdppass,
         * bfunc, bref, bmask, bfail, bdpfail, bdppass change.
         */
        int stencilId;

        // -------------

        /**
         * If enabled, the depth buffer test discards the incoming fragment
         * if a depth comparison fails. The comparison is specified with #depth.
         */
        bool enableDepth;

        /**
         * Depth comparison function.
         */
        Function depth;

        // -------------

        /**
         * If enabled, and if multiple draw buffers are set, blending will be
         *  enabled for all of them.
         */
        bool multiBlendEnable;

        /**
         * If enabled, and if multiple draw buffers are set, different blending
         * equations will be used.
         */
        bool multiBlendEq;

        /**
         * Determines for which draw buffers the blending should be enabled.
         */
        bool enableBlend[4];

        /**
         * The blending equation to use for color channels.
         */
        BlendEquation rgb[4];

        /**
         * The source color to be blended.
         */
        BlendArgument srgb[4];

        /**
         * The destination color with which the source color must be blended.
         */
        BlendArgument drgb[4];

        /**
         * The blending equation to use for alpha channel.
         */
        BlendEquation alpha[4];

        /**
         * The source alpha to be blended.
         */
        BlendArgument salpha[4];

        /**
         * The destination alpha with which the source alpha must be blended.
         */
        BlendArgument dalpha[4];

        /**
         * The color used in the blend equations. Only used if any of the equation has CONSTANT_COLOR inside it.
         */
        vec4<GLfloat> color;

        /**
         * A unique ID incremented each time multiBlendEnable, multiBlendeq, enableBlend, rgb, srgb, drgb,
         * alpha, salpha or dalpha change.
         */
        int blendId;

        // -------------

        /**
         * If enabled, dithering selects between two representable color values or indices.
         * A representable value is a value that has an exact representation in the color buffer.
         * Dithering selects, for each color component, either the largest representable color
         * value (for that particular color component) that is less than or equal to the incoming
         * color component value, c, or the smallest representable color value that is greater
         * than or equal to c.
         */
        bool enableDither;

        // -------------

        /**
         * If enabled, a logic operation is applied between the incoming fragment's color value
         * and the color values stored at the corresponding location in the framebuffer.
         */
        bool enableLogic;

        /**
         * The logical operation used if logical operations are enabled.
         */
        LogicOperation logicOp;

        // -------------

        /**
         * If enabled, each draw buffer will have its own color mask. Color mask will determine
         * which color(s) may be written to.
         */
        bool multiColorMask;

        /**
         * The color masks for each draw buffers.
         */
        vec4<bool> colorMask[4];

        /**
         * If enabled, depth buffer can be written to.
         */
        bool depthMask;

        /**
         * If enabled, stencil mask can be written to for front faces.
         */
        GLuint stencilMaskFront;

        /**
         * If enabled, stencil mask can be written to for back faces.
         */
        GLuint stencilMaskBack;

        /**
         * A unique ID incremented each time multiColorMask, colorMask, depthMask,
         * stencilMaskFront or stencilMaskBack change.
         */
        int maskId;

        /**
         * Sets these parameters to the values of the given parameters and
         * updates the corresponding OpenGL state for the parameters that
         * are different in the two sets.
         */
        void set(const Parameters &p);

        friend class FrameBuffer;
    };

    /**
     * Creates a new frambuffer. This creates an offsreen framebuffer. The
     * default framebuffer can be retrieved with #getDefault.
     */
    FrameBuffer();

    /**
     * Deletes this framebuffer.
     */
    virtual ~FrameBuffer();

    /**
     * Returns the default, onscreen framebuffer.
     */
    static ptr<FrameBuffer> getDefault();

    /**
     * Returns the render buffer attached to the given attachment point.
     *
     * @return the render buffer attached to the given attachment point,
     *       or NULL if there is no renderbuffer bound to this attachment
     *       point (either because nothing is attached, or because a texture
     *       is attached).
     */
    ptr<RenderBuffer> getRenderBuffer(BufferId b);

    /**
     * Returns the texture attached to the given attachment point.
     *
     * @return the texture attached to the given attachment point,
     *       or NULL if there is no texture bound to this attachment
     *       point (either because nothing is attached, or because a render
     *       buffer is attached).
     */
    ptr<Texture> getTextureBuffer(BufferId b);

    /**
     * Sets an attachment of this framebuffer using a render buffer.
     *
     * @param b an attachment point.
     * @param t the render buffer to be bound to this attachment point.
     */
    void setRenderBuffer(BufferId b, const ptr<RenderBuffer> t);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     */
    void setTextureBuffer(BufferId b, const ptr<Texture1D> t, int level);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     * @param layer the layer to be attached (-1 to attach all layers).
     */
    void setTextureBuffer(BufferId b, const ptr<Texture1DArray> t, int level, int layer);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     */
    void setTextureBuffer(BufferId b, const ptr<Texture2D> t, int level);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     */
    void setTextureBuffer(BufferId b, const ptr<TextureRectangle> t, int level);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     */
    void setTextureBuffer(BufferId b, const ptr<Texture2DMultisample> t);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     * @param layer the layer to be attached (-1 to attach all layers).
     */
    void setTextureBuffer(BufferId b, const ptr<Texture2DArray> t, int level, int layer);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param layer the layer to be attached (-1 to attach all layers).
     */
    void setTextureBuffer(BufferId b, const ptr<Texture2DMultisampleArray> t, int layer);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     * @param layer the z slice to be attached (-1 to attach all the slices).
     */
    void setTextureBuffer(BufferId b, const ptr<Texture3D> t, int level, int layer);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     * @param cf the cube face to be attached (-1 to attach all the faces).
     */
    void setTextureBuffer(BufferId b, const ptr<TextureCube> t, int level, CubeFace cf);

    /**
     * Sets an attachment of this framebuffer using a texture.
     *
     * @param b an attachment point.
     * @param t the texture to be bound to this attachment point.
     * @param level the LOD level to be attached.
     * @param cf the cube face to be attached (all the layers for this face
     *      will be attached at once).
     */
    void setTextureBuffer(BufferId b, const ptr<TextureCubeArray> t, int level, CubeFace cf);

    /**
     * Sets the read buffer for #readPixels and #copyPixels methods.
     *
     * @param b one of the color buffer.
     */
    void setReadBuffer(BufferId b);

    /**
     * Sets the draw buffer for #clear and #draw methods.
     *
     * @param b one of the color buffer.
     */
    void setDrawBuffer(BufferId b);

    /**
     * Sets the draw buffers for #clear and #draw methods.
     *
     * @param b a set of color buffers (ORed with each other).
     */
    void setDrawBuffers(BufferId b);

    /**
     * Returns the current parameters of this framebuffer.
     */
    Parameters getParameters();

    /**
     * Returns this framebuffer's viewport.
     */
    vec4<GLint> getViewport();

    /**
     * Returns the viewport of this framebuffer whose index is given.
     */
    vec4<GLfloat> getViewport(int index);

    /**
     * Returns this framebuffer's depth range.
     */
    vec2<GLfloat> getDepthRange();

    /**
     * Returns the depth range of this framebuffer whose index is given.
     */
    vec2<GLdouble> getDepthRange(int index);

    /**
     * Returns this framebuffer's clip distances mask.
     */
    GLint getClipDistances();

    /**
     * Returns the value used to clear the color buffer.
     */
    vec4<GLfloat> getClearColor();

    /**
     * Returns the value used to clear the depth buffer.
     */
    GLfloat getClearDepth();

    /**
     * Returns the value used to clear the stencil buffer.
     */
    GLint getClearStencil();

    /**
     * Returns the point primitive's size.
     */
    GLfloat getPointSize();

    /**
     * Returns the point's fade threshold size.
     */
    GLfloat getPointFadeThresholdSize();

    /**
     * Returns the left origin of the point primitive.
     */
    bool getPointLowerLeftOrigin();

    /**
     * Returns the line primitive's width.
     */
    GLfloat getLineWidth();

    /**
     * Retursn true if AA is enabled for lines.
     */
    bool getLineSmooth();

    /**
     * Returns true if front faces are clockwise.
     */
    bool getFrontFaceCW();

    /**
     * Returns the polygon mode for front and back faces cull state.
     */
    vec2<PolygonMode> getPolygonMode();

    /**
     * Returns true if AA is enabled for polygons.
     */
    bool getPolygonSmooth();

    /**
     * Returns factor and units used to offset the depth value.
     */
    vec2<GLfloat> getPolygonOffset();

    /**
     * Returns the types of primitives that must be offset (points, lines, and polygons).
     */
    vec3<bool> getPolygonOffsets();

    /**
     * Returns true if multisampling is enabled.
     */
    bool getMultiSample();

    /**
     * Returns the values for alpha sampling.
     */
    vec2<bool> getSampleAlpha();

    /**
     * Returns the sample coverage.
     */
    GLfloat getSampleCoverage();

    /**
     * Returns the sample mask.
     */
    GLint getSampleMask();

    /**
     * Returns true if AA is enabled on multi-sampling.
     */
    bool getSampleShading(GLfloat &minsamples);

    /**
     * Returns the occlusion test and its mode.
     */
    ptr<Query> getOcclusionTest(QueryMode &occlusionMode);

    /**
     * Returns True if the scissor test is enabled.
     */
    bool getScissorTest();

    /**
     * Returns True if the scissor test is enabled for the given viewport.
     */
    bool getScissorTest(int index);

    /**
     * Returns true if the scissor test is enabled.
     *
     * @param[out] scissor the current scissor test viewport.
     */
    bool getScissorTest(vec4<GLint> &scissor);

    /**
     * Returns true if the scissor test is enabled for the given viewport.
     *
     * @param index a viewport index.
     * @param[out] scissor the current scissor test viewport.
     */
    bool getScissorTest(int index, vec4<GLint> &scissor);

    /**
     * Returns true if stencil test is enabled.
     */
    bool getStencilTest();

    /**
     * Returns true if stencil test is enabled.
     *
     * @param[out] ff the current front face function.
     * @param[out] fref the current front face reference value.
     * @param[out] fmask the current front face stencil mask.
     * @param[out] ffail the current stencil operation used when failing stencil test on front faces.
     * @param[out] fdpfail the current stencil operation used when passing stencil test but failing depth test on front faces.
     * @param[out] fdppass the current stencil operation used when passing both stencil and depth tests on front faces.
     */
    bool getStencilTest(Function &ff, GLint &fref, GLint &fmask, StencilOperation &ffail, StencilOperation &fdpfail, StencilOperation &fdppass);

    /**
     * Returns true if stencil test is enabled.
     *
     * @param[out] ff the current front face function.
     * @param[out] fref the current front face reference value.
     * @param[out] fmask the current front face stencil mask.
     * @param[out] ffail the current stencil operation used when failing stencil test on front faces.
     * @param[out] fdpfail the current stencil operation used when passing stencil test but failing depth test on front faces.
     * @param[out] fdppass the current stencil operation used when passing both stencil and depth tests on front faces.
     * @param[out] bf the current back face function.
     * @param[out] bref the current back face reference value.
     * @param[out] bmask the current back face stencil mask.
     * @param[out] bfail the current stencil operation used when failing stencil test on back faces.
     * @param[out] bdpfail the current stencil operation used when passing stencil test but failing depth test on back faces.
     * @param[out] bdppass the current stencil operation used when passing both stencil and depth tests on back faces.
     */
    bool getStencilTest(Function &ff, GLint &fref, GLint &fmask, StencilOperation &ffail, StencilOperation &fdpfail, StencilOperation &fdppass,
                        Function &bf, GLint &bref, GLint &bmask, StencilOperation &bfail, StencilOperation &bdpfail, StencilOperation &bdppass);

    /**
     * Returns true if depth test is enabled.
     */
    bool getDepthTest();

    /**
     * Returns true if depth test is enabled.
     *
     * @param[out] depth the current depth test function.
     */
    bool getDepthTest(Function &depth);

    /**
     * Returns true if blending is enabled for specified buffer.
     * If no buffer is specified, will use default buffer.
     *
     * @param buffer an optionnal buffer id.
     */
    bool getBlend(BufferId buffer = BufferId(-1));

    /**
     * Returns true if blending is enabled for specified buffer.
     * If no buffer is specified, will use default buffer.
     *
     * @param buffer an optionnal buffer id.
     * @param[out] rgb the current color blending equation.
     * @param[out] srgb the current source color blending argument.
     * @param[out] drgb the current destination color blending argument.
     */
    bool getBlend(BufferId buffer, BlendEquation &rgb, BlendArgument &srgb, BlendArgument &drgb);

    /**
     * Returns true if blending is enabled for specified buffer.
     * If no buffer is specified, will use default buffer.
     *
     * @param buffer an optionnal buffer id.
     * @param[out] rgb the current color blending equation.
     * @param[out] srgb the current source color blending argument.
     * @param[out] drgb the current destination color blending argument.
     * @param[out] alpha the current alpha blending equation.
     * @param[out] salpha the current source alpha blending argument.
     * @param[out] dalpha the current destination alpha blending argument.
     */
    bool getBlend(BufferId buffer, BlendEquation &rgb, BlendArgument &srgb, BlendArgument &drgb,
                                   BlendEquation &alpha, BlendArgument &salpha, BlendArgument &dalpha);

    /**
     * Returns the current blending color parameter.
     */
    vec4<GLfloat> getBlendColor();

    /**
     * Returns true if dithering is enabled.
     */
    bool getDither();

    /**
     * Returns true if logical operation is enabled.
     */
    bool getLogicOp();

    /**
     * Returns true if logical operation is enabled.
     *
     * @param logicOp the current logical operation.
     */
    bool getLogicOp(LogicOperation &logicOp);

    /**
     * Returns the color writing mask for the given buffer.
     */
    vec4<bool> getColorMask(BufferId buffer = BufferId(-1));

    /**
     * Returns the depth buffer writing mask.
     */
    bool getDepthMask();

    /**
     * Returns the stencil buffer writing mask.
     */
    vec2<GLint> getStencilMask();

    /**
     * Sets all the parameters of this framebuffer at once.
     *
     * @param p a set of framebuffer parameters obtained via #getParameters().
     */
    void setParameters(const Parameters &p);

    /**
     * Sets the viewport for this framebuffer (up, down, left and right planes).
     *
     * @param viewport the new viewport.
     */
    void setViewport(const vec4<GLint> &viewport);

    /**
     * Sets a viewport for this framebuffer (up, down, left and right planes).
     *
     * @param index the viewport index.
     * @param viewport the new viewport.
     */
    void setViewport(int index, const vec4<GLfloat> &viewport);

    /**
     * Sets the depth range for this framebuffer (near and far planes).
     *
     * @param n near plane.
     * @param f far plane.
     */
    void setDepthRange(GLfloat n, GLfloat f);

    /**
     * Sets a depth range for this framebuffer (near and far planes).
     *
     * @param index the viewport index.
     * @param n near plane.
     * @param f far plane.
     */
    void setDepthRange(int index, GLdouble n, GLdouble f);

    /**
     * Sets the clipping bit, used to determine which planes will be used for clipping.
     */
    void setClipDistances(int clipDistances);

    /**
     * Sets the color used to clear the current draw buffer.
     */
    void setClearColor(const vec4<GLfloat> &clearColor);

    /**
     * Sets the depth used to clear the current depth buffer.
     */
    void setClearDepth(GLfloat clearDepth);

    /**
     * Sets the stencil used to clear the current stencil buffer.
     */
    void setClearStencil(GLint clearStencil);

    /**
     * Sets the point primitive's size.
     */
    void setPointSize(GLfloat pointSize);

    /**
     * Sets the maximum size of a point.
     */
    void setPointFadeThresholdSize(GLfloat pointFadeThresholdSize);

    /**
     * Sets the origin corner of a point.
     */
    void setPointLowerLeftOrigin(bool pointLowerLeftOrigin);

    /**
     * Sets the line primitive's width.
     */
    void setLineWidth(GLfloat lineWidth);

    /**
     * Enables or disables the AA on line drawing.
     */
    void setLineSmooth(bool lineSmooth);

    /**
     * Determines the orientation of front faces.
     *
     * @param frontFaceCW if true, clockwise faces will be front faces.
     */
    void setFrontFaceCW(bool frontFaceCW);

    /**
     * Sets the polygon mode for front and back faces.
     *
     * @param polygonFront drawing mode for front faces.
     * @param polygonBack drawing mode for back faces.
     */
    void setPolygonMode(PolygonMode polygonFront, PolygonMode polygonBack);

    /**
     * Enables or disables AA on polygon drawing.
     */
    void setPolygonSmooth(bool polygonSmooth);

    /**
     * Sets the factor and units when using offsets on primitives.
     */
    void setPolygonOffset(GLfloat factor, GLfloat units);

    /**
     * Determines which primitives should be offset.
     */
    void setPolygonOffset(bool pointOffset, bool lineOffset, bool polygonOffset);

    /**
     * Enables or disables multisampling.
     */
    void setMultisample(bool multiSample);

    /**
     * Sets sampling filter options.
     */
    void setSampleAlpha(bool sampleAlphaToCoverage, bool sampleAlphaToOne);

    /**
     * Sets sampling coverage.
     */
    void setSampleCoverage(GLfloat sampleCoverage);

    /**
     * Sets sampling mask.
     */
    void setSampleMask(GLbitfield sampleMask);

    /**
     * Enables or disables AA on multisampling.
     */
    void setSampleShading(bool sampleShading, GLfloat minSamples);

    /**
     * Adds an occlusion query.
     */
    void setOcclusionTest(ptr<Query> occlusionQuery, QueryMode occlusionMode);

    /**
     * Enables or disables scissor test.
     */
    void setScissorTest(bool enableScissor);

    /**
     * Enables or disables scissor test for the given viewport.
     */
    void setScissorTest(int index, bool enableScissor);

    /**
     * Enables or disables scissor test.
     */
    void setScissorTest(bool enableScissor, const vec4<GLint> &scissor);

    /**
     * Enables or disables scissor test for the given viewport.
     */
    void setScissorTest(int index, bool enableScissor, const vec4<GLint> &scissor);

    /**
     * Enables or disables stencil test.
     *
     * @param enableStencil true to enable the stencil test.
     */
    void setStencilTest(bool enableStencil);

    /**
     * Enables or disables stencil test.
     *
     * @param enableStencil true to enable the stencil test.
     * @param f the front and back face function.
     * @param ref the front and back face reference value.
     * @param mask the front and back face stencil mask.
     * @param sfail the stencil operation used when failing stencil test on front or back faces.
     * @param dpfail the stencil operation used when passing stencil test but failing depth test on front or back faces.
     * @param dppass the stencil operation used when passing both stencil and depth tests on front or back faces.
     */
    void setStencilTest(bool enableStencil,
        Function f, GLint ref, GLuint mask, StencilOperation sfail, StencilOperation dpfail, StencilOperation dppass);

    /**
     * Enables or disables stencil test.
     *
     * @param enableStencil true to enable the stencil test.
     * @param ff the front face function.
     * @param fref the front face reference value.
     * @param fmask the front face stencil mask.
     * @param ffail the stencil operation used when failing stencil test on front faces.
     * @param fdpfail the stencil operation used when passing stencil test but failing depth test on front faces.
     * @param fdppass the stencil operation used when passing both stencil and depth tests on front faces.
     * @param bf the back face function.
     * @param bref the back face reference value.
     * @param bmask the back face stencil mask.
     * @param bfail the stencil operation used when failing stencil test on back faces.
     * @param bdpfail the stencil operation used when passing stencil test but failing depth test on back faces.
     * @param bdppass the stencil operation used when passing both stencil and depth tests on back faces.
     */
    void setStencilTest(bool enableStencil,
        Function ff, GLint fref, GLuint fmask, StencilOperation ffail, StencilOperation fdpfail, StencilOperation fdppass,
        Function bf, GLint bref, GLuint bmask, StencilOperation bfail, StencilOperation bdpfail, StencilOperation bdppass);

    /**
     * Enables or disables depth test.
     */
    void setDepthTest(bool enableDepth);

    /**
     * Enables or disables depth test.
     */
    void setDepthTest(bool enableDepth, Function depth);

    /**
     * Enables or disables blending.
     */
    void setBlend(bool enableBlend);

    /**
     * Enables or disables blending for a given buffer.
     */
    void setBlend(BufferId buffer, bool enableBlend);

    /**
     * Enables or disables blending.
     *
     * @param enableBlend true to enable blending.
     * @param e the color and alpha blending equation.
     * @param src the source color and alpha blending argument.
     * @param dst the destination color and alpha blending argument.
     */
    void setBlend(bool enableBlend,
        BlendEquation e, BlendArgument src, BlendArgument dst);

    /**
     * Enables or disables blending for a given buffer.
     *
     * @param buffer the buffer whose blending options must be changed.
     * @param enableBlend true to enable blending.
     * @param e the color and alpha blending equation.
     * @param src the source color and alpha blending argument.
     * @param dst the destination color and alpha blending argument.
     */
    void setBlend(BufferId buffer, bool enableBlend,
        BlendEquation e, BlendArgument src, BlendArgument dst);

    /**
     * Enables or disables blending.
     *
     * @param enableBlend true to enable blending.
     * @param rgb the color blending equation.
     * @param srgb the source color blending argument.
     * @param drgb the destination color blending argument.
     * @param alpha the alpha blending equation.
     * @param salpha the source alpha blending argument.
     * @param dalpha the destination alpha blending argument.
     */
    void setBlend(bool enableBlend,
        BlendEquation rgb, BlendArgument srgb, BlendArgument drgb,
        BlendEquation alpha, BlendArgument salpha, BlendArgument dalpha);

    /**
     * Enables or disables blending for a given buffer.
     *
     * @param buffer the buffer whose blending options must be changed.
     * @param enableBlend true to enable blending.
     * @param rgb the color blending equation.
     * @param srgb the source color blending argument.
     * @param drgb the destination color blending argument.
     * @param alpha the alpha blending equation.
     * @param salpha the source alpha blending argument.
     * @param dalpha the destination alpha blending argument.
     */
    void setBlend(BufferId buffer, bool enableBlend,
        BlendEquation rgb, BlendArgument srgb, BlendArgument drgb,
        BlendEquation alpha, BlendArgument salpha, BlendArgument dalpha);

    /**
     * Sets Blend color parameter.
     */
    void setBlendColor(const vec4<GLfloat> &color);

    /**
     * Enables or disables dithering.
     */
    void setDither(bool enableDither);

    /**
     * Enables or disables logical operation.
     */
    void setLogicOp(bool enableLogic);

    /**
     * Enables or disables logical operation.
     */
    void setLogicOp(bool enableLogic, LogicOperation logicOp);

    /**
     * Sets color buffer's writing mask.
     */
    void setColorMask(bool r, bool g, bool b, bool a);

    /**
     * Sets color buffer's writing mask.
     */
    void setColorMask(BufferId buffer, bool r, bool g, bool b, bool a);

    /**
     * Sets depth buffer's writing mask.
     */
    void setDepthMask(bool d);

    /**
     * Sets stencil buffer's writing mask.
     */
    void setStencilMask(GLuint frontMask, GLuint backMask);

    /**
     * Clears the buffers attached to this framebuffer.
     *
     * @param color true to clear the attached color buffers.
     * @param stencil true to clear the attached stencil buffer.
     * @param depth true to clear the attached depth buffer.
     */
    void clear(bool color, bool stencil, bool depth);

    /**
     * Draws the given mesh.
     *
     * @param p the program to use to draw the mesh.
     * @param mesh the mesh to draw.
     * @param primCount the number of times this mesh must be instanced.
     */
    template<class vertex, class index>
    void draw(ptr<Program> p, const Mesh<vertex, index> &mesh, int primCount = 1);

    /**
     * Draws a part of a mesh one or more times.
     *
     * @param p the program to use to draw the mesh.
     * @param mesh the mesh to draw.
     * @param m how the mesh vertices must be interpreted.
     * @param first the first vertex to draw, or the first indice to draw if
     *      this mesh has indices.
     * @param count the number of vertices to draw, or the number of indices
     *      to draw if this mesh has indices.
     * @param primCount the number of times this mesh must be drawn (with
     *      geometry instancing).
     * @param base the base vertex to use. Only used for meshes with indices.
     */
    void draw(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, GLint first, GLsizei count, GLsizei primCount = 1, GLint base = 0);

    /**
     * Draws several parts of a mesh. Each part is specified with a first
     * and count parameter as in #draw(). These values are passed in arrays
     * of primCount values.
     *
     * @param p the program to use to draw the mesh.
     * @param mesh the mesh to draw.
     * @param m how the mesh vertices must be interpreted.
     * @param firsts an array of primCount 'first vertex' to draw, or an array
     *      of 'first indice' to draw if this mesh has indices.
     * @param counts an array of number of vertices to draw, or an array of
     *      number of indices to draw if this mesh has indices.
     * @param primCount the number of parts of this mesh to draw.
     * @param bases the base vertices to use. Only used for meshes with indices.
     */
    void multiDraw(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint* bases = 0);

    /**
     * Draws a part of a mesh one or more times.
     * Only available with OpenGL 4.0 or more.
     *
     * @param p the program to use to draw the mesh.
     * @param m how the mesh vertices must be interpreted.
     * @param buf a CPU or GPU buffer containing the 'count', 'primCount',
     *      'first' and 'base' parameters, in this order, followed by '0',
     *      as 32 bit integers.
     */
    void drawIndirect(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, const Buffer &buf);

    /**
     * Draws a mesh with a vertex count resulting from a transform feedback session.
     * Only available with OpenGL 4.0 or more.
     *
     * @param p the program to use to draw the mesh.
     * @param mesh the mesh to draw.
     * @param m how the mesh vertices must be interpreted.
     * @param tfb a TransformFeedback containing the results of a transform feedback session.
     * @param stream the stream to draw.
     */
    void drawFeedback(ptr<Program> p, const MeshBuffers &mesh, MeshMode m, const TransformFeedback &tfb, int stream = 0);

    /**
     * Draws a quad mesh. This mesh has a position attribute made of four
     * floats. xy coordinates vary between -1 and 1, while zw coordinates
     * vary between 0 and 1.
     */
    void drawQuad(ptr<Program> p);

    /**
     * Reads pixels from the attached color buffers into the given buffer.
     *
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param f the format of pixels to be stored in dstBuf.
     * @param t the type of pixel components in dstBuf.
     * @param s optional pixel storage parameters for dstBuf.
     * @param dstBuf destination buffer for the read pixels.
     * @param clamp true to clamp read colors to 0..1.
     */
    void readPixels(int x, int y, int w, int h, TextureFormat f, PixelType t, const Buffer::Parameters &s, const Buffer &dstBuf, bool clamp = false);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int x, int y, int w, const Texture1D &dst, int level);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param layer destination layer in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param d number of layers to read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int layer, int x, int y, int w, int d, const Texture1DArray &dst, int level);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int x, int y, int w, int h, const Texture2D &dst, int level);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param layer destination layer in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int layer, int x, int y, int w, int h, const Texture2DArray &dst, int level);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param zoff z offset in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int zoff, int x, int y, int w, int h, const Texture3D &dst, int level);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     * @param cf destination face for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int x, int y, int w, int h, const TextureCube &dst, int level, CubeFace cf);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param layer destination layer in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     * @param cf destination face for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int layer, int x, int y, int w, int h, const TextureCubeArray &dst, int level, CubeFace cf);

    /**
     * Copies pixels from the attached color buffers into the given texture.
     *
     * @param xoff x offset in the destination texture.
     * @param yoff y offset in the destination texture.
     * @param x lower left corner of the area where the pixels must be read.
     * @param y lower left corner of the area where the pixels must be read.
     * @param w width of the area where the pixels must be read.
     * @param h height of the area where the pixels must be read.
     * @param dst destination texture for the read pixels.
     * @param level destination LOD level for the read pixels.
     */
    void copyPixels(int xoff, int yoff, int x, int y, int w, int h, const TextureRectangle &dst, int level);

    /**
     * Returns the OpenGL major version.
     */
    static GLint getMajorVersion();

    /**
     * Returns the OpenGL minor version.
     */
    static GLint getMinorVersion();

    /**
     * Returns the OpenGL state.
     *
     * @return the error code of the last invalid operation, or 0 if all
     *       operations executed normally.
     */
    static GLenum getError();

    /**
     * Resets all the internal Ork specific state.
     * Call this method before and after using OpenGL API directly.
     */
    static void resetAllStates();

private:
    struct FrameBufferMap : public Object
    {
        FrameBufferMap() : Object("FrameBufferMap")
        {
        }

        std::map< void*, ptr<FrameBuffer> > buffers;
    };

    /**
     * The id of this framebuffer object. 0 for the default one.
     */
    GLuint framebufferId;

    /**
     * The attachments of this framebuffer.
     */
    ptr<Object> textures[10];

    /**
     * The levels specified for each attachments of this framebuffer.
     */
    int levels[10];

    /**
     * The layers specified for each attachments of this framebuffer. Only used for
     * Texture arrays, Texture Cube, and Texture 3D.
     */
    int layers[10];

    /**
     * True if #textures, #levels or #layers has changed since the last call to #set().
     */
    bool attachmentsChanged;

    /**
     * The read buffer.
     */
    BufferId readBuffer;

    /**
     * The number of draw buffers.
     */
    int drawBufferCount;

    /**
     * The draw buffers.
     */
    BufferId drawBuffers[8];

    /**
     * True if #readBuffer, #drawBufferCount or #drawBuffers has changed since
     * the last call to #set().
     */
    bool readDrawChanged;

    /**
     * The parameters of this framebuffer.
     */
    Parameters parameters;

    /**
     * True if #parameters has changed since the last call to #set().
     */
    bool parametersChanged;

    /**
     * The default, onscreen framebuffer (one per OpenGL context).
     */
    static static_ptr<FrameBufferMap> DEFAULT;

    /**
     * The current framebuffer.
     */
    static FrameBuffer* CURRENT;

    /**
     * The current framebuffer parameters.
     */
    static Parameters PARAMETERS;

    /**
     * A quad mesh.
     */
    static static_ptr< Mesh<vec4f, unsigned int> > QUAD;

    /**
     * Creates a new framebuffer.
     *
     * @param main true for the default framebuffer.
     */
    FrameBuffer(bool main);

    /**
     * Sets this framebuffer as the current framebuffer.
     */
    void set();

    /**
     * Sets the attachments of this framebuffer.
     */
    void setAttachments();

    /**
     * Checks the attachments of this framebuffer.
     */
    void checkAttachments();

    /**
     * Enables the conditional rendering mode.
     */
    void beginConditionalRender();

    /**
     * Disables the conditional rendering mode.
     */
    void endConditionalRender();

    /**
     * Returns the id of the given framebuffer attachment point.
     */
    GLenum getBuffer(BufferId b) const;

    friend class TransformFeedback;
};

template<class vertex, class index>
inline void FrameBuffer::draw(ptr<Program> p, const Mesh<vertex, index> &mesh, int primCount)
{
    assert(TransformFeedback::TRANSFORM == NULL);
    set();
    p->set();
    beginConditionalRender();
    mesh.getBuffers()->draw(mesh.getMode(), 0, mesh.getIndiceCount() == 0 ? mesh.getVertexCount() : mesh.getIndiceCount(), primCount, 0);
    endConditionalRender();
}

}

#endif
