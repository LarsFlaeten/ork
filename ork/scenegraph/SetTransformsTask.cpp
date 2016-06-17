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

#include "ork/scenegraph/SetTransformsTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

namespace ork
{

SetTransformsTask::SetTransformsTask() : AbstractTask("SetTransformsTask")
{
}

SetTransformsTask::SetTransformsTask(const string &screen, QualifiedName m,
                                     const char *t, const char *ltow, const char *ltos,
                                     const char *ctow, const char *ctos, const char *stoc,
                                     const char *wtos, const char *wp, const char *wd) :
    AbstractTask("SetTransformsTask")
{
    init(screen, m, t, ltow, ltos, ctow, ctos, stoc, wtos, wp, wd);
}

void SetTransformsTask::init(const string &screen, QualifiedName m,
                             const char *t, const char *ltow, const char *ltos,
                             const char *ctow, const char *ctos, const char *stoc,
                             const char *wtos, const char *wp, const char *wd)
{
    this->screen = QualifiedName(screen + ".");
    this->m = m;
    this->t = t;
    this->ltow = ltow;
    this->ltos = ltos;
    this->ctow = ctow;
    this->ctos = ctos;
    this->stoc = stoc;
    this->wtos = wtos;
    this->wp = wp;
    this->wd = wd;
    this->time = NULL;
    this->localToWorld = NULL;
    this->localToScreen = NULL;
    this->cameraToWorld = NULL;
    this->cameraToScreen = NULL;
    this->screenToCamera = NULL;
    this->worldToScreen = NULL;
    this->worldPos = NULL;
    this->worldDir = NULL;
}

SetTransformsTask::~SetTransformsTask()
{
}

ptr<Task> SetTransformsTask::getTask(ptr<Object> context)
{
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    ptr<SceneNode> screenNode;
    if (ltos == NULL || wtos == NULL)
    {
        if (screen.target.size() > 0)
        {
            screenNode = screen.getTarget(n);
            if (screenNode == NULL)
            {
                if (Logger::ERROR_LOGGER != NULL)
                {
                    Logger::ERROR_LOGGER->log("SCENEGRAPH", "SetTransforms: cannot find screen node");
                }
                throw exception();
            }
        }
    }

    if (m.target.size() > 0 && module == NULL)
    {
        module = m.getTarget(n)->getModule(m.name);

        if (module == NULL)
        {
            if (Logger::ERROR_LOGGER != NULL)
            {
                Logger::ERROR_LOGGER->log("SCENEGRAPH", "SetTransforms: cannot find " + m.name + " node");
            }
            throw exception();
        }
    }
    return new Impl(screenNode, n, this);
}

void SetTransformsTask::swap(ptr<SetTransformsTask> t)
{
    std::swap(screen, t->screen);
    std::swap(module, t->module);
    std::swap(m, t->m);
    std::swap(this->t, t->t);
    std::swap(ltow, t->ltow);
    std::swap(ltos, t->ltos);
    std::swap(ctow, t->ctow);
    std::swap(ctos, t->ctos);
    std::swap(stoc, t->stoc);
    std::swap(wp, t->wp);
    std::swap(wd, t->wd);

    if (lastProg != NULL)
    {
        time = this->t == NULL ? NULL : lastProg->getUniform2f(this->t);
        localToWorld = ltow == NULL ? NULL : lastProg->getUniformMatrix4f(ltow);
        localToScreen = ltos == NULL ? NULL : lastProg->getUniformMatrix4f(ltos);
        cameraToWorld = ctow == NULL ? NULL : lastProg->getUniformMatrix4f(ctow);
        cameraToScreen = ctos == NULL ? NULL : lastProg->getUniformMatrix4f(ctos);
        screenToCamera = stoc == NULL ? NULL : lastProg->getUniformMatrix4f(stoc);
        worldToScreen = wtos == NULL ? NULL : lastProg->getUniformMatrix4f(wtos);
        worldPos = wp == NULL ? NULL : lastProg->getUniform3f(wp);
        worldDir = wd == NULL ? NULL : lastProg->getUniform3f(wd);
    }
}

SetTransformsTask::Impl::Impl(ptr<SceneNode> screenNode, ptr<SceneNode> context, ptr<SetTransformsTask> source) :
    Task("SetTransforms", true, 0), screenNode(screenNode), context(context), source(source)
{
}

SetTransformsTask::Impl::~Impl()
{
}

bool SetTransformsTask::Impl::run()
{
    if (Logger::DEBUG_LOGGER != NULL)
    {
        Logger::DEBUG_LOGGER->log("SCENEGRAPH", "SetTransforms");
    }

    ptr<Program> prog = NULL;
    if (source->module != NULL && !source->module->getUsers().empty())
    {
        prog = *(source->module->getUsers().begin());
    }
    else
    {
        prog = SceneManager::getCurrentProgram();
    }

    if (prog == NULL)
    {
        return true;
    }
    if (Logger::DEBUG_LOGGER != NULL)
    {
        Logger::DEBUG_LOGGER->logf("SCENEGRAPH", "SetTransforms %d", prog.get());
    }

    if (prog != source->lastProg)
    {
        source->time = source->t == NULL ? NULL : prog->getUniform2f(source->t);
        source->localToWorld = source->ltow == NULL ? NULL : prog->getUniformMatrix4f(source->ltow);
        source->localToScreen = source->ltos == NULL ? NULL : prog->getUniformMatrix4f(source->ltos);
        source->cameraToWorld = source->ctow == NULL ? NULL : prog->getUniformMatrix4f(source->ctow);
        source->cameraToScreen = source->ctos == NULL ? NULL : prog->getUniformMatrix4f(source->ctos);
        source->screenToCamera = source->stoc == NULL ? NULL : prog->getUniformMatrix4f(source->stoc);
        source->worldToScreen = source->wtos == NULL ? NULL : prog->getUniformMatrix4f(source->wtos);
        source->worldPos = source->wp == NULL ? NULL : prog->getUniform3f(source->wp);
        source->worldDir = source->wd == NULL ? NULL : prog->getUniform3f(source->wd);
        source->lastProg = prog;
    }

    if (source->time != NULL)
    {
        source->time->set(vec2f(context->getOwner()->getTime(), context->getOwner()->getElapsedTime()));
    }

    if (source->localToWorld != NULL)
    {
        source->localToWorld->setMatrix(context->getLocalToWorld().cast<float>());
    }

    if (source->localToScreen!= NULL)
    {
        if (source->screen.target.size() == 0)
        {
            source->localToScreen->setMatrix(context->getLocalToScreen().cast<float>());
        }
        else
        {
            mat4d ltow = context->getLocalToWorld();
            mat4d wtos = screenNode->getWorldToLocal();
            mat4f ltos = (wtos * ltow).cast<float>();
            source->localToScreen->setMatrix(ltos);
        }
    }

    if (source->cameraToWorld != NULL)
    {
        mat4d ctow = context->getOwner()->getCameraNode()->getLocalToWorld();
        source->cameraToWorld->setMatrix(ctow.cast<float>());
    }

    if (source->cameraToScreen != NULL)
    {
        mat4d ctos = context->getOwner()->getCameraToScreen();
        source->cameraToScreen->setMatrix(ctos.cast<float>());
    }

    if (source->screenToCamera != NULL)
    {
        mat4d ctos = context->getOwner()->getCameraToScreen();
        source->screenToCamera->setMatrix(ctos.inverse().cast<float>());
    }

    if (source->worldToScreen != NULL)
    {
        if (source->screen.target.size() == 0)
        {
            source->worldToScreen->setMatrix(context->getOwner()->getWorldToScreen().cast<float>());
        }
        else
        {
            mat4f wtos = screenNode->getWorldToLocal().cast<float>();
            source->worldToScreen->setMatrix(wtos);
        }
    }

    if (source->worldPos != NULL)
    {
        source->worldPos->set(context->getWorldPos().cast<float>());
    }

    if (source->worldDir != NULL)
    {
        vec4d d = context->getLocalToWorld() * vec4d::UNIT_Z;
        source->worldDir->set(vec3f((float) -d.x, (float) -d.y, (float) -d.z));
    }

    return true;
}

/// @cond RESOURCES

class SetTransformsTaskResource : public ResourceTemplate<40, SetTransformsTask>
{
public:
    SetTransformsTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, SetTransformsTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "screen,time,localToWorld,localToScreen,cameraToWorld,cameraToScreen,screenToCamera,module,worldToScreen,worldPos,worldDir,");

        const char* s = e->Attribute("screen");
        string screen = s == NULL ? "" : string(e->Attribute("screen"));

        s = e->Attribute("module");
        QualifiedName module = QualifiedName(s == NULL ? "" : string(s));

        const char *time = e->Attribute("time");
        const char *localToWorld = e->Attribute("localToWorld");
        const char *localToScreen = e->Attribute("localToScreen");
        const char *cameraToWorld = e->Attribute("cameraToWorld");
        const char *cameraToScreen = e->Attribute("cameraToScreen");
        const char *screenToCamera = e->Attribute("screenToCamera");
        const char *worldToScreen = e->Attribute("worldToScreen");
        const char *worldPos = e->Attribute("worldPos");
        const char *worldDir = e->Attribute("worldDir");
        init(screen, module, time, localToWorld, localToScreen, cameraToWorld, cameraToScreen, screenToCamera, worldToScreen, worldPos, worldDir);
    }
};

extern const char setTransforms[] = "setTransforms";

static ResourceFactory::Type<setTransforms, SetTransformsTaskResource> SetTransformsTaskType;

/// @endcond

}
