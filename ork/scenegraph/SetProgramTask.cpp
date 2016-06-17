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

#include "ork/scenegraph/SetProgramTask.h"

#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

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
                name = name + dynamic_cast<Resource*>(&(*s))->getName() + ";";
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
            Resource *r = dynamic_cast<Resource*>(&(*p));
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
