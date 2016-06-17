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

#ifndef _ORK_SET_PROGRAM_TASK_H_
#define _ORK_SET_PROGRAM_TASK_H_

#include "ork/render/Program.h"
#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to set a program.
 * @ingroup scenegraph
 */
class ORK_API SetProgramTask : public AbstractTask
{
public:
    /**
     * Creates a SetProgramTask.
     *
     * @param modules the modules of the program to be set. Each module is
     *      specified by a "node.module" qualified name. The first part
     *      specifies the scene node that contains the module. The second part
     *      specifies the name of the module in this node.
     * @param setUniforms true to set the uniforms of the program, using the
     *      values defined in the scene node from which this task is called.
     */
    SetProgramTask(const vector<QualifiedName> &modules, bool setUniforms);

    /**
     * Deletes this SetProgramTask.
     */
    virtual ~SetProgramTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an uninitialized SetProgramTask.
     */
    SetProgramTask();

    /**
     * Initializes this SetProgramTask.
     *
     * @param modules the modules of the program to be set. Each module is
     *      specified by a "node.module" qualified name. The first part
     *      specifies the scene node that contains the module. The second part
     *      specifies the name of the module in this node.
     * @param setUniforms true to set the uniforms of the program, using the
     *      values defined in the scene node from which this task is called.
     */
    void init(const vector<QualifiedName> &modules, bool setUniforms);

    /**
     * Swaps this SetProgramTask with the given one.
     *
     * @param t a SetProgramTask.
     */
    void swap(ptr<SetProgramTask> t);

private:
    /**
     * The modules of the program to be set. Each module is specified by a
     * "node.module" qualified name. The first part specifies the scene node
     * that contains the module. The second part specifies the name of the
     * module in this node.
     */
    vector<QualifiedName> modules;

    /**
     * True to set the uniforms of the program, using the values defined
     * in the scene node from which this task is called.
     */
    bool setUniforms;

    /**
     * A ork::Task to set a program.
     */
    class Impl : public Task
    {
    public:
        /**
         * The program to be set.
         */
        ptr<Program> p;

        /**
         * The scene node whose uniforms must be in #p.
         */
        ptr<SceneNode> n;

        /**
         * Creates a new SetProgramTask:Impl.
         *
         * @param p the program to be set.
         * @param n the scene node whose uniforms must be in #p.
         */
        Impl(ptr<Program> p, ptr<SceneNode> n);

        /**
         * Deletes this SetProgramTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
