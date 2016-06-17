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

#include "ork/taskgraph/TaskGraph.h"

namespace ork
{

TaskGraph::TaskGraph() : Task("TaskGraph", false, 0)
{
}

TaskGraph::TaskGraph(ptr<Task> task) : Task("TaskGraph", false, 0)
{
    addTask(task);
}

TaskGraph::~TaskGraph()
{
    TaskIterator i = getAllTasks();
    while (i.hasNext()) {
        i.next()->removeListener(this);
    }
}

void TaskGraph::setIsDone(bool done, unsigned int t, reason r)
{
    Task::setIsDone(done, t, r);
    if (!done) { // calls sub tasks recursively only if task must be reexecuted
        // if a dependency of this task graph has changed, then all sub tasks
        // must be reexecuted; otherwise, if the data produced by this graph
        // is needed again then, a priori, only the sub tasks without successors
        // must be reexecuted (these sub tasks may need other sub tasks to be
        // reexecuted if they need their data; in this case they can change
        // their execution state recursively in their own setIsDone method).
        TaskIterator i = r == DEPENDENCY_CHANGED ? getAllTasks() : getLastTasks();
        while (i.hasNext()) {
            i.next()->setIsDone(done, t, r);
        }
    }
}

void TaskGraph::setPredecessorsCompletionDate(unsigned int t)
{
    TaskIterator i = getFirstTasks();
    while (i.hasNext()) {
        i.next()->setPredecessorsCompletionDate(t);
    }
}

/*
 * Initializes a set of tasks. In order to support the modification of a task
 * graph during the initialization of its sub tasks, this method is recursive
 * and initializes the tasks after all the iterator elements have been found.
 * Without this the iterator over the sub tasks could become invalid if the set
 * of sub tasks is modified during the iteration (by the init method of one of
 * the sub tasks).
 */
void initTasks(TaskGraph::TaskIterator &i, set<Task*> &initialized)
{
    if (i.hasNext()) {
        ptr<Task> t = i.next();
        initTasks(i, initialized);
        t->init(initialized);
    }
}

void TaskGraph::init(set<Task*> &initialized)
{
    if (initialized.find(this) == initialized.end()) {
        initialized.insert(this);
        TaskIterator i = getAllTasks();
        initTasks(i, initialized);
    }
}

bool TaskGraph::isEmpty()
{
    return allTasks.size() == 0;
}

TaskGraph::TaskIterator TaskGraph::getAllTasks()
{
    return TaskIterator(allTasks);
}

TaskGraph::TaskIterator TaskGraph::getFirstTasks()
{
    return TaskIterator(firstTasks);
}

TaskGraph::TaskIterator TaskGraph::getLastTasks()
{
    return TaskIterator(lastTasks);
}

TaskGraph::TaskIterator TaskGraph::getDependencies(ptr<Task> t)
{
    map< ptr<Task>, set< ptr<Task> > >::iterator i;
    i = dependencies.find(t);
    if (i != dependencies.end()) {
        set< ptr<Task> > s = i->second;
        return TaskIterator(s);
    }
    return TaskIterator();
}

TaskGraph::TaskIterator TaskGraph::getInverseDependencies(ptr<Task> t)
{
    map< ptr<Task>, set< ptr<Task> > >::iterator i;
    i = inverseDependencies.find(t);
    if (i != inverseDependencies.end()) {
        return TaskIterator(i->second);
    }
    return TaskIterator();
}

void TaskGraph::addTask(ptr<Task> t)
{
    assert(t.cast<TaskGraph>() == NULL || !t.cast<TaskGraph>()->isEmpty());
    if (allTasks.find(t) == allTasks.end()) {
        // if the task has not already been added we add ourselves has a listener
        t->addListener(this);
        // and we add the task in all three sets of tasks
        allTasks.insert(t);
        firstTasks.insert(t);
        lastTasks.insert(t);
    }
}

void TaskGraph::removeTask(ptr<Task> t)
{
    set< ptr<Task> >::iterator i = allTasks.find(t);
    if (i != allTasks.end()) {
        // we remove ourselves from the listeners of t
        t->removeListener(this);
        // and we remove t from all data structures
        allTasks.erase(t);
        firstTasks.erase(t);
        lastTasks.erase(t);
        assert(dependencies.find(t) == dependencies.end());
        assert(inverseDependencies.find(t) == inverseDependencies.end());
    }
}

void TaskGraph::addDependency(ptr<Task> src, ptr<Task> dst)
{
    assert(allTasks.find(src) != allTasks.end());
    assert(allTasks.find(dst) != allTasks.end());
    // src now has a predecessor,
    // so it must be removed from the set of tasks without predecessor
    firstTasks.erase(src);
    // dst now has a successor,
    // so it must be removed from the set of tasks without successor
    lastTasks.erase(dst);
    // updates the successors and predecessors maps
    dependencies[src].insert(dst);
    inverseDependencies[dst].insert(src);
}

void TaskGraph::removeDependency(ptr<Task> src, ptr<Task> dst)
{
    assert(allTasks.find(src) != allTasks.end());
    assert(allTasks.find(dst) != allTasks.end());
    // updates the successors and predecessors maps
    dependencies[src].erase(dst);
    if (dependencies[src].empty()) {
        dependencies.erase(dependencies.find(src));
        // src has no more predecessors,
        // so it must be added to the set of tasks without predecessors
        firstTasks.insert(src);
    }
    inverseDependencies[dst].erase(src);
    if (inverseDependencies[dst].empty()) {
        inverseDependencies.erase(inverseDependencies.find(dst));
        // dst now has no more successors,
        // so it must be added to the set of tasks without successor
        lastTasks.insert(dst);
    }
}

void TaskGraph::removeAndGetDependencies(ptr<Task> src, set< ptr<Task> >& deletedDependencies)
{
    // find the set of dependencies for this task
    map< ptr<Task>, set< ptr<Task> > >::iterator it = dependencies.find(src);
    if (it != dependencies.end()) { // there are dependencies
        set< ptr<Task> > dests = it->second;
        for (set< ptr<Task> >::iterator i = dests.begin(); i != dests.end(); ++i) {
            deletedDependencies.insert(*i);
            set< ptr<Task> > dstSet = inverseDependencies[*i];
            bool removed = dstSet.erase(src) != 0;
            assert(removed); // should exist in inverse dependencies
            if (dstSet.empty()) {
                // dst now has no more successors,
                // so it must be added to the set of tasks without successor
                inverseDependencies.erase(inverseDependencies.find(*i));
                lastTasks.insert(*i);
            }
        }
        // erase src dependencies, erase all components
        dependencies.erase(it);
        // src has no more predecessors,
        // so it must be added to the set of tasks without predecessors
        firstTasks.insert(src);
    }
}

void TaskGraph::clearDependencies()
{
    firstTasks.insert(allTasks.begin(), allTasks.end());
    lastTasks.insert(allTasks.begin(), allTasks.end());
    dependencies.clear();
    inverseDependencies.clear();
}

void TaskGraph::taskStateChanged(ptr<Task> t, bool done, reason r)
{
    assert(allTasks.find(t) != allTasks.end());
    if (!done) {
        if (r != DATA_NEEDED) {
            Task::setIsDone(false, 0, r);

            // if the result of t is needed again but has not changed, the
            // tasks that depend on this result need not be reexecuted.
            // Otherwise we notify these successor tasks that one of their
            // dependencies has changed, and that they must be reexecuted:
            TaskIterator i = getInverseDependencies(t);
            while (i.hasNext()) {
                ptr<Task> s = i.next();
                s->setIsDone(false, 0, DEPENDENCY_CHANGED);
            }
        }
    } else {
        // updates the predecessor completion date of the successors of t
        completionDateChanged(t, t->getCompletionDate());

        // if a subtask of this task graph is now completed, the task graph
        // itself may become completed (it can of course not become uncompleted).
        TaskIterator i = getAllTasks();
        while (i.hasNext()) {
            if (!i.next()->isDone()) {
                return;
            }
        }
        Task::setIsDone(true, getCompletionDate());
    }
}

void TaskGraph::completionDateChanged(ptr<Task> t, unsigned int date)
{
    completionDate = max(completionDate, date);
    TaskIterator i = getInverseDependencies(t);
    if (i.hasNext()) {
        // if t has successors,
        // updates the predecessor completion date of the successors of t
        while (i.hasNext()) {
            ptr<Task> s = i.next();
            s->setPredecessorsCompletionDate(date);
        }
    } else {
        // if t does not have predecessors, notifies the listeners of
        // this taskgraph that its completion date has changed
        for (unsigned int i = 0; i < listeners.size(); ++i) {
            listeners[i]->completionDateChanged(this, date);
        }
    }
}

void TaskGraph::cleanup()
{
    flattenedFirstTasks.clear();
    flattenedLastTasks.clear();
}

}
