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

#include <pthread.h>

#include "ork/scenegraph/ShowLogTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

void initInfoTask(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e,
    ptr<Font> &f, ptr<Program> &p, int &c, float &size, vec3i &pos);

class LogBuffer : public Object
{
public:
    static static_ptr<LogBuffer> INSTANCE;

    static ptr<LogBuffer> getInstance(int capacity = 256)
    {
        if (INSTANCE == NULL) {
            INSTANCE = new LogBuffer(capacity);
        }
        return INSTANCE;
    }

    enum type {
        DEBUG_LOG,
        INFO_LOG,
        WARNING_LOG,
        ERROR_LOG
    };

    bool hasNewErrors;

    LogBuffer(int capacity) :
        Object("LogBuffer"), hasNewErrors(false), capacity(capacity), first(0), size(0)
    {
        types = new type[capacity];
        lines = new string[capacity];
    }

    virtual ~LogBuffer()
    {
        delete[] types;
        delete[] lines;
    }

    int getSize()
    {
        return size;
    }

    type getType(int index)
    {
        return types[(first + index) % capacity];
    }

    string getLine(int index)
    {
        return lines[(first + index) % capacity];
    }

    void addLine(type t, const string &s)
    {
        if (size < capacity) {
            types[size] = t;
            lines[size] = s;
            size += 1;
        } else {
            types[first] = t;
            lines[first] = s;
            first = (first + 1) % capacity;
        }
        if (t == WARNING_LOG || t == ERROR_LOG) {
            hasNewErrors = true;
        }
    }

    void addText(type t, const string &s)
    {
        string line;
        for (unsigned int i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (c == '\n' || c == '\033') {
                addLine(t, line);
                line = "";
            } else {
                line = line + c;
            }
        }
    }

private:
    type *types;

    string *lines;

    int capacity;

    int first;

    int size;
};

static_ptr<LogBuffer> LogBuffer::INSTANCE;

class MemLogger : public Logger
{
public:
    MemLogger(const string &type, LogBuffer::type t, ptr<LogBuffer> buf, ptr<Logger> next = NULL) :
        Logger(type), type(t), buf(buf), next(next)
    {
    }

    virtual ~MemLogger()
    {
    }

    virtual void log(const string &topic, const string &msg)
    {
        if (next != NULL) {
            if (next->hasTopic(topic)) {
                pthread_mutex_lock((pthread_mutex_t*) mutex);
                buf->addText(type, "[" + topic + "] " + msg + '\n');
                pthread_mutex_unlock((pthread_mutex_t*) mutex);
            }
            next->log(topic, msg);
        }
    }

    virtual void flush()
    {
        if (next != NULL) {
            next->flush();
        }
    }

private:
    LogBuffer::type type;

    ptr<LogBuffer> buf;

    ptr<Logger> next;
};

bool ShowLogTask::enabled = false;

ShowLogTask::ShowLogTask() : ShowInfoTask()
{
}

ShowLogTask::ShowLogTask(ptr<Font> f, ptr<Program> p, float fontHeight, vec3i pos) :
    ShowInfoTask(f, p, 0, fontHeight, pos)
{
}

void ShowLogTask::init(ptr<Font> f, ptr<Program> p, float fontHeight, vec3i pos)
{
    static bool initialized = false;
    if (!initialized) {
        int capacity = 256;
        ptr<LogBuffer> buf = LogBuffer::getInstance(capacity);
        if (Logger::DEBUG_LOGGER != NULL) {
            Logger::DEBUG_LOGGER = new MemLogger("DEBUG_LOGGER", LogBuffer::DEBUG_LOG, buf, Logger::DEBUG_LOGGER);
        }
        Logger::INFO_LOGGER = new MemLogger("INFO", LogBuffer::INFO_LOG, buf, Logger::INFO_LOGGER);
        Logger::WARNING_LOGGER = new MemLogger("WARNING", LogBuffer::WARNING_LOG, buf, Logger::WARNING_LOGGER);
        Logger::ERROR_LOGGER = new MemLogger("ERROR", LogBuffer::ERROR_LOG, buf, Logger::ERROR_LOGGER);
        initialized = true;
    }
    ShowInfoTask::init(f, p, 0, fontHeight, pos);
}

ShowLogTask::~ShowLogTask()
{
}

void ShowLogTask::draw(ptr<Method> context)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("SCENEGRAPH", "ShowLog");
    }
    ptr<LogBuffer> buf = LogBuffer::getInstance();
    if (buf->hasNewErrors) {
        enabled = true;
        buf->hasNewErrors = false;
    }
    if (!enabled) {
        return;
    }
    ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
    fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, ADD, ZERO, ONE);

    fontMesh->clear();

    vec4f vp = fb->getViewport().cast<float>();

    float xs = (float) position.x;
    float ys = (float) ((position.y > 0) ? position.y : vp.w + position.y - min(buf->getSize(), position.z) * fontHeight);
    for (int l = max(0, buf->getSize() - position.z); l < buf->getSize(); ++l) {
        LogBuffer::type t = buf->getType(l);
        if (t == LogBuffer::DEBUG_LOG) {
            drawLine(vp, xs, ys, 0x00800000, buf->getLine(l));
        } else if (t == LogBuffer::INFO_LOG) {
            drawLine(vp, xs, ys, 0x00FF0000, buf->getLine(l));
        } else if (t == LogBuffer::WARNING_LOG) {
            drawLine(vp, xs, ys, 0xFFCC0000, buf->getLine(l));
        } else {
            drawLine(vp, xs, ys, 0xFF000000, buf->getLine(l));
        }
        ys += (float) fontHeight;
    }

    fb->draw(fontProgram, *fontMesh);
    fb->setBlend(false);
}

/// @cond RESOURCES

class ShowLogTaskResource : public ResourceTemplate<40, ShowLogTask>
{
public:
    ShowLogTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, ShowLogTask>(manager, name, desc)
    {
        ptr<Font> f;
        ptr<Program> p;
        int c;
        float fontHeight;
        vec3i pos;
        initInfoTask(manager, name, desc, e, f, p, c, fontHeight, pos);
        init(f, p, fontHeight, pos);
    }
};

extern const char showLog[] = "showLog";

static ResourceFactory::Type<showLog, ShowLogTaskResource> ShowLogTaskType;

/// @endcond

}
