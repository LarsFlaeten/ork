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

#ifndef _ORK_GLUT_WINDOW_H_
#define _ORK_GLUT_WINDOW_H_

#include <map>

#include "ork/math/vec2.h"
#include "ork/core/Timer.h"
#include "ork/ui/Window.h"

namespace ork
{

/**
 * A Window implemented using GLUT.
 * @ingroup ui
 */
class ORK_API GlutWindow : public Window
{
public:
    /**
     * Creates a new window. If the window size is 0,0 a full screen window
     * is created.
     *
     * @param params the parameters of the window.
     */
    GlutWindow(const Window::Parameters &params);

    /**
     * Deletes this window.
     */
    virtual ~GlutWindow();

    virtual int getWidth() const;

    virtual int getHeight() const;

    virtual void start();

    virtual void redisplay(double t, double dt);

    virtual void reshape(int x, int y);

    virtual void idle(bool damaged);

private:
    /**
     * The Window instances. Maps window id to Window instances.
     */
    static map<int, GlutWindow*> INSTANCES;

    /**
     * The id of this window.
     */
    int windowId;

    /**
     * The current size of this window.
     */
    vec2i size;

    /**
     * True if this windows just gained focus.
     */
    bool damaged;

    /**
     * Timer used for computing the parameters of redisplay.
     */
    Timer timer;

    /**
     * The time at the end of the last execution of #redisplay.
     */
    double t;

    /**
     * The elapsed time bewteen the two previous calls to #redisplay.
     */
    double dt;

    /**
     * GLUT callback that calls #redisplay on the active Window.
     */
    static void redisplayFunc();

    /**
     * GLUT callback that calls #reshape on the active Window.
     */
    static void reshapeFunc(int x, int y);

    /**
     * GLUT callback that calls #idle on the active Window.
     */
    static void idleFunc();

    /**
     * GLUT callback that calls #mouseClick on the active Window.
     */
    static void mouseClickFunc(int button, int state, int x, int y);

    /**
     * GLUT callback that calls #mouseMotion on the active Window.
     */
    static void mouseMotionFunc(int x, int y);

    /**
     * GLUT callback that calls #mousePassiveMotion on the active Window.
     */
    static void mousePassiveMotionFunc(int x, int y);

    /**
     * GLUT callback that calls #keyTyped on the active Window.
     */
    static void keyboardFunc(unsigned char c, int x, int y);

    /**
     * GLUT callback that calls #keyReleased on the active Window.
     */
    static void keyboardUpFunc(unsigned char c, int x, int y);

    /**
     * GLUT callback that calls #specialKey on the active Window.
     */
    static void specialKeyFunc(int c, int x, int y);

    /**
     * GLUT callback that calls #specialKeyReleased on the active Window.
     */
    static void specialKeyUpFunc(int c, int x, int y);

    /**
     * GLUT callback for focus events.
     */
    static void focusFunc(int focus);
};

}

#endif
