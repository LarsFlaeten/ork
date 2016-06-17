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

#include "ork/taskgraph/Task.h"

#include "pmath.h"
#include <algorithm>
#include <sstream>

#include "ork/core/Logger.h"

#include <pthread.h>

using namespace std;

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
