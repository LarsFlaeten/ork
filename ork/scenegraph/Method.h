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

#ifndef _ORK_METHOD_H_
#define _ORK_METHOD_H_

#include "ork/taskgraph/TaskFactory.h"

namespace ork
{

class SceneNode;

/**
 * A SceneNode method. A Method defines a behavior of a scene node. It can be a
 * basic task or a combination of basic tasks using sequences, loops or method
 * calls. The <i>body</i> of a method is TaskFactory that can be shared between
 * several methods. This TaskFactory is used to get the tasks to be run to
 * execute the method, depending on the context (the context passed to
 * TaskFactory#getTask being the Method itself, from which the owner SceneNode
 * can be found, and then then owner SceneManager).
 *
 * @ingroup scenegraph
 */
class ORK_API Method : public Object
{
public:
    /**
     * Creates a new Method using the given basic or compound task.
     *
     * @param body the method definition.
     */
    Method(ptr<TaskFactory> body);

    /**
     * Deletes this method.
     */
    virtual ~Method();

    /**
     * Returns the SceneNode to which this Method belongs.
     * See SceneNode#getMethod.
     */
    ptr<SceneNode> getOwner();

    /**
     * Returns true if this method is enabled. A call to disabled method is
     * skipped. A method is enabled by default.
     */
    bool isEnabled();

    /**
     * Enables or disables this method. A call to disabled method is skipped.
     *
     * @param enabled true to enable this method, false to disable it.
     */
    void setIsEnabled(bool enabled);

    /**
     * Returns the body of this method.
     */
    ptr<TaskFactory> getTaskFactory();

    /**
     * Sets the body of this method.
     *
     * @param taskFactory the new method body.
     */
    void setTaskFactory(ptr<TaskFactory> taskFactory);

    /**
     * Returns the Task to be run to execute this method.
     */
    ptr<Task> getTask();

private:
    /**
     * The SceneNode to which this Method belongs.
     */
    SceneNode *owner;

    /**
     * True if this method is enabled.
     */
    bool enabled;

    /**
     * The body of this method.
     */
    ptr<TaskFactory> taskFactory;

    friend class SceneNode;
};

}

#endif
