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
    LoopTask(const std::string &var, const std::string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask);

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
    void init(const std::string &var, const std::string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask);

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
    std::string var;

    /**
     * The flag thatt specifies the scene nodes to which the loop must be applied.
     */
    std::string flag;

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
