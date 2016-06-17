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

#ifndef _ORK_SET_STATE_TASK_H_
#define _ORK_SET_STATE_TASK_H_

#include "ork/render/FrameBuffer.h"
#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to set the state of a framebuffer.
 * @ingroup scenegraph
 */
class ORK_API SetStateTask : public AbstractTask
{
public:
    /**
     * A 'subtask' of this task.
     */
    class Runnable
    {
    protected:
        /**
         * Deletes this 'subtask'.
         */
        virtual ~Runnable();

        /**
         * Runs this 'subtask'.
         *
         * @param fb the framebuffer to use for this 'subtask'.
         */
        virtual void run(ptr<FrameBuffer> fb) = 0;

        friend class SetStateTask;
    };

    /**
     * Creates an empty SetStateTask.
     */
    SetStateTask();

    /**
     * Deletes this SetStateTask.
     */
    virtual ~SetStateTask();

    /**
     * Adds a 'subtask' to this task.
     *
     * @param r a 'subtask' to be added to this task.
     */
    void addRunnable(Runnable *r);

    /**
     * Sets the viewport (up, down, left and right planes).
     *
     * @param viewport the new viewport.
     */
    void setViewport(vec4<GLint> viewport);

    /**
     * Sets the depth range (near and far planes).
     *
     * @param n near plane.
     * @param f far plane.
     */
    void setDepthRange(GLfloat n, GLfloat f);

    /**
     * Sets the clipping bits, used to determine which planes will be used for clipping.
     */
    void setClipDistances(GLint d);

    /**
     * Sets the color used to clear the current draw buffer.
     */
    void setClearColor(vec4<GLfloat> &c);

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
    void setPointFadeThresholdSize(GLfloat tSize);

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
    void setSampleMask(GLint sampleMask);

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
     * Enables or disables scissor test.
     */
    void setScissorTest(bool enableScissor, vec4<GLint> &scissor);

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
        Function ff = (Function)-1, GLint fref = -1, GLint fmask = -1, StencilOperation ffail = (StencilOperation)-1, StencilOperation fdpfail = (StencilOperation)-1, StencilOperation fdppass = (StencilOperation)-1,
        Function bf = (Function)-1, GLint bref = -1, GLint bmask = -1, StencilOperation bfail = (StencilOperation)-1, StencilOperation bdpfail = (StencilOperation)-1, StencilOperation bdppass = (StencilOperation)-1);

    /**
     * Enables or disables depth test.
     */
    void setDepthTest(bool enableDepth, Function depth = (Function) -1);

    /**
     * Enables or disables blending.
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
    void setBlend(BufferId buffer, bool enableBlend, BlendEquation rgb = (BlendEquation)-1, BlendArgument srgb = (BlendArgument)-1, BlendArgument drgb = (BlendArgument)-1,
        BlendEquation alpha = (BlendEquation)-1, BlendArgument salpha = (BlendArgument)-1, BlendArgument dalpha = (BlendArgument)-1);

    /**
     * Sets Blend color parameter.
     */
    void setBlendColor(vec4<GLfloat> &color);

    /**
     * Enables or disables dithering.
     */
    void setDither(bool enableDither);

    /**
     * Enables or disables logical operation.
     */
    void setLogicOp(bool enableLogic, LogicOperation logicOp = (LogicOperation)-1);

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
    void setStencilMask(GLint frontMask, GLint backMask);

    /**
     * Sets the color, stencil and depth used to clear the current draw buffer.
     */
    void setClearState(bool color, bool stencil, bool depth);

    /**
     * Sets the read and draw buffers.
     */
    void setBuffers(BufferId rb, BufferId db);

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Swaps this SetStateTask with the given one.
     *
     * @param t a SetStateTask.
     */
    void swap(ptr<SetStateTask> t);

private:
    /**
     * The 'subtasks' to do in this task.
     */
    vector<Runnable*> runnables;

    /**
     * Runs each 'subtask' in #runnables.
     */
    void run();

    /**
     * A ork::Task to set the state of a framebuffer.
     */
    class Impl : public Task
    {
    public:
        /**
         * The SetStateTask that created this task.
         */
        ptr<SetStateTask> source;

        /**
         * Creates a new SetStateTask::Impl.
         *
         * @param source the SetStateTask that created this task.
         */
        Impl(ptr<SetStateTask> source);

        /**
         * Deletes this SetStateTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();
    };

    friend class Impl;
};

}

#endif
