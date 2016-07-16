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

#ifndef _ORK_EVENTHANDLER_H_
#define _ORK_EVENTHANDLER_H_

#include "ork/core/Object.h"

namespace ork
{

/**
 * A handler of user interface events (i.e. window, mouse and keyboard events).
 * By default nothing is done for any event. You must override this class to
 * provide a user interface behavior.
 *
 * @ingroup ui
 */
class ORK_API EventHandler : public Object
{
public:
    /**
     * Mouse buttons.
     */
    enum button {
        LEFT_BUTTON = 0, ///< left button
        MIDDLE_BUTTON = 1, ///< middle button
        RIGHT_BUTTON = 2 ///< right button
    };

    /**
     * Mouse wheel event
     */
    enum wheel {
        WHEEL_UP,  ///< wheel up event
        WHEEL_DOWN ///< wheel up event
    };

    /**
     * Mouse button states.
     */
    enum state {
        DOWN = 0, ///< down
        UP = 1 ///< up
    };

    /**
     * Modifier keys Shift, Ctrl and Alt.
     */
    enum modifier {
        NONE = 0,  // Lars F add. 2016
        SHIFT = 1, ///< shift
        CTRL = 2, ///< ctrl
        ALT = 4 ///< alt
    };

    /**
     * Special keys.
     */
    enum key {
        //KEY_F1 = 1, ///< F1
        //KEY_F2 = 2, ///< F2
        //KEY_F3 = 3, ///< F3
        //KEY_F4 = 4, ///< F4
        //KEY_F5 = 5, ///< F5
        //KEY_F6 = 6, ///< F6
        //KEY_F7 = 7, ///< F7
        //KEY_F8 = 8, ///< F8
        //KEY_F9 = 9, ///< F9
        //KEY_F10 = 10, ///< F10
        //KEY_F11 = 11, ///< F11
        //KEY_F12 = 12, ///< F12
        //KEY_LEFT = 100, ///< left arrow
        //KEY_UP = 101, ///< up arrow
        //KEY_RIGHT = 102, ///< right arrow
        //KEY_DOWN = 103, ///< down arrow
        //KEY_PAGE_UP = 104, ///< page up
        //KEY_PAGE_DOWN = 105, ///< page down
        //KEY_HOME = 106, ///< home
        //KEY_END = 107, ///< end
        //KEY_INSERT = 108, ///< insert
        KEY_ESCAPE =  256,
        KEY_ENTER =  257,
        KEY_TAB =  258,
        KEY_BACKSPACE =  259,
        KEY_INSERT =  260,
        KEY_DELETE =  261,
        KEY_RIGHT =  262,
        KEY_LEFT =  263,
        KEY_DOWN =  264,
        KEY_UP =  265,
        KEY_PAGE_UP =  266,
        KEY_PAGE_DOWN =  267,
        KEY_HOME =  268,
        KEY_END =  269,
        KEY_CAPS_LOCK =  280,
        KEY_SCROLL_LOCK =  281,
        KEY_NUM_LOCK =  282,
        KEY_PRINT_SCREEN =  283,
        KEY_PAUSE =  284,
        KEY_F1 =  290,
        KEY_F2 =  291,
        KEY_F3 =  292,
        KEY_F4 =  293,
        KEY_F5 =  294,
        KEY_F6 =  295,
        KEY_F7 =  296,
        KEY_F8 =  297,
        KEY_F9 =  298,
        KEY_F10 =  299,
        KEY_F11 =  300,
        KEY_F12 =  301,
        KEY_F13 =  302,
        KEY_F14 =  303,
        KEY_F15 =  304,
        KEY_F16 =  305,
        KEY_F17 =  306,
        KEY_F18 =  307,
        KEY_F19 =  308,
        KEY_F20 =  309,
        KEY_F21 =  310,
        KEY_F22 =  311,
        KEY_F23 =  312,
        KEY_F24 =  313,
        KEY_F25 =  314,
        KEY_KP_0 =  320,
        KEY_KP_1 =  321,
        KEY_KP_2 =  322,
        KEY_KP_3 =  323,
        KEY_KP_4 =  324,
        KEY_KP_5 =  325,
        KEY_KP_6 =  326,
        KEY_KP_7 =  327,
        KEY_KP_8 =  328,
        KEY_KP_9 =  329,
        KEY_KP_DECIMAL =  330,
        KEY_KP_DIVIDE  = 331,
        KEY_KP_MULTIPLY =  332,
        KEY_KP_SUBTRACT =  333,
        KEY_KP_ADD =  334,
        KEY_KP_ENTER =  335,
        KEY_KP_EQUAL =  336,
        KEY_LEFT_SHIFT =  340,
        KEY_LEFT_CONTROL =  341,
        KEY_LEFT_ALT =  342,
        KEY_LEFT_SUPER =  343,
        KEY_RIGHT_SHIFT =  344,
        KEY_RIGHT_CONTROL =  345,
        KEY_RIGHT_ALT =  346,
        KEY_RIGHT_SUPER =  347,
        KEY_MENU =  348,         
    };

    /**
     * Creates a new event handler.
     */
    EventHandler(const char *type);

    /**
     * Deletes this event handler.
     */
    virtual ~EventHandler();

    /**
     * This method is called when a window must be redrawn.
     *
     * @param t the current time in micro-seconds.
     * @param dt the elapsed time in micro-seconds since the last call to #redisplay.
     */
    virtual void redisplay(double t, double dt);

    /**
     * This method is called when a window is resized.
     *
     * @param x the new window width.
     * @param y the new window height.
     */
    virtual void reshape(int x, int y);

    /**
     * This method is called periodically when no event occur.
     *
     * @param damaged true if the window (partial) visibility has changed since
     *      the last call to this method. This occurs if the window is minized,
     *      maximized or if it is partially covered by another window.
     */
    virtual void idle(bool damaged);

    /**
     * This method is called when a mouse click occurs. The default
     * implementation of this method does nothing. It must be overriden to
     * provide the desired behaviour. Note that the mouse coordinates are
     * relative to the topleft corner of the window, with positive y towards the
     * bottom.
     *
     * @param b the button that was clicked.
     * @param s the %state of this button.
     * @param m the modifiers that were pressed when the click occured.
     * @param x the mouse x coordinate in pixels when the click occured.
     * @param y the mouse y coordinate in pixels when the click occured.
     * @return true if the event has been handled by this method.
     */
    virtual bool mouseClick(button b, state s, modifier m, int x, int y);

    /**
     * This method is called when a mouse wheel event occurs. The default
     * implementation of this method does nothing. It must be overriden to
     * provide the desired behaviour. Note that the mouse coordinates are
     * relative to the topleft corner of the window, with positive y towards the
     * bottom.
     *
     * @param b the wheel event that occured (WHEEL_UP or WHEEL_DOWN)
     * @param m the modifiers that were pressed when the wheel event occured.
     * @param x the mouse x coordinate in pixels when the wheel event occured.
     * @param y the mouse y coordinate in pixels when the wheel event occured.
     * @return true if the event has been handled by this method.
     */
    virtual bool mouseWheel(wheel b, modifier m, int x, int y);

    /**
     * This method is called periodically when the mouse is moved. Note that the
     * mouse coordinates are relative to the topleft corner of the window, with
     * positive y towards the bottom.
     *
     * @param x the mouse x coordinate in pixels.
     * @param y the mouse y coordinate in pixels.
     * @return true if the event has been handled by this method.
     */
    virtual bool mouseMotion(int x, int y);

    /**
     * This method is called periodically when the mouse is moved. Note that the
     * mouse coordinates are relative to the topleft corner of the window, with
     * positive y towards the bottom.
     *
     * @param x the mouse x coordinate in pixels.
     * @param y the mouse y coordinate in pixels.
     * @return true if the event has been handled by this method.
     */
    virtual bool mousePassiveMotion(int x, int y);

    /**
     * This method is called when a regular key is typed.
     *
     * @param c the typed character.
     * @param m the modifiers that were pressed when the key was typed.
     * @param x the mouse x coordinate in pixels when the key was typed.
     * @param y the mouse y coordinate in pixels when the key was typed.
     * @return true if the event has been handled by this method.
     */
    virtual bool keyTyped(unsigned char c, modifier m, int x, int y);

    /**
     * This method is called when a regular key is released.
     *
     * @param c the released character.
     * @param m the modifiers that were pressed when the key was released.
     * @param x the mouse x coordinate in pixels when the key was released.
     * @param y the mouse y coordinate in pixels when the key was released.
     * @return true if the event has been handled by this method.
     */
    virtual bool keyReleased(unsigned char c, modifier m, int x, int y);

    /**
     * This method is called when a special key is typed.
     *
     * @param k the typed key.
     * @param m the modifiers that were pressed when the key was typed.
     * @param x the mouse x coordinate in pixels when the key was typed.
     * @param y the mouse y coordinate in pixels when the key was typed.
     * @return true if the event has been handled by this method.
     */
    virtual bool specialKey(key  k, modifier m, int x, int y);

    /**
     * This method is called when a special key is released.
     *
     * @param k the released key.
     * @param m the modifiers that were pressed when the key was released.
     * @param x the mouse x coordinate in pixels when the key was released.
     * @param y the mouse y coordinate in pixels when the key was released.
     * @return true if the event has been handled by this method.
     */
    virtual bool specialKeyReleased(key  k, modifier m, int x, int y);
};

}

#endif
