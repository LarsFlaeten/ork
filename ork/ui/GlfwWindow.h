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

#ifndef _ORK_GLFW_WINDOW_H_
#define _ORK_GLLFW_WINDOW_H_

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ork/math/vec2.h"
#include "ork/core/Timer.h"
#include "ork/ui/Window.h"

namespace ork
{

/**
 * A Window implemented using glfw.
 * (c) 2016 Lars Flæten
 */
class ORK_API GlfwWindow : public Window
{
public:
    /**
     * Creates a new window. If the window size is 0,0 a full screen window
     * is created.
     *
     * @param params the parameters of the window.
     */
    GlfwWindow(const Window::Parameters &params);

    /**
     * Deletes this window.
     */
    virtual ~GlfwWindow();

    virtual int getWidth() const;

    virtual int getHeight() const;

    virtual void start();

    virtual void redisplay(double t, double dt);

    virtual void reshape(int x, int y);

    virtual void idle(bool damaged);


    /**
     * Tells the windowing system wether to wait for a vertical
     * sync or not beofre swapping buffers.
     */
    void    waitForVSync(bool wait);

    /**
     * Returns the current mouse cursor position
     */
    void    getMousePosition(int* x, int* y);

private:
    /**
     * The Window instances. Maps window id to Window instances.
     */
    //static std::map<int, GlfwWindow*> INSTANCES;

    /**
     * The id of this window.
     */
    int windowId;

    /**
     * The glfw window handle
     * (A bit sluggish to use void*??)
     */
    void* glfwWindowHandle;

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
     * The handle to the VAO
     * Lars F addition 16.05.2016
     */
    GLuint vao;

    /**
     * Glfw callback that calls #redisplay on the active Window.
     */
    static void redisplayFunc();

    /**
     * Glfw callback that calls #reshape on the active Window.
     */
    static void reshapeFunc(GLFWwindow*, int w, int h);

    /**
     * Glfw callback that calls #idle on the active Window.
     */
    //static void idleFunc();

    /**
     * Glfw callback that calls #mouseClick on the active Window.
     */
    static void mouseClickFunc(GLFWwindow* window, int button, int action, int mods);

    /**
     *
     */
    static void scrollFunc(GLFWwindow* window, double xScroll, double yScroll);
    
    /**
     * Glfw callback that calls #mouseMotion on the active Window.
     */
    static void mouseMotionFunc(GLFWwindow* window, double x, double y);

    /**
     * Glfw callback that calls #mouseEnterLeave on the active Window.
     */
    static void mouseEnterLeaveFunc(GLFWwindow* window, int entered);

    /**
     * Key callback
     */
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * Glfw callback for focus events.
     */
    static void focusFunc(GLFWwindow* window, int focus);

    /**
     * Glfw error callback.
     */
    static void errorCallback(int error, const char* message);


    /**
     * Utility function to check wether shift, ctrl or alt
     * has been pressed
     */
    static EventHandler::modifier getModifiers(GLFWwindow* wd);


};

}

#endif
