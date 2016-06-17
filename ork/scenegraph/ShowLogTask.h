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
