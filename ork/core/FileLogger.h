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
        File(const string &name);

    private:
        /**
         * The stream used to write in this file.
         */
        fstream stream;

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
    FileLogger(const string &type, File *out, ptr<Logger> next = NULL);

    /**
     * Destroys this logger.
     */
    virtual ~FileLogger();

    /**
     * Writes the given message to the file used by this logger.
     */
    virtual void log(const string &topic, const string &msg);

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
