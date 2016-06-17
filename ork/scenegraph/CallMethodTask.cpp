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

#include "ork/scenegraph/CallMethodTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/taskgraph/TaskGraph.h"
#include "ork/scenegraph/SceneManager.h"

namespace ork
{

CallMethodTask::CallMethodTask() : AbstractTask("CallMethodTask")
{
}

CallMethodTask::CallMethodTask(const QualifiedName &method) :
    AbstractTask("CallMethodTask")
{
    init(method);
}

void CallMethodTask::init(const QualifiedName &method)
{
    this->method = method;
}

CallMethodTask::~CallMethodTask()
{
}

ptr<Task> CallMethodTask::getTask(ptr<Object> context)
{
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    ptr<SceneNode> target = method.getTarget(n);
    if (target != NULL) {
        ptr<Method> m = target->getMethod(method.name);
        if (m != NULL) {
            if (m->isEnabled()) {
                return m->getTask();
            } else {
                return new TaskGraph();
            }
        }
    }
    if (Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->log("SCENEGRAPH", "CallMethod: cannot find method '" + method.target + "." + method.name + "'");
    }
    throw exception();
}

void CallMethodTask::swap(ptr<CallMethodTask> t)
{
    std::swap(method, t->method);
}

/// @cond RESOURCES

class CallMethodTaskResource : public ResourceTemplate<40, CallMethodTask>
{
public:
    CallMethodTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, CallMethodTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,");
        string n = getParameter(desc, e, "name");
        init(QualifiedName(n));
    }
};

extern const char callMethod[] = "callMethod";

static ResourceFactory::Type<callMethod, CallMethodTaskResource> CallMethodTaskType;

/// @endcond

}
