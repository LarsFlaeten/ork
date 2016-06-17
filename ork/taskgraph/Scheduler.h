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
