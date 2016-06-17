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

#ifndef _ORK_DRAW_MESH_TASK_H_
#define _ORK_DRAW_MESH_TASK_H_

#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to draw a mesh. The mesh is drawn using the current
 * framebuffer and the current program.
 *
 * @ingroup scenegraph
 */
class ORK_API DrawMeshTask : public AbstractTask
{
public:
    /**
     * Creates a new DrawMeshTask.
     *
     * @param mesh a "node.mesh" qualified name. The first part specifies the
     *      scene node that contains the mesh. The second part specifies the
     *      name of the mesh in this node.
     * @param count the number of time this mesh must be drawn.
     */
    DrawMeshTask(const QualifiedName &mesh, int count = 1);

    /**
     * Deletes this DrawMeshTask.
     */
    virtual ~DrawMeshTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an empty DrawMeshTask.
     */
    DrawMeshTask();

    /**
     * Initializes this DrawMeshTask.
     *
     * @param mesh a "node.mesh" qualified name. The first part specifies the
     *      scene node that contains the mesh. The second part specifies the
     *      name of the mesh in this node.
     * @param count the number of time this mesh must be drawn.
     */
    void init(const QualifiedName &mesh, int count);

    /**
     * Swaps this DrawMeshTask with anoter one.
     *
     * @param t a DrawMeshTask.
     */
    void swap(ptr<DrawMeshTask> t);

private:
    /**
     * A "node.mesh" qualified name. The first part specifies the scene node
     * that contains the mesh. The second part specifies the name of the mesh in
     * this node.
     */
    QualifiedName mesh;

    /**
     * The number of time the mesh must be drawn.
     */
    int count;

    /**
     * A ork::Task to draw a mesh.
     */
    class Impl : public Task
    {
    public:
        /**
         * The mesh that must be drawn.
         */
        ptr<MeshBuffers> m;

        /**
         * The number of time #m must be drawn.
         */
        int count;

        /**
         * Creates a new DrawMeshTask::Impl task.
         *
         * @param m the mesh to be drawn.
         * @param count the number of time the mesh must be drawn.
         */
        Impl(ptr<MeshBuffers> m, int count);

        /**
         * Deletes this DrawMeshTask::Impl task.
         */
        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
