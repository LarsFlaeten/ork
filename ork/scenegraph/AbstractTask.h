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
        std::string target;

        /**
         * The second part of this qualified name.
         */
        std::string name;

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
        QualifiedName(const std::string &n);

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
