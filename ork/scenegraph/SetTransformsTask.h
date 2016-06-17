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

#ifndef _ORK_SET_TRANSFORMS_TASK_H_
#define _ORK_SET_TRANSFORMS_TASK_H_

#include "ork/scenegraph/AbstractTask.h"
#include "ork/render/Program.h"

namespace ork
{

/**
 * An AbstractTask to set transformation matrices in programs.
 * @ingroup scenegraph
 */
class ORK_API SetTransformsTask : public AbstractTask
{
public:
    /**
     * Creates a new SetTransformsTask.
     *
     * @param screen the "screen" node to be used for transformation involving
     *     the "screen" space. An empty name means the "real" screen space of
     *     the camera node.
     * @param m a "node.module" qualified name. The first part specifies the
     *      scene node that contains the module. The second part specifies the
     *      name of a module in this node. This module is used to find the
     *      uniforms that this task must set.
     * @param t the vec2 uniform that contains time of current frame and time
     *      elapsed since last frame.
     * @param ltow the mat4 uniform to be set to the local to world
     *      transformation.
     * @param ltos the mat4 uniform to be set to the local to screen
     *      transformation.
     * @param ctow the mat4 uniform to be set to the camera to world
     *      transformation.
     * @param ctos the mat4 uniform to be set to the camera to screen
     *      transformation.
     * @param stoc the mat4 uniform to be set to the screen to camera
     *      transformation.
     * @param wtos the mat4 uniform to be set to the world to screen
     *      transformation.
     * @param wp the vec3 uniform to be set to the world coordinates
     *      of the origin of the local frame.
     * @param wd the vec3 uniform to be set to the world coordinates
     *      of the unit z vector of the local frame.
     */
    SetTransformsTask(const std::string &screen, QualifiedName m,
        const char *t, const char *ltow, const char *ltos,
        const char *ctow, const char *ctos, const char *stoc,
        const char *wtos, const char *wp, const char *wd);

    /**
     * Deletes this SetTransformsTask.
     */
    virtual ~SetTransformsTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an uninitialized SetTransformsTask.
     */
    SetTransformsTask();

    /**
     * Initializes this SetTransformsTask.
     *
     * See #SetTransformsTask.
     */
    void init(const std::string &screen, QualifiedName m,
        const char *t, const char *ltow, const char *ltos,
        const char *ctow, const char *ctos, const char *stoc,
        const char *wtos, const char *wp, const char *wd);

    /**
     * Swaps this SetTransformsTask with the given one.
     *
     * @param t a SetTransformsTask.
     */
    void swap(ptr<SetTransformsTask> t);

private:
    /**
     * The "screen" node to be used for transformation involving the "screen"
     * space. An empty name means the "real" screen space of the camera node.
     */
    QualifiedName screen;

    QualifiedName m;

    ptr<Module> module;

    ptr<Program> lastProg;

    ptr<Uniform2f> time;

    ptr<UniformMatrix4f> localToWorld;

    ptr<UniformMatrix4f> localToScreen;

    ptr<UniformMatrix4f> cameraToWorld;

    ptr<UniformMatrix4f> cameraToScreen;

    ptr<UniformMatrix4f> screenToCamera;

    ptr<UniformMatrix4f> worldToScreen;

    ptr<Uniform3f> worldPos;

    ptr<Uniform3f> worldDir;

    const char *t;

    const char *ltow;

    const char *ltos;

    const char *ctow;

    const char *ctos;

    const char *stoc;

    const char *wtos;

    const char *wp;

    const char *wd;

    /**
     * An ork::Task to set transformation matrices in programs.
     */
    class Impl : public Task
    {
    public:
        /**
         * The scene node corresponding to the "screen" space.
         */
        ptr<SceneNode> screenNode;

        /**
         * Creates a new SetTransformsTask::Task.
         *
         * @param screenNode the SceneNode corresponding to the "screen" space.
         * @param context the SceneNode that contains the Method to which
         *      'source' belongs.
         * @param source the SetTransformsTask that created this task.
         */
        Impl(ptr<SceneNode> screenNode, ptr<SceneNode> context, ptr<SetTransformsTask> source);

        /**
         * Deletes this SetTransformsTask::Task
         */
        virtual ~Impl();

        virtual bool run();

    private:
        /**
         * The SceneNode that contains the Method to which #source belongs.
         */
        ptr<SceneNode> context;

        /**
         * The SetTransformsTask that created this task.
         */
        ptr<SetTransformsTask> source;
    };

    friend class Impl;
};

}

#endif
