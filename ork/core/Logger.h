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

#ifndef _ORK_LOGGER_H_
#define _ORK_LOGGER_H_

#include <string>
#include "ork/core/Object.h"

namespace ork
{

/**
 * A logger to log messages. Debug, info, warning and error messages must be
 * logged to the static #DEBUG_LOGGER, #INFO_LOGGER, #WARNING_LOGGER and
 * #ERROR_LOGGER. Each message has a topic. By default a logger logs all
 * messages, whatever their topic, but it is possible to restrict logging to
 * some topics only with #addTopic.
 * @ingroup core
 */
class ORK_API Logger : public Object
{
public:
    /**
     * The logger for debug messages.
     */
    static static_ptr<Logger> DEBUG_LOGGER;

    /**
     * The logger for information messages.
     */
    static static_ptr<Logger> INFO_LOGGER;

    /**
     * The logger for warning messages.
     */
    static static_ptr<Logger> WARNING_LOGGER;

    /**
     * The logger for error messages.
     */
    static static_ptr<Logger> ERROR_LOGGER;

    /**
     * Creates a new logger of the given type (debug, info, warning, etc).
     */
    Logger(const string &type);

    /**
     * Destroys this logger.
     */
    virtual ~Logger();

    /**
     * Adds the given topic to the list of topics managed by this logger.
     * By default a logger logs all messages, whatever their topic. But if at
     * least one topic is selected by this method, only messages whose topic
     * has been selected by this method will be logged.
     */
    void addTopic(const string &topic);

    /**
     * Returns true if messages of the given topic are logged by this logger.
     */
    bool hasTopic(const string &topic);

    /**
     * Logs a message given by its topic and its content. The default
     * implementation of this method sends the message to the standard error
     * output stream cerr.
     */
    virtual void log(const string &topic, const string &msg);

    /**
     * Formatted logging for more usability.
     * The problem is that Logger is not an ostream (or an ostringstream).
     * I tried to derive ostream but it requires extra work.
     * So here go C variadic functions.
     * Must have a char * topic since std::string are not POD and this does not work
     * with variable arguments list :|
     * So it's really a compromise.
     */
    void logf(const char * topic, const char *fmt, ...);

    /**
     * Flushes the buffer of this logger, if any.
     */
    virtual void flush();

protected:
    /**
     * The type of this logger (debug, info, warning, error, etc).
     */
    const string type;

    /**
     * The topics handled by this logger, separated by semi-columns.
     * An empty string means that all topics are handled.
     */
    string topics;

    /**
     * A mutex to access this logger from multiple threads.
     */
    void *mutex;
};

}

#endif
