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

#include "ork/scenegraph/DrawMeshTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

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
            Resource *r = dynamic_cast<Resource*>(&(*m));
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
