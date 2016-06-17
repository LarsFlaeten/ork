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

#include "ork/core/FileLogger.h"

#include <ctime>

#include <pthread.h>
#include <string.h>

using namespace std;

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
