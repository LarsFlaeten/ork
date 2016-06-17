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

#include "ork/scenegraph/Method.h"

#include "ork/scenegraph/SceneNode.h"

namespace ork
{

Method::Method(ptr<TaskFactory> body) : Object("Method"), enabled(true)
{
    taskFactory = body;
}

Method::~Method()
{
}

ptr<SceneNode> Method::getOwner()
{
    return owner;
}

bool Method::isEnabled()
{
    return enabled;
}

void Method::setIsEnabled(bool enabled)
{
    this->enabled = enabled;
}

ptr<TaskFactory> Method::getTaskFactory()
{
    return taskFactory;
}

void Method::setTaskFactory(ptr<TaskFactory> taskFactory)
{
    this->taskFactory = taskFactory;
}

ptr<Task> Method::getTask()
{
    return taskFactory->getTask(this);
}

}
