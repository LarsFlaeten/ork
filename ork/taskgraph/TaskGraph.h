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

#ifndef _ORK_TASK_GRAPH_H_
#define _ORK_TASK_GRAPH_H_

#include <set>
#include <map>
#include "ork/core/Iterator.h"
#include "ork/taskgraph/Task.h"

using namespace std;

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
    virtual void init(set<Task*> &initialized);

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
    void removeAndGetDependencies(ptr<Task> src, set< ptr<Task> >& deletedDependencies);

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
    set< ptr<Task> > allTasks; ///< all the tasks of this graph

    set< ptr<Task> > firstTasks; ///< the tasks without predecessors

    set< ptr<Task> > lastTasks; ///< the tasks without successors

    set< ptr<Task> > flattenedFirstTasks; ///< the primitive tasks without predecessors

    set< ptr<Task> > flattenedLastTasks; ///< the primitive tasks without successors

    /**
     * The predecessors of the sub tasks of this graph.
     * Maps each task to its set of predecessors.
     */
    map< ptr<Task>, set< ptr<Task> > > dependencies;

    /**
     * The successors of the sub tasks of this graph.
     * Maps each task to its set of successors.
     */
    map< ptr<Task>, set< ptr<Task> > > inverseDependencies;

    friend class MultithreadScheduler;
};

}

#endif
