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

#include "ork/core/FileLogger.h"

#include <ctime>

#include <pthread.h>
#include <string.h>

namespace ork
{

FileLogger::File::File(const string &name) : prev(0), cur(0)
{
    stream.open(name.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
    //stream.exceptions(fstream::eofbit | fstream::failbit | fstream::badbit);
    stream << "<html>\n";
    stream << "<head>\n";
    stream << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
    stream << "<title>Ork Log</title>\n";
    stream << "<style type=\"text/css\">\n";
    stream << "body, html { background: #EEEEEE; color: #000000; font-family: sans-serif; }\n";
    stream << ".DATE { background: #DDDDDD; color: #333333; vertical-align: top; }\n";
    stream << ".DEBUG { color: #888888; }\n";
    stream << ".WARNING { color: #CC9900; }\n";
    stream << ".ERROR { color: #CC0000; }\n";
    stream << "</style>\n";
    stream << "</head>\n";
    stream << "<body>\n";
    stream << "<table cellspacing=\"0\" cellpadding=\"2\">\n";
    stream.flush();
}

bool FileLogger::File::repetition()
{
    unsigned int n = min((unsigned int) BUFFER_SIZE, (unsigned int) (cur - prev));
    stream.seekg(prev, ios::beg);
    stream.read(prevBuf, n);
    stream.seekg(cur, ios::beg);
    stream.read(curBuf, n);
    stream.tellg();
    return strncmp(prevBuf, curBuf, n) == 0;
}

void FileLogger::File::flush()
{
    unsigned int pos = (unsigned int) stream.tellp();
    if (prev == 0) {
        prev = pos;
    } else if (cur == 0) {
        cur = pos;
    } else {
        unsigned int prevSize = cur - prev;
        unsigned int curSize = pos - cur;
        if (curSize > 0 && curSize == prevSize && repetition()) {
            stream.seekp(cur, ios::beg);
        } else {
            prev = cur;
            cur = pos;
        }
    }
}

FileLogger::FileLogger(const string &type, File *out, ptr<Logger> next) :
    Logger(type), out(out), next(next)
{
}

FileLogger::~FileLogger()
{
    fstream &o = out->stream;
    if (o.is_open()) {
        o << "</table>\n";
        o << "</body>\n";
        o << "</html>\n";
        o.close();
    }
}

void FileLogger::log(const string &topic, const string &msg)
{
    if (hasTopic(topic)) {
        time_t rawtime;
        char timestring[256];

        time(&rawtime);
#ifdef _MSC_VER
        struct tm timeinfo;
        localtime_s(&timeinfo, &rawtime);
        strftime(timestring, 256, "%H:%M:%S", &timeinfo);
#else
        struct tm *timeinfo = localtime(&rawtime);
        strftime(timestring, 256, "%H:%M:%S", timeinfo);
#endif

        pthread_mutex_lock((pthread_mutex_t*) mutex);
        fstream &o = out->stream;
        o << "<tr><td class=\"DATE\">" << timestring << "</td>\n";
        o << "<td class=\"" << type << "\">[" << topic << "] ";
        bool pre = false;
        bool bold = false;
        for (string::const_iterator it = msg.begin(); it < msg.end(); it++) {
            char c = *it;
            if (c == '\033') {
                o << (pre ? "</pre>" : "<pre>");
                pre = !pre;
            } else if (c == '\'') {
                o << (bold ? "</b>" : "<b>");
                bold = !bold;
            } else if (c == '<') {
                o << "&lt;";
            } else if (c == '>') {
                o << "&gt;";
            } else {
                o << c;
            }
        }
        o << "</td></tr>\n";
        o.flush();
        pthread_mutex_unlock((pthread_mutex_t*) mutex);
    }

    if (next != NULL) {
        next->log(topic, msg);
    }
}

void FileLogger::flush()
{
    out->flush();
}

}
