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
    Logger(const std::string &type);

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
    void addTopic(const std::string &topic);

    /**
     * Returns true if messages of the given topic are logged by this logger.
     */
    bool hasTopic(const std::string &topic);

    /**
     * Logs a message given by its topic and its content. The default
     * implementation of this method sends the message to the standard error
     * output stream cerr.
     */
    virtual void log(const std::string &topic, const std::string &msg);

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
    const std::string type;

    /**
     * The topics handled by this logger, separated by semi-columns.
     * An empty string means that all topics are handled.
     */
    std::string topics;

    /**
     * A mutex to access this logger from multiple threads.
     */
    void *mutex;
};

}

#endif
