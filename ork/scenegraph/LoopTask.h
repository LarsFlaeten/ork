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

#ifndef _ORK_LOOP_TASK_H_
#define _ORK_LOOP_TASK_H_

#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to execute a task on a set of scene nodes.
 * @ingroup scenegraph
 */
class ORK_API LoopTask : public AbstractTask
{
public:
    /**
     * Creates a new LoopTask.
     *
     * @param var the loop variable name.
     * @param flag a flag that specifies the scene nodes to which the loop must
     *      be applied.
     * @param cull true to apply the loop only on the visible scene nodes.
     * @param parallel true the apply the loop to all scene nodes in parallel.
     * @param subtask the task that must be executed on each SceneNode.
     */
    LoopTask(const string &var, const string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask);

    /**
     * Deletes this LoopTask.
     */
    virtual ~LoopTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an empty LoopTask.
     */
    LoopTask();

    /**
     * Initializes this LoopTask.
     *
     * @param var the loo variable name.
     * @param flag a flag that specifies the scene nodes to which the loop must
     *      be applied.
     * @param cull true to apply the loop only on the visible scene nodes.
     * @param parallel true the apply the loop to all scene nodes in parallel.
     * @param subtask the task that must be executed on each SceneNode.
     */
    void init(const string &var, const string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask);

    /**
     * Swaps this LoopTask with the given one.
     *
     * @param t a LoopTask.
     */
    void swap(ptr<LoopTask> t);

private:
    /**
     * The loop variable name.
     */
    string var;

    /**
     * The flag thatt specifies the scene nodes to which the loop must be applied.
     */
    string flag;

    /**
     * True to apply the loop to all scene nodes in parallel.
     */
    bool parallel;

    /**
     * True to apply the loop only on the visible scene nodes.
     */
    bool cull;

    /**
     * The task that must be executed on each scene node.
     */
    ptr<TaskFactory> subtask;
};

}

#endif
