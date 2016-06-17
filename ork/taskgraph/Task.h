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

#ifndef _ORK_TASK_H_
#define _ORK_TASK_H_

#include <typeinfo>
#include <vector>
#include <map>
#include <set>

#include "ork/core/Object.h"

using namespace std;

namespace ork
{

/**
 * @defgroup taskgraph taskgraph
 * @ingroup ork
 * Provides a task graph description and scheduling framework.
 */

class TaskListener;

/**
 * An abstract Task. A task can be a CPU or GPU task, it has a deadline measured
 * as the frame number before which the task must be done. A task also has a
 * complexity, which is used to predict the duration of this task from the
 * measure of the duration of previous tasks of the same type. A task can be
 * made of several tasks organized in a graph (see TaskGraph). Finally a GPU task
 * can have an execution context (for instance an OpenGL %state) which must be
 * setup before the task is run. In order to reduce the number of context
 * switches, the context setup and cleanup are isolated in the
 * #begin and #end methods, while the task itself is implemented in the #run
 * method. The context itself is returned by the #getContext method, which
 * allows tasks that share the same context to be executed in a group. For
 * instance if t1, t2 and t3 are GPU tasks with the same context, they can be
 * executed with t1.begin, t1.run, t2.run, t3.run and t3.end instead of t1.begin,
 * t1.run, t1.end, t2.begin, t2.run, t2.end, t3.begin, t3.run, and t3.end, which
 * saves two context switches.
 *
 * @ingroup taskgraph
 */
class ORK_API Task : public Object
{
public:
    /**
     * Possible reasons for which a task must be reexecuted.
     */
    enum reason {
        DEPENDENCY_CHANGED, ///< data used by this task and produced by a predecessor task has changed
        DATA_CHANGED, ///< data used by this task but not produced by another task has changed
        DATA_NEEDED ///< result of this task is needed again by a successor task of this task
    };

    /**
     * Creates a new task.
     *
     * @param type the type of the task.
     * @param gpuTask if the task must be executed on GPU.
     * @param deadline the frame number before which the task must be executed.
     *      0 means that the task must be executed immediately.
     */
    Task(const char *type, bool gpuTask, unsigned int deadline);

    /**
     * Deletes this task.
     */
    virtual ~Task();

    /**
     * Returns the execution context of this task. This context is used to sort
     * GPU tasks that share the same context, in order to save context switches.
     * The context is unused for CPU tasks, and can be NULL.
     */
    virtual void* getContext() const;

    /**
     * Returns true if this task is a GPU task.
     */
    bool isGpuTask() const;

    /**
     * Returns the frame number before which this task must be completed.
     */
    unsigned int getDeadline() const;

    /**
     * Sets the frame number before which this task must be completed. <i>For
     * internal use only</i>. This method is called by schedulers, it must not
     * called directly by users.
     */
    void setDeadline(unsigned int deadline);

    /**
     * Returns the complexity of this task. This number is used to estimate the
     * duration d of this task as d=k*complexity, where k is estimated based on
     * the actual duration and complexity of previous tasks of the same type
     * (see #getTypeInfo).
     */
    virtual int getComplexity() const;

    /**
     * Prepares this task before its execution. This method is called when the
     * task is scheduled to be executed. It can perform work that cannot be
     * executed during the task execution itself, such as scheduling new tasks
     * for execution (indeed, once a set of tasks has been scheduled for
     * execution, this set cannot be modified, i.e. a task cannot schedule the
     * execution of a task that was not previously in this set). The default
     * implementation of this task does nothing.
     *
     * @param initialized the tasks already initialized. This set is used to
     *      avoid initializing several times the same task (for tasks that
     *      belong to several task graphs).
     */
    virtual void init(set<Task*> &initialized);

    /**
     * Sets the execution context for this task. All tasks that share the same
     * execution context must do the same work in this method, i.e. if t1 and t2
     * return the same object in #getContext, then t1.begin and t2.begin should
     * be equivalent.
     */
    virtual void begin();

    /**
     * Executes this task.
     *
     * @return true if the result of this execution is different from the result
     *      of the last execution of this task. Indeed a task can be executed
     *      several times (see Scheduler#reschedule), and its result may or may
     *      not change at each execution. If the result does not change, then
     *      a task that depends on this task will <i>not</i> be reexecuted if
     *      its own data has not changed (i.e. if the reason for its
     *      rescheduling was DEPENDENCY_CHANGED -- we assume in this framework
     *      that the result of any task is deterministic and depends only on its
     *      own data and on the result of the predecessor tasks). The default
     *      implementation of this method returns true, which is the safe
     *      default result (returning true even if the result of the task has
     *      not changed is not a problem, but returning false while the result
     *      <i>has</i> changed will cause problems).
     */
    virtual bool run();

    /**
     * Restores the execution context after this task. All tasks that share the
     * same execution context must do the same work in this method, i.e. if t1
     * and t2 return the same object in #getContext, then t1.end and t2.end
     * should be equivalent.
     */
    virtual void end();

    /**
     * Returns true if this task is completed.
     */
    virtual bool isDone();

    /**
     * Sets the execution %state of this task. If the task is completed and its
     * execution %state is set to "not done" then it will be executed again.
     * <i>For internal use only</i>. This method is called by schedulers, it
     * must not called directly by users.
     *
     * @param done true if task is completed, false otherwise.
     * @param t if done is true, the task's completion date (a frame number).
     * @param r if done is false, indicates why the task must be reexecuted.
     */
    virtual void setIsDone(bool done, unsigned int t, reason r = DATA_NEEDED);

    /**
     * Returns the time at which this task was completed. This completion date
     * is not reinitialized when the task is marked as not done, to force its
     * reexecution. It is not changed either if the task result does not change
     * after a reexecution (see #run). Hence this date gives the <i>last
     * modification date</i> of the result of this task.
     */
    unsigned int getCompletionDate();

    /**
     * Returns the last completion date of the predecessors of this task.
     */
    unsigned int getPredecessorsCompletionDate();

    /**
     * Sets the last completion date of the predecessors of this task. <i>For
     * internal use only</i>. This method is called by schedulers, it must not
     * called directly by users.
     *
     * @param t the completion date of a predecessor task of this task.
     */
    virtual void setPredecessorsCompletionDate(unsigned int t);

    /**
     * Returns the expected duration of this task in micro seconds. The result
     * is based on the complexity of this task (see #getComplexity).
     */
    float getExpectedDuration();

    /**
     * Sets the actual duration of this task. This actual duration is used to
     * improve the estimator for the duration of tasks of this type (see
     * #getTypeInfo). <i>For internal use only</i>. This method is called by
     * schedulers, it must not called directly by users.
     *
     * @param duration the actual duration of this task in micro seconds.
     */
    void setActualDuration(float duration);

    /**
     * Adds a listener to this task.
     *
     * @param l a task listener.
     */
    void addListener(TaskListener *l);

    /**
     * Removes a listener from this task.
     *
     * @param l a task listener.
     */
    void removeListener(TaskListener *l);

    /**
     * Logs the statistics about the execution time of the tasks, depending on
     * their type.
     */
    static void logStatistics();

protected:
    unsigned int completionDate; ///< time at which this task was completed.

    vector<TaskListener*> listeners; ///< the listeners of this tasks.

    /**
     * Returns the type of this task. This type is used to group the execution
     * time statistics of tasks of the same type.
     */
    virtual const type_info *getTypeInfo();

private:
    /**
     * Sort operator for type_info objects.
     */
    struct TypeInfoSort : public less<type_info*>
    {
        bool operator()(const type_info *x, const type_info *y) const;
    };

    /**
     * Execution time statistics for tasks of a given type.
     */
    struct TaskStatistics
    {
        float durationSum; ///< sum of the execution times.

        float durationSquareSum; ///< sum of the squares of the execution times.

        float minDuration; ///< minimum execution time.

        float maxDuration; ///< maximum execution time.

        bool corrected; ///< true if min and max values have been removed from sum and squareSum

        int n; ///< number of executions.

        TaskStatistics();
    };

    bool gpuTask; ///< true is this task is a GPU task.

    unsigned int deadline; ///< frame number before which this tasks must be completed.

    unsigned int predecessorsCompletionDate; ///< last completion date of the predecessors of this task.

    bool done; ///< true is the task is completed.

    float expectedDuration; ///< expected duration of this task.

    static void* mutex; ///< mutex used to synchronize accesses to #statistics

    /**
     * The execution time statistics for each task type. Maps TaskStatistics to
     * type_info objects.
     */
    static map<type_info const*, TaskStatistics*, TypeInfoSort> statistics;
};

/**
 * A task listener, notified when changes occur in a task.
 */
class ORK_API TaskListener
{
public:
    /**
     * Notifies this listener that the execution state of the given task has
     * changed.
     *
     * @param t the task whose execution state has changed.
     * @param done the new execution state.
     * @param r if done is false, the reason why the task must be reexecuted.
     */
    virtual void taskStateChanged(ptr<Task> t, bool done, Task::reason r) = 0;

    /**
     * Notifies this listener that the completion date of the given task has
     * changed.
     *
     * @param t the task whose completion date has changed.
     * @param date the new completion date.
     */
    virtual void completionDateChanged(ptr<Task> t, unsigned int date) = 0;
};

}

#endif
