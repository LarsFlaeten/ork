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

#include "ork/scenegraph/LoopTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/taskgraph/TaskGraph.h"
#include "ork/scenegraph/SceneManager.h"
#include "ork/scenegraph/SequenceTask.h"

using namespace std;

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
