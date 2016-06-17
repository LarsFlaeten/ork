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
    static std::map<int, GlutWindow*> INSTANCES;

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
