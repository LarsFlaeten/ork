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

#ifndef _ORK_MULTITHREAD_SCHEDULER_H_
#define _ORK_MULTITHREAD_SCHEDULER_H_

#include <vector>
#include <set>
#include <sstream>
#include "ork/taskgraph/Scheduler.h"
#include "ork/taskgraph/TaskGraph.h"

namespace ork
{

/**
 * A Scheduler that can use multiple threads. This scheduler can work with one
 * or more threads, and it can try to follow a fixed framerate (i.e. a number
 * of calls to #run per second). If a prefetch rate is specified, the main
 * thread executes at least the specified number of prefetching tasks par frame
 * (when such tasks are available), after all the tasks for the current frame
 * have been executed. Hence if a prefetch rate is specified, or if a fixed frame
 * rate is specified, this scheduler supports prefetching of tasks of any kind.
 * Otherwise, if several threads are used, prefetching of cpu tasks is supported,
 * but not prefetching of gpu tasks.
 *
 * @ingroup taskgraph
 */
class ORK_API MultithreadScheduler : public Scheduler
{
public:
    /**
     * Creates a new multithread scheduler.
     *
     * @param prefetchRate the minimum number of prefetch task to execute at
     *      each frame, after all the tasks for the current frame have been
     *      executed (and if prefetching tasks are available). The prefetching
     *      of gpu tasks is only possible if this rate is not 0.
     * @param prefetchQueue the maximum number of prefetching tasks that can be
     *      queued for execution. If a prefetch rate or a fixed frame rate is
     *      specified, this value must not be 0. When the queue is full, new
     *      prefetch tasks are simply discarded and #schedule returns false.
     *      This maximum queue size prevents the number of prefetching tasks to
     *      grow unbounded, if new prefetching tasks are generated at a greater
     *      rate than the rate at which they are executed.
     * @param frameRate a fixed framerate that this scheduler should try to
     *      follow, or 0 to not fix any framerate.
     * @param nThreads the number of threads to use in addition to the main
     *      thread of the application. Hence 0 means that only one thread will
     *      be used, the main application thread.
     */
    MultithreadScheduler(int prefetchRate = 0, int prefetchQueue = 0, float frameRate = 0.0f, int nThreads = 0);

    /**
     * Deletes this scheduler.
     */
    virtual ~MultithreadScheduler();

    /**
     * Returns true if the prefetch rate or the fixed frame rate is not null,
     * or if there are several threads and gpuTasks is false. In addition,
     * if there is only one thread for cpu tasks, and for gpu tasks in all
     * cases, the curent number of prefetching tasks waiting for execution must
     * be less than the prefetch queue size, otherwise this method returns false.
     */
    virtual bool supportsPrefetch(bool gpuTasks);

    virtual void schedule(ptr<Task> task);

    virtual void reschedule(ptr<Task> task, Task::reason r, unsigned int deadline);

    virtual void run(ptr<Task> task);

    /**
     * Adds the given task type to the tasks whose execution times must be monitored (debug).
     */
    void monitorTask(const string &taskType);

protected:
    /**
     * Initializes this scheduler.
     *
     * See #MultithreadScheduler.
     */
    void init(int prefetchRate, int prefetchQueue, float frameRate, int nThreads);

private:
    /**
     * A key to store tasks in a map. This key is made of the task deadline and
     * of its execution context.
     */
    typedef pair<unsigned int, void*> taskKey;

    /**
     * A sort operator for task keys. Task keys are sorted based on their
     * deadlines and if their deadlines are equal, based on their execution
     * context. This ensures that tasks whose result is needed first are
     * executed first, while providing a grouping of tasks that share the same
     * the same execution context (which can save context switches).
     */
    struct taskKeySort : public less<taskKey>
    {
        bool operator()(const taskKey &x, const taskKey &y) const;
    };

    /**
     * A sort operator for tasks. This operator is based on the expected
     * duration of tasks, so that shorter tasks are executed first.
     */
    struct taskSort : public less< ptr<Task> >
    {
        bool operator()(const ptr<Task> x, const ptr<Task> y) const;
    };

    /**
     * A sorted task set, where tasks are sorted based on their deadline,
     * execution context and expected duration.
     */
    typedef map<taskKey, set<ptr<Task>, taskSort>, taskKeySort> SortedTaskSet;

    /**
     * A mutex used to ensure consistent access to the data structures of this
     * scheduler from the various execution threads.
     */
    void* mutex;

    /**
     * A condition to signal to execution threads that new tasks are ready to be
     * executed.
     */
    void* allTasksCond;

    /**
     * A condition to signal to execution threads that new CPU tasks are ready
     * to be executed.
     */
    void* cpuTasksCond;

    /**
     * The threads used to execute tasks, in addition to the main thread.
     */
    vector<void*> threads;

    /**
     * Target frame duration in micro seconds, or 0 if no fixed framerate.
     */
    float framePeriod;

    /**
     * Minimum number of prefetching tasks to execute per frame (if prefetching
     * tasks are ready to be executed). Additionnal tasks can be executed if
     * there is a fixed framerate and if enough time remains until the deadline
     * for the current frame.
     */
    int prefetchRate;

    /**
     * Maximum number of prefetching tasks that can be waiting for execution.
     * When the queue size is reached, new prefetching tasks are discarded.
     */
    unsigned int prefetchQueueSize;

    /**
     * Time at the end of the last call to #run.
     */
    double lastFrame;

    /**
     * Logical time used for task completion dates. This logical time is a
     * counter incremented by one after each task execution.
     */
    unsigned int time;

    /**
     * True if this scheduler must be stopped. This is used in the destructor
     * to stop the execution threads in order to delete them properly.
     */
    bool stop;

    /**
     * The primitive tasks that must be executed at the current frame.
     */
    set< ptr<Task> > immediateTasks;

    /**
     * The primitive CPU or GPU tasks that are ready to be executed. A task is
     * ready to be executed when all its predecessor tasks are completed.
     */
    SortedTaskSet allReadyTasks;

    /**
     * The primitive CPU tasks that are ready to be executed. A task is ready to
     * be executed when all its predecessor tasks are completed.
     */
    SortedTaskSet readyCpuTasks;

    /**
     * The predecessors of the tasks that remain to be executed.
     */
    map< ptr<Task>, set< ptr<Task> > > dependencies;

    /**
     * The successors of the tasks that remain to be executed.
     */
    map< ptr<Task>, set< ptr<Task> > > inverseDependencies;

    /**
     * The prefetching tasks that remain to be executed.
     */
    set< ptr<Task> > prefetchQueue;

    /**
     * The task classes whose execution time must be monitored (debug).
     */
    vector<string> monitoredTasks;

    /**
     * The statistics per frame for monitored task classes. For each frame and
     * for each task type, this map gives the number of tasks of this type that
     * have been executed, and the total execution time.
     */
    map< string, pair<int, float> > frameStatistics;

    /**
     * Buffered array of frame statistics, to avoid writing them to disk at
     * each frame.
     */
    float* bufferedStatistics;

    /**
     * Number of frame statistics buffered in #bufferedStatistics.
     */
    int bufferedFrames;

    /**
     * File to store task execution time statistics per frame for monitored tasks.
     */
    FILE *statisticsFile;

    /**
     * Adds all the primitive tasks of the given task to the set of tasks to be
     * executed. This method calls itself recursively on any TaskGraph, in order
     * to find all the primitive tasks, whatever their level of nesting inside
     * task graphs. It also creates all the needed dependencies with
     * #addFlattenedDependency.
     *
     * @param t the task whose primitive sub tasks must be added.
     * @param[in,out] addedTasks the already added tasks. This method adds the
     *      tasks it adds to this set.
     */
    void addFlattenedTask(ptr<Task> t, set< ptr<Task> > &addedTasks);

    /**
     * Adds all the primitive dependencies between the primitive first tasks of
     * src and the primitive last tasks of dst.
     *
     * @param src a task that must be executed after dst.
     * @param dst a task that must be execute before src.
     */
    void addFlattenedDependency(ptr<Task> src, ptr<Task> dst);

    /**
     * Sets the deadline of this task. This method ensures that the predecessors
     * of this task, and so on recursively, have a deadline that is less than
     * the deadline of this task. This method also updates the sorted sets that
     * may contain this task, since the task order depends on the task deadline.
     *
     * @param t a task.
     * @param deadline the new deadline for this task.
     * @param visited a set of tasks already visited by this method.
     */
    void setDeadline(ptr<Task> t, unsigned int deadline, set< ptr<Task> > &visited);

    /**
     * Updates the data structures after the execution of a task. This method
     * removes the given task from #dependencies and #inverseDependencies. This
     * can make new tasks ready to be executed, which are then added to
     * #allReadyTasks and #readyCpuTasks. Finally t.setIsDone(true) is called.
     *
     * @param t a completed task.
     * @param changes true if the task execution changed the result of its
     *      previous execution.
     */
    void taskDone(ptr<Task> t, bool changes);

    /**
     * The method executed by the additional threads of this scheduler. This
     * method contains an infinite loop that executes tasks when they are ready
     * to be executed. The method returns only when #stop is set to true.
     */
    void schedulerThread();

    /**
     * Writes the buffered frame statistics to the statisticsFile.
     */
    void clearBufferedFrames();

    /**
     * Static method needed by pthread to launch a thread. This method just
     * calls #schedulerThread on the MultithreadScheduler passed as argument.
     *
     * @param arg a MultithreadScheduler.
     */
    static void* schedulerThread(void* arg);

    /**
     * Returns a tasks from the given set with, if possible, the same execution
     * context as the given one.
     *
     * @param s a task set.
     * @param previousContext an execution context.
     * @return a task from s with, if possible, the given execution context. The
     *      returned task is not removed from the set.
     */
    static ptr<Task> getTask(SortedTaskSet &s, void *previousContext);

    /**
     * Inserts a task in the given set.
     *
     * @param s a task set.
     * @param t the task to be added in s.
     */
    static void insertTask(SortedTaskSet &s, ptr<Task> t);

    /**
     * Removes a task from the given set.
     *
     * @param s a task set.
     * @param t the task to be removed from s.
     * @return true if the set contained t.
     */
    static bool removeTask(SortedTaskSet &s, ptr<Task> t);
};

}

#endif
