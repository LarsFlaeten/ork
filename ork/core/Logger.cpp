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

#include "ork/core/Logger.h"

#include <iostream>

#include <pthread.h>
#include <cstdarg>
#include <cstdio>

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
