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

#include "ork/scenegraph/DrawMeshTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

DrawMeshTask::DrawMeshTask() : AbstractTask("DrawMeshTask")
{
}

DrawMeshTask::DrawMeshTask(const QualifiedName &mesh, int count) :
    AbstractTask("DrawMeshTask")
{
    init(mesh, count);
}

void DrawMeshTask::init(const QualifiedName &mesh, int count)
{
    this->mesh = mesh;
    this->count = count;
}

DrawMeshTask::~DrawMeshTask()
{
}

ptr<Task> DrawMeshTask::getTask(ptr<Object> context)
{
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    ptr<SceneNode> target = mesh.getTarget(n);
    ptr<MeshBuffers> m = NULL;
    if (target == NULL) {
        m = n->getOwner()->getResourceManager()->loadResource(mesh.name + ".mesh").cast<MeshBuffers>();
    } else {
        m = target->getMesh(mesh.name);
    }
    if (m == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("SCENEGRAPH", "DrawMesh : cannot find mesh '" + mesh.target + "." + mesh.name + "'");
        }
        throw exception();
    }
    return new Impl(m, count);
}

void DrawMeshTask::swap(ptr<DrawMeshTask> t)
{
    std::swap(mesh, t->mesh);
    std::swap(count, t->count);
}

DrawMeshTask::Impl::Impl(ptr<MeshBuffers> m, int count) :
    Task("DrawMesh", true, 0), m(m), count(count)
{
}

DrawMeshTask::Impl::~Impl()
{
}

bool DrawMeshTask::Impl::run()
{
    if (m != NULL) {
        if (Logger::DEBUG_LOGGER != NULL) {
            Resource *r = dynamic_cast<Resource*>(m.get());
            Logger::DEBUG_LOGGER->log("SCENEGRAPH", r == NULL ? "DrawMesk" : "DrawMesh '" + r->getName() + "'");
        }
        ptr<Program> prog = SceneManager::getCurrentProgram();
        if (m->nindices == 0) {
            SceneManager::getCurrentFrameBuffer()->draw(prog, *m, m->mode, 0, m->nvertices);
        } else {
            SceneManager::getCurrentFrameBuffer()->draw(prog, *m, m->mode, 0, m->nindices);
        }
    }
    return true;
}

/// @cond RESOURCES

class DrawMeshTaskResource : public ResourceTemplate<40, DrawMeshTask>
{
public:
    DrawMeshTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, DrawMeshTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,count,");
        string n = getParameter(desc, e, "name");
        int count = 1;
        const char* s = e->Attribute("count");
        if (s != NULL) {
            getIntParameter(desc, e, "count", &count);
        }
        init(QualifiedName(n), count);
    }
};

extern const char drawMesh[] = "drawMesh";

static ResourceFactory::Type<drawMesh, DrawMeshTaskResource> DrawMeshTaskType;

/// @endcond

}
