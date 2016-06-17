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

#include "ork/scenegraph/SetProgramTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

SetProgramTask::SetProgramTask() : AbstractTask("SetProgramTask")
{
}

SetProgramTask::SetProgramTask(const vector<QualifiedName> &modules, bool setUniforms) :
    AbstractTask("SetProgramTask")
{
    init(modules, setUniforms);
}

void SetProgramTask::init(const vector<QualifiedName> &modules, bool setUniforms)
{
    this->modules = modules;
    this->setUniforms = setUniforms;
}

SetProgramTask::~SetProgramTask()
{
}

ptr<Task> SetProgramTask::getTask(ptr<Object> context)
{
    string name;
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    ptr<SceneManager> m = n->getOwner();
    ptr<Program> p;
    try {
        for (unsigned int i = 0; i < modules.size(); ++i) {
            ptr<SceneNode> target = modules[i].getTarget(n);
            if (target == NULL) {
                name = name + modules[i].name + ";";
            } else {
                ptr<Module> s = target->getModule(modules[i].name);
                name = name + dynamic_cast<Resource*>(s.get())->getName() + ";";
            }
        }
        // TODO sort name components!!!
        p = m->getResourceManager()->loadResource(name).cast<Program>();
    } catch (...) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("SCENEGRAPH", "SetProgram: cannot find program");
        }
        throw exception();
    }
    return new Impl(p, setUniforms ? n : NULL);
}

void SetProgramTask::swap(ptr<SetProgramTask> t)
{
    std::swap(modules, t->modules);
}

SetProgramTask::Impl::Impl(ptr<Program> p, ptr<SceneNode> n) :
    Task("SetProgram", true, 0), p(p), n(n)
{
}

SetProgramTask::Impl::~Impl()
{
}

bool SetProgramTask::Impl::run()
{
    if (p != NULL) {
        if (Logger::DEBUG_LOGGER != NULL) {
            Resource *r = dynamic_cast<Resource*>(p.get());
            Logger::DEBUG_LOGGER->log("SCENEGRAPH", r == NULL ? "SetProgram" : "SetProgram '" + r->getName() + "'");
        }
        if (n != NULL) {
            SceneNode::ValueIterator i = n->getValues();
            while (i.hasNext()) {
                ptr<Value> v = i.next();
                ptr<Uniform> u = p->getUniform(v->getName());
                if (u != NULL) {
                    u->setValue(v);
                }
            }
        }
        SceneManager::setCurrentProgram(p);
    }
    return true;
}

/// @cond RESOURCES

class SetProgramTaskResource : public ResourceTemplate<40, SetProgramTask>
{
public:
    SetProgramTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, SetProgramTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "setUniforms,");
        vector<QualifiedName> modules;
        bool setUniforms;
        if (e->Attribute("setUniforms") != NULL) {
            setUniforms = strcmp(e->Attribute("setUniforms"), "true") == 0;
        }
        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f != NULL) {
                if (strcmp(f->Value(), "module") != 0) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        Resource::log(Logger::ERROR_LOGGER, desc, f, "Invalid subelement '" + f->ValueStr() + "'");
                    }
                    throw exception();
                }
                checkParameters(desc, f, "name,");
                const char* moduleName = f->Attribute("name");
                if (moduleName == NULL) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        Resource::log(Logger::ERROR_LOGGER, desc, f, "Missing 'name' attribute");
                    }
                    throw exception();
                }
                modules.push_back(QualifiedName(string(moduleName)));
            }
            n = n->NextSibling();
        }
        init(modules, setUniforms);
    }
};

extern const char setProgram[] = "setProgram";

static ResourceFactory::Type<setProgram, SetProgramTaskResource> SetProgramTaskType;

/// @endcond

}
