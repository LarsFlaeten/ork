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

#include "ork/scenegraph/LoopTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/taskgraph/TaskGraph.h"
#include "ork/scenegraph/SceneManager.h"
#include "ork/scenegraph/SequenceTask.h"

namespace ork
{

LoopTask::LoopTask() : AbstractTask("LoopTask")
{
}

LoopTask::LoopTask(const string &var, const string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask) :
    AbstractTask("LoopTask")
{
    init(var, flag, cull, parallel, subtask);
}

void LoopTask::init(const string &var, const string &flag, bool cull, bool parallel, ptr<TaskFactory> subtask)
{
    this->var = var;
    this->flag = flag;
    this->cull = cull;
    this->parallel = parallel;
    this->subtask = subtask;
}

LoopTask::~LoopTask()
{
}

ptr<Task> LoopTask::getTask(ptr<Object> context)
{
    ptr<SceneManager> manager = context.cast<Method>()->getOwner()->getOwner();

    vector< ptr<SceneNode> > nodes;
    SceneManager::NodeIterator i = manager->getNodes(flag);
    while (i.hasNext()) {
        ptr<SceneNode> n = i.next();
        if (!cull || n->isVisible) {
            nodes.push_back(n);
        }
    }

    if (nodes.size() == 1) {
        manager->setNodeVar(var, nodes[0]);
        return subtask->getTask(context);
    } else {
        ptr<TaskGraph> result = new TaskGraph();
        ptr<Task> prev = NULL;
        for (unsigned int i = 0; i < nodes.size(); ++i) {
            manager->setNodeVar(var, nodes[i]);
            try {
                ptr<Task> next = subtask->getTask(context);
                if (next.cast<TaskGraph>() == NULL || !next.cast<TaskGraph>()->isEmpty()) {
                    result->addTask(next);
                    if (!parallel && prev != NULL) {
                        result->addDependency(next, prev);
                    }
                    prev = next;
                }
            } catch (...) {
            }
        }
        return result;
    }
}

void LoopTask::swap(ptr<LoopTask> t)
{
    std::swap(var, t->var);
    std::swap(flag, t->flag);
    std::swap(cull, t->cull);
    std::swap(subtask, t->subtask);
}

/// @cond RESOURCES

class LoopTaskResource : public ResourceTemplate<40, LoopTask>
{
public:
    LoopTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, LoopTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "var,flag,culling,parallel,");
        string var = getParameter(desc, e, "var");
        string flag = getParameter(desc, e, "flag");
        bool cull = false;
        bool parallel = false;
        if (e->Attribute("culling") != NULL && strcmp(e->Attribute("culling"), "true") == 0) {
            cull = true;
        }
        if (e->Attribute("parallel") != NULL && strcmp(e->Attribute("parallel"), "true") == 0) {
            parallel = true;
        }
        vector< ptr<TaskFactory> > subtasks;
        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f != NULL) {
                ptr<TaskFactory> tf;
                tf = ResourceFactory::getInstance()->create(manager, "", desc, f).cast<TaskFactory>();
                subtasks.push_back(tf);
            }
            n = n->NextSibling();
        }
        if (subtasks.size() == 1) {
            init(var, flag, cull, parallel, subtasks[0]);
        } else {
            init(var, flag, cull, parallel, new SequenceTask(subtasks));
        }
    }
};

extern const char foreach[] = "foreach";

static ResourceFactory::Type<foreach, LoopTaskResource> LoopTaskType;

/// @endcond

}
