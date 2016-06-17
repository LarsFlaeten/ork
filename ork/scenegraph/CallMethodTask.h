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

#ifndef _ORK_CALL_METHOD_TASK_H_
#define _ORK_CALL_METHOD_TASK_H_

#include "ork/scenegraph/AbstractTask.h"

namespace ork
{

/**
 * An AbstractTask to execute a Method on a SceneNode.
 * @ingroup scenegraph
 */
class ORK_API CallMethodTask : public AbstractTask
{
public:
    /**
     * Creates a new CallMethodTask.
     *
     * @param method a "node.method" qualified name. The first part specifies
     *      the scene node on which the method must be called. The second part
     *      specifies the name of the method that must be called.
     */
    CallMethodTask(const QualifiedName &method);

    /**
     * Deletes this CallMethodTask.
     */
    virtual ~CallMethodTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an empty CallMethodTask.
     */
    CallMethodTask();

    /**
     * Initializes this CallMethodTask.
     *
     * @param method a "node.method" qualified name. The first part specifies
     *      the scene node on which the method must be called. The second part
     *      specifies the name of the method that must be called.
     */
    void init(const QualifiedName &method);

    /**
     * Swaps this CallMethodTask with the given one.
     *
     * @param t a CallMethodTask.
     */
    void swap(ptr<CallMethodTask> t);

private:
    /**
     * A "node.method" qualified name. The first part specifies the scene node
     * on which the method must be called. The second part specifies the name of
     * the method that must be called.
     */
    QualifiedName method;
};

}

#endif
