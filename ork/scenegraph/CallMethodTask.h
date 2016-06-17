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
