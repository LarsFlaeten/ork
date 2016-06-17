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

#ifndef _ORK_TASK_GRAPH_H_
#define _ORK_TASK_GRAPH_H_

#include <set>
#include <map>
#include "ork/core/Iterator.h"
#include "ork/taskgraph/Task.h"

namespace ork
{

/**
 * A task made of several sub tasks that may depend on each other. A dependency
 * between two tasks means that one task must be executed before the other. More
 * precisely a dependency from task src to task dst means that dst must be
 * executed before src. We say that dst is a predecessor task for src, and that
 * src is a successor task for dst. Likewise, we say that src has a predecessor
 * task (dst), and that dst has a successor task (src). A task graph is
 * completed when all its sub tasks are executed.
 *
 * @ingroup taskgraph
 */
class ORK_API TaskGraph : public Task, public TaskListener
{
public:
    /**
     * An iterator to iterate over a set of tasks.
     */
    typedef SetIterator< ptr<Task> > TaskIterator;

    /**
     * Creates a new, empty task graph.
     */
    TaskGraph();

    /**
     * Creates a new task graph that encapsulates the given task.
     */
    TaskGraph(ptr<Task>);

    /**
     * Deletes this tak graph.
     */
    virtual ~TaskGraph();

    /**
     * Calls init recursively on all sub tasks of this task graph.
     */
    virtual void init(std::set<Task*> &initialized);

    /**
     * Calls #setIsDone recursively on all sub tasks of this task graph.
     */
    virtual void setIsDone(bool done, unsigned int t, reason r);

    /**
     * Calls #setPredecessorsCompletionDate on the sub tasks of this task
     * without predecessors.
     */
    virtual void setPredecessorsCompletionDate(unsigned int t);

    /**
     * Returns true if this task graph does not contain any sub task.
     */
    bool isEmpty();

    /**
     * Returns all the sub tasks of this task. The returned iterator does not
     * iterate recursively in the sub sub tasks, if any.
     */
    TaskIterator getAllTasks();

    /**
     * Returns the sub tasks that do not have any predecessor. These tasks are
     * executed first. The returned iterator does not iterate recursively in the
     * sub sub tasks, if any.
     */
    TaskIterator getFirstTasks();

    /**
     * Returns the sub tasks that do not have any successor. These tasks are
     * executed last. The returned iterator does not iterate recursively in the
     * sub sub tasks, if any.
     */
    TaskIterator getLastTasks();

    /**
     * Returns the predecessor tasks of the given task.
     *
     * @param t a sub task of this task graph.
     */
    TaskIterator getDependencies(ptr<Task> t);

    /**
     * Returns the successor tasks of the given task.
     *
     * @param t a sub task of this task graph.
     */
    TaskIterator getInverseDependencies(ptr<Task> t);

    /**
     * Adds a sub task to this task graph. Note that a task can be added to
     * several task graphs at the same time.
     *
     * @param t the task to be added to this sub graph. This task can be a task
     *      graph itself.
     */
    void addTask(ptr<Task> t);

    /**
     * Removes a sub task from this task graph. This sub task must not have any
     * dependencies or inverse dependencies.
     *
     * @param t the task to be removed from this sub graph.
     */
    void removeTask(ptr<Task> t);

    /**
     * Adds a dependency between two sub tasks of this task graph.
     *
     * @param src a sub task of this graph that must be executed after dst.
     * @param dst a sub task of this graph that must be executed before src.
     */
    void addDependency(ptr<Task> src, ptr<Task> dst);

    /**
     * Removes a dependency between two sub tasks of this task graph.
     *
     * @param src a sub task of this graph that must be executed after dst.
     * @param dst a sub task of this graph that must be executed before src.
     */
    void removeDependency(ptr<Task> src, ptr<Task> dst);

    /**
     * Removes all dependencies of the given subtask.
     * All deleted dependencies are stored in deletedDependencies.
     *
     * @param src a sub task of this graph.
     * @param[out] deletedDependencies the dependencies that src had.
     */
    void removeAndGetDependencies(ptr<Task> src, std::set< ptr<Task> >& deletedDependencies);

    /**
     * Removes all the dependencies between the sub tasks of this task graph.
     */
    void clearDependencies();

    /**
     * Notifies the listeners of this task that its execution state has changed.
     * This method is called when the execution state of a sub task of this graph
     * has changed. Indeed a TaskGraph is a TaskListener that listens to state
     * changes in all its sub tasks.
     */
    virtual void taskStateChanged(ptr<Task> t, bool done, Task::reason r);

    /**
     * Notifies the listeners of this task that the completion date of a sub task
     * without successors has changed.
     */
    virtual void completionDateChanged(ptr<Task> t, unsigned int date);

protected:
    /**
     * Clears the temporary data structures in this task graph that are used by
     * schedulers.
     */
    void cleanup();

private:
    std::set< ptr<Task> > allTasks; ///< all the tasks of this graph

    std::set< ptr<Task> > firstTasks; ///< the tasks without predecessors

    std::set< ptr<Task> > lastTasks; ///< the tasks without successors

    std::set< ptr<Task> > flattenedFirstTasks; ///< the primitive tasks without predecessors

    std::set< ptr<Task> > flattenedLastTasks; ///< the primitive tasks without successors

    /**
     * The predecessors of the sub tasks of this graph.
     * Maps each task to its set of predecessors.
     */
    std::map< ptr<Task>, std::set< ptr<Task> > > dependencies;

    /**
     * The successors of the sub tasks of this graph.
     * Maps each task to its set of successors.
     */
    std::map< ptr<Task>, std::set< ptr<Task> > > inverseDependencies;

    friend class MultithreadScheduler;
};

}

#endif
