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

#ifndef _ORK_SET_TARGET_TASK_H_
#define _ORK_SET_TARGET_TASK_H_

#include "ork/render/FrameBuffer.h"
#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to set the attachments of a framebuffer.
 * @ingroup scenegraph
 */
class ORK_API SetTargetTask : public AbstractTask
{
public:
    /**
     * A framebuffer attachment specification.
     */
    struct Target {
        /**
         * A framebuffer attachment point.
         */
        BufferId buffer;

        /**
         * The texture to be attached to #buffer. Each texture is specified by a
         * "node.uniform" or "node.module:uniform" qualified name. The first
         * part specifies the scene node that contains the texture. The second
         * part specifies the name of the uniform that refers to the texture
         * (either directly or via a module).
         */
        QualifiedName texture;

        /**
         * The mipmap level of #texture to be attached.
         */
        int level;

        /**
         * The layer, z slice or cube face of #texture to be attached.
         */
        int layer;
    };

    /**
     * Creates a new SetTargetTask.
     *
     * @param targets the framebuffer attachments to be set.
     * @param autoResize true to automatically resize the target textures to
     *      the default framebuffer viewport size.
     */
    SetTargetTask(const std::vector<Target> &targets, bool autoResize);

    /**
     * Deletes this SetTargetTask.
     */
    virtual ~SetTargetTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an uninitialized SetTargetTask.
     */
    SetTargetTask();

    /**
     * Initializes this SetTargetTask.
     *
     * @param targets the framebuffer attachments to be set.
     * @param autoResize true to automatically resize the target textures to
     *      the default framebuffer viewport size.
     */
    void init(const std::vector<Target> &targets, bool autoResize);

    /**
     * Swaps this SetTargetTask with the given one.
     *
     * @param t a SetTargetTask.
     */
    void swap(ptr<SetTargetTask> t);

private:
    /**
     * An offscreen framebuffer for use with SetTargetTask.
     */
    static static_ptr<FrameBuffer> TARGET_BUFFER;

    /**
     * The framebuffer attachments to be set.
     */
    std::vector<Target> targets;

    /**
     * True to automatically resize the target textures to the default
     * framebuffer viewport size.
     */
    bool autoResize;

    /**
     * Returns an offscreen framebuffer for use with SetTargetTask.
     */
    static ptr<FrameBuffer> getTargetBuffer();

    /**
     * An ork::Task to set the attachments of a framebuffer.
     */
    class Impl : public Task
    {
    public:
        /**
         * The SetTargetTask that created this task.
         */
        ptr<SetTargetTask> source;

        /**
         * The textures to be set to the framebuffer attachment points.
         */
        std::vector< ptr<Texture> > textures;

        /**
         * Creates a new SetTargetTask::Impl.
         *
         * @param source the SetTargetTask that created this task.
         * @param textures the textures to be set to the framebuffer attachment
         *      points.
         */
        Impl(ptr<SetTargetTask> source, std::vector< ptr<Texture> > textures);

        /**
         * Deletes this SetTargetTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
