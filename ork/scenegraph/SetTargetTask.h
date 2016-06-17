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
    SetTargetTask(const vector<Target> &targets, bool autoResize);

    /**
     * Deletes this SetTargetTask.
     */
    virtual ~SetTargetTask();

    virtual ptr<Task> getTask(ptr<Object> context);

    /**
     * Returns a shared offscreen framebuffer.
     */
    static ptr<FrameBuffer> getOffscreenBuffer();

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
    void init(const vector<Target> &targets, bool autoResize);

    /**
     * Swaps this SetTargetTask with the given one.
     *
     * @param t a SetTargetTask.
     */
    void swap(ptr<SetTargetTask> t);

private:
    /**
     * An offscreen framebuffer.
     */
    static static_ptr<FrameBuffer> FRAME_BUFFER;

    /**
     * An offscreen framebuffer for use with SetTargetTask.
     */
    static static_ptr<FrameBuffer> TARGET_BUFFER;

    /**
     * The framebuffer attachments to be set.
     */
    vector<Target> targets;

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
        vector< ptr<Texture> > textures;

        /**
         * Creates a new SetTargetTask::Impl.
         *
         * @param source the SetTargetTask that created this task.
         * @param textures the textures to be set to the framebuffer attachment
         *      points.
         */
        Impl(ptr<SetTargetTask> source, vector< ptr<Texture> > textures);

        /**
         * Deletes this SetTargetTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
