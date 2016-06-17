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

#ifndef _ORK_FILE_LOGGER_H_
#define _ORK_FILE_LOGGER_H_

#include <fstream>

#include "ork/core/Logger.h"

namespace ork
{

#define BUFFER_SIZE 1024

/**
 * A Logger that logs messages to an html file. This logger removes duplicated
 * messages when the same messages are logged at each frame, in order to avoid
 * very big log files.
 * @ingroup core
 */
class ORK_API FileLogger : public Logger
{
public:
    /**
     * A file for use by a FileLogger.
     */
    class ORK_API File
    {
    public:
        /**
         * Creates a new File with the given file name.
         */
        File(const std::string &name);

    private:
        /**
         * The stream used to write in this file.
         */
        std::fstream stream;

        /**
         * The file size just after the penultimate flush has been done.
         */
        unsigned int prev;

        /**
         * The file size just after the last flush has been done.
         */
        unsigned int cur;

        /**
         * The content added between the previous last two flushes.
         */
        char prevBuf[BUFFER_SIZE];

        /**
         * The content added since the last flush.
         */
        char curBuf[BUFFER_SIZE];

        /**
         * Returns true if the content added since the last flush is equal to
         * the content added between the previous last two flushes.
         */
        bool repetition();

        /**
         * Flushes the buffers of this file. If the content added since the last
         * flush is equal to the content between the last two flushes, then this
         * duplicated content is not written to the file. The goal is to avoid
         * very big log files when identical messages are logged at each frame.
         */
        void flush();

        friend class FileLogger;
    };

    /**
     * Creates a new FileLogger.
     * @param type the type of this logger.
     * @param out the File to which messages must be logged.
     * @param next the logger to which messages must be forwarded after logging by this logger.
     */
    FileLogger(const std::string &type, File *out, ptr<Logger> next = NULL);

    /**
     * Destroys this logger.
     */
    virtual ~FileLogger();

    /**
     * Writes the given message to the file used by this logger.
     */
    virtual void log(const std::string &topic, const std::string &msg);

    /**
     * Flushes the buffer of the file used by this logger.
     */
    virtual void flush();

protected:
    /**
     * The File to which this logger logs its messages.
     */
    File *out;

    /**
     * The logger to which messages are forwarded after logging by this logger.
     */
    ptr<Logger> next;
};

}

#endif
