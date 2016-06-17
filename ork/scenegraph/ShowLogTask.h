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

#ifndef _ORK_SHOW_LOG_TASK_H_
#define _ORK_SHOW_LOG_TASK_H_

#include "ork/scenegraph/ShowInfoTask.h"

namespace ork
{

/**
 * A ShowInfoTask sub class to display the ork::Logger messages.
 * @ingroup scenegraph
 */
class ORK_API ShowLogTask : public ShowInfoTask
{
public:
    /**
     * True if this task is enabled. When disabled the message logs are not
     * displayed.
     */
    static bool enabled;

    /**
     * Creates a new ShowLogTask.
     *
     * @param f the Font used to display Text.
     * @param p the program to be used to draw characters.
     * @param fontHeight the used font height.
     * @param pos x,y position and maximum number of lines of text to display.
     */
    ShowLogTask(ptr<Font> f, ptr<Program> p, float fontHeight, vec3i pos);

    /**
     * Deletes this ShowLogTask.
     */
    virtual ~ShowLogTask();

protected:
    /**
     * Creates an uninitialized ShowLogTask.
     */
    ShowLogTask();

    /**
     * Initializes this ShowLogTask.
     *
     * @param f the Font used to display Text.
     * @param p the program to be used to draw characters.
     * @param fontHeight the used font height.
     * @param pos x,y position and maximum number of lines of text to display.
     */
    virtual void init(ptr<Font> f, ptr<Program> p, float fontHeight, vec3i pos);

    virtual void draw(ptr<Method> context);
};

}

#endif
