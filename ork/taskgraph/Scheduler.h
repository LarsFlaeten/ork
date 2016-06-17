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

#ifndef _ORK_SCHEDULER_H_
#define _ORK_SCHEDULER_H_

#include "ork/taskgraph/Task.h"

namespace ork
{

/**
 * An abstract scheduler, sorts and executes tasks with one or more threads.
 * @ingroup taskgraph
 */
class ORK_API Scheduler : public Object
{
public:
    /**
     * Creates a new scheduler.
     *
     * @param type the type of this scheduler.
     */
    Scheduler(const char *type);

    /**
     * Deletes this scheduler.
     */
    virtual ~Scheduler();

    /**
     * Returns true if this scheduler can execute CPU or GPU tasks whose
     * deadline is not immediate. This means tasks whose result will be needed
     * in the next few frames, but that are known in advance and could be
     * computed ahead of time to reduce the load of these coming frames.
     *
     * @param gpuTasks true to know if this scheduler can prefetch GPU tasks,
     *      or false to know if it can prefetch CPU tasks.
     * @return true if this scheduler can prefetch GPU (resp. CPU) tasks, if
     *      gpuTasks is true (resp. false).
     */
    virtual bool supportsPrefetch(bool gpuTasks) = 0;

    /**
     * Adds a task whose deadline is not immediate. This method must not be
     * called if this scheduler does not support prefetch (see
     * #supportsPrefetch). Otherwise it adds this task and its sub tasks to the
     * list of tasks to be executed by this scheduler, and returns immediately
     * (i.e. before these tasks are executed).
     *
     * @param task a task or task graph whose deadline is not immediate.
     */
    virtual void schedule(ptr<Task> task) = 0;

    /**
     * Forces the reexecution of the given task and of its sub tasks.
     *
     * @param task a task or task graph that must be reexecuted. This task is marked
     *      as undone (with Task#setIsDone) so that it will be reexecuted.
     * @param r the reason why the task must be reexecuted.
     * @param deadline the frame number before which this task must be
     *      reexecuted.
     */
    virtual void reschedule(ptr<Task> task, Task::reason r, unsigned int deadline) = 0;

    /**
     * Executes the given tasks. This method does not return before all tasks
     * with an immediate deadline are completed.
     *
     * @param task a task or task graph to be executed.
     */
    virtual void run(ptr<Task> task) = 0;

protected:
    /**
     * Swaps this scheduler with the given one.
     */
    void swap(ptr<Scheduler> s);
};

}

#endif
