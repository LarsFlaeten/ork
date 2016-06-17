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

#include "ork/taskgraph/Task.h"

#include "pmath.h"
#include <algorithm>
#include <sstream>

#include "ork/core/Logger.h"

#include <pthread.h>

namespace ork
{

void *Task::mutex = NULL;

map<type_info const*, Task::TaskStatistics*, Task::TypeInfoSort> Task::statistics;

bool Task::TypeInfoSort::operator()(const type_info *x, const type_info *y) const
{
    return (*x).before(*y) != 0;
}

Task::TaskStatistics::TaskStatistics() :
    durationSum(0.0f), durationSquareSum(0.0f), minDuration(INFINITY), maxDuration(0.0f), corrected(false), n(0)
{
}

Task::Task(const char *type, bool gpuTask, unsigned int deadline) :
    Object(type), completionDate(0), gpuTask(gpuTask), deadline(deadline), predecessorsCompletionDate(1), done(false), expectedDuration(-1.0f)
{
    if (mutex == NULL) {
        mutex = new pthread_mutex_t;
        pthread_mutex_init((pthread_mutex_t*) mutex, NULL);
    }
}

Task::~Task()
{
}

void* Task::getContext() const
{
    return NULL;
}

bool Task::isGpuTask() const
{
    return gpuTask;
}

unsigned int Task::getDeadline() const
{
    return deadline;
}

void Task::setDeadline(unsigned int deadline)
{
    this->deadline = min(this->deadline, deadline);
}

int Task::getComplexity() const
{
    return 1;
}

void Task::init(set<Task*> &initialized)
{
}

void Task::begin()
{
}

bool Task::run()
{
    assert(!done);
    return true;
}

void Task::end()
{
}

bool Task::isDone()
{
    return done;
}

void Task::setIsDone(bool done, unsigned int t, reason r)
{
    if (this->done != done) {
        this->done = done;
        if (done || r != DEPENDENCY_CHANGED) {
            this->completionDate = t;
        }
        for (unsigned int i = 0; i < listeners.size(); ++i) {
            listeners[i]->taskStateChanged(this, done, r);
        }
    }
}

unsigned int Task::getCompletionDate()
{
    return completionDate;
}

unsigned int Task::getPredecessorsCompletionDate()
{
    return predecessorsCompletionDate;
}

void Task::setPredecessorsCompletionDate(unsigned int t)
{
    predecessorsCompletionDate = max(predecessorsCompletionDate, t);
}

float Task::getExpectedDuration()
{
    if (expectedDuration == -1.0f) {
        expectedDuration = 0.0f;
        const type_info *id = getTypeInfo();
        pthread_mutex_lock((pthread_mutex_t*) mutex);
        map<type_info const*, TaskStatistics*, TypeInfoSort>::iterator i = statistics.find(id);
        if (i != statistics.end()) {
            TaskStatistics *stats = i->second;
            // to get "valid" statistics, we wait until we have enough samples,
            // and we ignore the min and max values
            const int MIN_SAMPLES = 64;
            if (stats->n >= MIN_SAMPLES) {
                if (!stats->corrected) {
                    float sum = stats->durationSum - stats->maxDuration - stats->minDuration;
                    float squareSum = stats->durationSum - stats->maxDuration * stats->maxDuration - stats->minDuration * stats->minDuration;
                    stats->durationSum = (sum * stats->n) / (stats->n - 2) ;
                    stats->durationSquareSum = (squareSum * stats->n) / (stats->n - 2);
                    stats->corrected = true;
                }
                float mean = stats->durationSum / stats->n;
                float squareMean = stats->durationSquareSum / stats->n;
                float variance = squareMean - mean * mean;
                float standardDeviation = sqrt(variance);
                expectedDuration = (mean + 2.0f * standardDeviation) * getComplexity();
            }
        }
        pthread_mutex_unlock((pthread_mutex_t*) mutex);
    }
    return expectedDuration;
}

void Task::setActualDuration(float duration)
{
    TaskStatistics *stats;
    const type_info *id = getTypeInfo();
    pthread_mutex_lock((pthread_mutex_t*) mutex);
    map<type_info const*, TaskStatistics*, TypeInfoSort>::iterator i = statistics.find(id);
    if (i == statistics.end()) {
        stats = new TaskStatistics();
        statistics.insert(make_pair(id, stats));
    } else {
        stats = i->second;
    }
    duration = duration / getComplexity();
    stats->durationSum += duration;
    stats->durationSquareSum += duration * duration;
    stats->minDuration = min(duration, stats->minDuration);
    stats->maxDuration = max(duration, stats->maxDuration);
    stats->n += 1;
    pthread_mutex_unlock((pthread_mutex_t*) mutex);
}

const type_info *Task::getTypeInfo()
{
    return &typeid(*this);
}

void Task::addListener(TaskListener *l)
{
    listeners.push_back(l);
}

void Task::removeListener(TaskListener *l)
{
    vector<TaskListener*>::iterator i = find(listeners.begin(), listeners.end(), l);
    if (i != listeners.end()) {
        listeners.erase(i);
    }
}

void Task::logStatistics()
{
    pthread_mutex_lock((pthread_mutex_t*) mutex);
    map<type_info const*, TaskStatistics*, TypeInfoSort>::iterator i = statistics.begin();
    while (i != statistics.end()) {
        TaskStatistics *stats = i->second;
        float mean = stats->durationSum / stats->n;
        float squareMean = stats->durationSquareSum / stats->n;
        float variance = squareMean - mean * mean;
        float standardDeviation = sqrt(variance);

        ostringstream oss;
        oss.setf(ios::fixed,ios::floatfield);
        oss.precision(3);
        oss << i->first->name() << ": " << mean / 1000.0 << " +/- " << standardDeviation / 1000.0 << "; min/max " << stats->minDuration / 1000.0 << " " << stats->maxDuration / 1000.0;
        Logger::DEBUG_LOGGER->log("SCHEDULER", oss.str());
        i++;
    }
    pthread_mutex_unlock((pthread_mutex_t*) mutex);
}

}
