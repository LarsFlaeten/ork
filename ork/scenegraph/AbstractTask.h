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

#ifndef _ORK_ABSTRACT_TASK_H_
#define _ORK_ABSTRACT_TASK_H_

#include "ork/taskgraph/TaskFactory.h"
#include "ork/scenegraph/SceneNode.h"

namespace ork
{

/**
 * An abstract task for a Method. A method "task" is in fact a TaskFactory that
 * creates Task. Indeed a new Task is created at each method invocation.
 *
 * @ingroup scenegraph
 */
class ORK_API AbstractTask : public TaskFactory
{
public:
    /**
     * Creates a new AbstractTask.
     *
     * @param type the task type.
     */
    AbstractTask(const char* type);

    /**
     * Deletes this AbstractTask.
     */
    virtual ~AbstractTask();

protected:
    /**
     * A qualified name of the form <i>target</i>.<i>name</i>.
     */
    struct ORK_API QualifiedName {

        /**
         * The first part of this qualified name. The first part is optional.
         * It can be "this", "$v" or any scene node flag.
         */
        string target;

        /**
         * The second part of this qualified name.
         */
        string name;

        /**
         * Creates an empty qualified name.
         */
        QualifiedName();

        /**
         * Creates a qualified name.
         *
         * @param n a qualified name of the form <i>target</i>.<i>name</i> or
         *      <i>name</i>.
         */
        QualifiedName(const string &n);

        /**
         * Returns the SceneNode designated by this qualified name.
         *
         * @param context the scene graph into which the target SceneNode must
         *      be looked for.
         */
        ptr<SceneNode> getTarget(ptr<SceneNode> context);
    };
};

}

#endif
