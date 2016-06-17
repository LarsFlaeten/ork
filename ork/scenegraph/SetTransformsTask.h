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
    SetTransformsTask(const string &screen, QualifiedName m,
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
    void init(const string &screen, QualifiedName m,
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
