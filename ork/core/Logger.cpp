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

#include "ork/core/Logger.h"

#include <iostream>

#include <pthread.h>
#include <cstdarg>
#include <cstdio>

using namespace std;

namespace ork
{

static_ptr<Logger> Logger::DEBUG_LOGGER(NULL);

static_ptr<Logger> Logger::INFO_LOGGER(new Logger("INFO"));

static_ptr<Logger> Logger::WARNING_LOGGER(new Logger("WARNING"));

static_ptr<Logger> Logger::ERROR_LOGGER(new Logger("ERROR"));

Logger::Logger(const string &type) : Object("Logger"), type(type)
{
    mutex = new pthread_mutex_t;
    pthread_mutex_init((pthread_mutex_t*) mutex, NULL);
}

Logger::~Logger()
{
    pthread_mutex_destroy((pthread_mutex_t*) mutex);
    delete (pthread_mutex_t*) mutex;
}

void Logger::addTopic(const string &topic)
{
    topics = topics + topic + ";";
}

bool Logger::hasTopic(const string &topic)
{
    return topics.size() == 0 || topics.find(topic, 0) != string::npos;
}

void Logger::log(const string &topic, const string &msg)
{
    if (hasTopic(topic)) {
        pthread_mutex_lock((pthread_mutex_t*) mutex);
        cerr << type << " [" << topic << "] " << msg << endl;
        pthread_mutex_unlock((pthread_mutex_t*) mutex);
    }
}

void Logger::logf(const char * topic, const char *fmt, ...)
{
    static const int MAX_LOG_SIZE = 512;
    char buf[MAX_LOG_SIZE];

    va_list vl;
    va_start(vl, fmt);
    vsnprintf(buf, MAX_LOG_SIZE, fmt, vl);
    va_end(vl);
    std::string topicStr = topic;
    std::string msgStr = buf;
    log(topicStr, msgStr);
}

void Logger::flush()
{
    cerr.flush();
}

}
