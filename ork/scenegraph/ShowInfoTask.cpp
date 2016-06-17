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

#include "ork/scenegraph/ShowInfoTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

void initInfoTask(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e,
    ptr<Font> &f, ptr<Program> &p, int &c, float &size, vec3i &pos)
{
    e = e == NULL ? desc->descriptor : e;
    Resource::checkParameters(desc, e, "name,x,y,maxLines,font,fontSize,fontColor,fontProgram,");
    int x = 4;
    int y = -4;
    int maxLines = 8;
    vec4f color = vec4f(1.0f, 0.0f, 0.0f, 0.0f);

    string fontName = "defaultFont";
    if (e->Attribute("font") != NULL) {
        fontName = Resource::getParameter(desc, e, "font");
    }
    f = manager->loadResource(fontName).cast<Font>();

    size = f->getTileHeight();
    if (e->Attribute("fontSize") != NULL) {
        Resource::getFloatParameter(desc, e, "fontSize", &size);
    }

    if (e->Attribute("x") != NULL) {
        Resource::getIntParameter(desc, e, "x", &x);
    }
    if (e->Attribute("y") != NULL) {
        Resource::getIntParameter(desc, e, "y", &y);
    }
    if (e->Attribute("maxLines") != NULL) {
        Resource::getIntParameter(desc, e, "maxLines", &maxLines);
    }
    if (e->Attribute("fontColor") != NULL) {
        string c = string(e->Attribute("fontColor")) + ",";
        string::size_type start = 0;
        string::size_type index;
        for (int i = 0; i < 3; i++) {
            index = c.find(',', start);
            color[i] = (float) atof(c.substr(start, index - start).c_str()) / 255;
            start = index + 1;
        }
    }
    string fontProgram = "text;";
    if (e->Attribute("fontProgram") != NULL) {
        fontProgram = string(e->Attribute("fontProgram"));
    }
    p = manager->loadResource(fontProgram).cast<Program>();
    c = (int(color[0] * 255) & 0xFF) << 24;
    c |= (int(color[1] * 255) & 0xFF) << 16;
    c |= (int(color[2] * 255) & 0xFF) << 8;
    c |= int(color[3] * 255) & 0xFF;
    pos = vec3i(x, y, maxLines);
}

static_ptr< Mesh<Font::Vertex, unsigned int> > ShowInfoTask::fontMesh;

map<string, string> ShowInfoTask::infos;

ShowInfoTask::ShowInfoTask() : AbstractTask("ShowInfoTask")
{
}

ShowInfoTask::ShowInfoTask(ptr<Font> f, ptr<Program> p, int color, float size, vec3i pos) :
    AbstractTask("ShowInfoTask")
{
    init(f, p, color, size, pos);
}

void ShowInfoTask::init(ptr<Font> f, ptr<Program> p, int color, float size, vec3i pos)
{
    fps = 0;
    frames = 0;
    start = 0.0;
    fontProgram = p;
    fontU = p->getUniformSampler("font");
    font = f;
    fontColor = color;
    position = pos;
    fontHeight = size;
    if (fontMesh == NULL) {
        fontMesh = new Mesh<Font::Vertex, unsigned int>(TRIANGLES, GPU_DYNAMIC);
        fontMesh->addAttributeType(0, 4, A16F, false);
        fontMesh->addAttributeType(1, 4, A8UI, true);
    }
}

ShowInfoTask::~ShowInfoTask()
{
}

ptr<Task> ShowInfoTask::getTask(ptr<Object> context)
{
    return new Impl(context.cast<Method>(), this);
}

void ShowInfoTask::setInfo(const string &topic, const string &info)
{
    infos[topic] = info;
}

void ShowInfoTask::drawLine(const vec4f &vp, float xs, float ys, int color, const string &s)
{
    font->addLine(vp, xs, ys, s, fontHeight, color, fontMesh);
}

void ShowInfoTask::draw(ptr<Method> context)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("SCENEGRAPH", "ShowInfo");
    }

    ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
    fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, ADD, ZERO, ONE);

    vec4f vp = fb->getViewport().cast<float>();
    float xs = (float) position.x;
    float ys = position.y > 0 ? position.y : vp.w + position.y - position.z * fontHeight;

    ++frames;
    double current = context->getOwner()->getOwner()->getTime();
    double delay = (current - start) * 1e-6;
    if (delay > 1.0) {
        fps = int(frames / delay);
        frames = 0;
        start = current;
    } else if (delay < 0.0) {
        // happens when replaying recorded events
        fps = 0;
        frames = 0;
        start = current;
    }
    ostringstream os;
    map<string, string>::iterator i = infos.find("FPS");
    if (i == infos.end()) {
        os << fps << " FPS";
    } else {
        os << i->second << " FPS";
    }

    fontMesh->clear();
    drawLine(vp, xs, ys, fontColor, os.str());
    ys += fontHeight;

    i = infos.begin();
    while (i != infos.end()) {
        if (i->first != "FPS" && i->second.length() > 0) {
            drawLine(vp, xs, ys, fontColor, i->second);
            ys += fontHeight;
        }
        i++;
    }
    infos.clear();

    fontU->set(font->getImage());
    fb->draw(fontProgram, *fontMesh);

    fb->setBlend(false);

}

void ShowInfoTask::swap(ptr<ShowInfoTask> t)
{
    std::swap(fontProgram, t->fontProgram);
    std::swap(fontU, t->fontU);
    std::swap(font, t->font);
    std::swap(fontColor, t->fontColor);
    std::swap(fontHeight, t->fontHeight);
    std::swap(position, t->position);
    std::swap(fps, t->fps);
    std::swap(frames, t->frames);
    std::swap(start, t->start);
}

ShowInfoTask::Impl::Impl(ptr<Method> context, ptr<ShowInfoTask> source) :
    Task("ShowInfo", true, 0), context(context), source(source)
{
}

ShowInfoTask::Impl::~Impl()
{
}

bool ShowInfoTask::Impl::run()
{
    source->draw(context);
    return true;
}

const type_info *ShowInfoTask::Impl::getTypeInfo()
{
    return &typeid(*source);
}

/// @cond RESOURCES

class ShowInfoTaskResource : public ResourceTemplate<40, ShowInfoTask>
{
public:
    ShowInfoTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, ShowInfoTask>(manager, name, desc)
    {
        ptr<Font> f;
        ptr<Program> p;
        int c;
        vec3i pos;
        float size;
        initInfoTask(manager, name, desc, e, f, p, c, size, pos);
        init(f, p, c, size, pos);
    }
};

extern const char showInfo[] = "showInfo";

static ResourceFactory::Type<showInfo, ShowInfoTaskResource> ShowInfoTaskType;

/// @endcond

}
