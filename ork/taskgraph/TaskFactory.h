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

#ifndef _ORK_TASK_GRAPH_FACTORY_H_
#define _ORK_TASK_GRAPH_FACTORY_H_

#include "ork/taskgraph/Task.h"

using namespace std;

namespace ork
{

/**
 * An object that can create Task.
 * @ingroup taskgraph
 */
class ORK_API TaskFactory : public Object
{
public:
    /**
     * Creates a new task factory.
     *
     * @param type the type of this factory.
     */
    TaskFactory(const char* type);

    /**
     * Deletes this task factory.
     */
    virtual ~TaskFactory();

    /**
     * Creates a new task.
     *
     * @param context an optional parameter to control the task creation.
     * @return the created task.
     */
    virtual ptr<Task> getTask(ptr<Object> context) = 0;
};

}

#endif
