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

#ifndef _ORK_SEQUENCE_TASK_H_
#define _ORK_SEQUENCE_TASK_H_

#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to compose a list of tasks in a sequence.
 * @ingroup scenegraph
 */
class ORK_API SequenceTask : public AbstractTask
{
public:
    /**
     * Creates a SequenceTask with the given list of tasks.
     *
     * @param subtasks the tasks that must be composed into a sequence.
     */
    SequenceTask(const vector< ptr<TaskFactory> > &subtasks);

    /**
     * Deletes this SequenceTask.
     */
    virtual ~SequenceTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an empty SequenceTask.
     */
    SequenceTask();

    /**
     * Initializes this SequenceTask with the given list of tasks.
     *
     * @param subtasks the tasks that must be composed into a sequence.
     */
    void init(const vector< ptr<TaskFactory> > &subtasks);

    /**
     * Swaps this SequenceTask with another one.
     *
     * @param t a SequenceTask.
     */
    void swap(ptr<SequenceTask> t);

private:
    /**
     * The tasks that are composed sequentially by this task.
     */
    vector< ptr<TaskFactory> > subtasks;
};

}

#endif
