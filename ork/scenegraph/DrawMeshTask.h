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

    /**
     * Creates an empty DrawMeshTask.
     */
    DrawMeshTask();
protected:
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
