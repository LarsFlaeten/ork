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

#include "ork/scenegraph/AbstractTask.h"

#include "ork/scenegraph/SceneManager.h"

namespace ork
{

AbstractTask::AbstractTask(const char* type) :
    TaskFactory(type)
{
}

AbstractTask::~AbstractTask()
{
}

AbstractTask::QualifiedName::QualifiedName()
{
}

AbstractTask::QualifiedName::QualifiedName(const string &n)
{
    string::size_type i = n.find('.', 0);
    if (i != string::npos) {
        target = n.substr(0, i);
        name = n.substr(i + 1);
    } else {
        name = n;
    }
}

ptr<SceneNode> AbstractTask::QualifiedName::getTarget(ptr<SceneNode> context)
{
    if (target.size() == 0) {
        return NULL;
    } else if (strcmp(target.c_str(), "this") == 0) {
        return context;
    } else if (target[0] == '$') {
        return context->getOwner()->getNodeVar(target.substr(1));
    } else {
        SceneManager::NodeIterator i = context->getOwner()->getNodes(target);
        return i.hasNext() ? i.next() : NULL;
    }
}

}
