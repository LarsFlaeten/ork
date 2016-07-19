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

#include "ork/ui/GlfwWindow.h"

#include "ork/core/Logger.h"

//#include <GL/glew.h>

//#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdexcept>

#include "DebugCallback.h"

using namespace std;

//TODO:
//XX User pointer stores GLFW window instance, instead of static INSTANCE map
//-  Damaged / refresh callback?
//-  Mouse enter/leave callback?
//XX Enable adjusting swap interval
//XX Remove unused callbacks from glut
//-  General cleanup
//-  Enable fullscreen


namespace ork
{


GlfwWindow::GlfwWindow(const Parameters &params) : Window(params), glfwWindowHandle(NULL)
{
    // Implemented glfw user pointers:
    // http://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback

    // We start with allocating the error callback:
    glfwSetErrorCallback(errorCallback);

    // Initialise GLFW
    if( !glfwInit() )
    {
        Logger::ERROR_LOGGER->log("UI", "Could not init GLFW!");
        Logger::ERROR_LOGGER->flush();
        return;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    GLFWwindow* gwd = glfwCreateWindow(params.width(), params.height(), params.name().c_str(), NULL, NULL);
    glfwWindowHandle = (void*)gwd;

    if(glfwWindowHandle == NULL && Logger::ERROR_LOGGER != NULL)
    {
        Logger::ERROR_LOGGER->log("UI", "Could not creat GLFW window!");
        Logger::ERROR_LOGGER->flush();
        glfwTerminate();
        return;
    }


    // Here we get the actual size we got from glfw
    // May not be the same as given as "hint"?
    int width, height;
    glfwGetFramebufferSize(gwd, &width, &height);
    
    // We should also call reshape function, as GLUT does,
    // since some of the examples sets a few states based on this,
    // however since we are in CTOR now, the derived function will not
    // be calles. Examples that rely on setting states in reshape
    // must provide an initial value (See e.g. minimalglfw)..
    reshape(width, height);

    damaged = false;
    timer.start();
    t = 0.0;
    dt = 0.0;

    /*
     * TODO: Add possibility to go fullscreen
     */     

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(gwd, GLFW_STICKY_KEYS, GL_TRUE);
           


    // Set up callbacks
    glfwSetWindowSizeCallback(gwd, reshapeFunc);
    glfwSetWindowFocusCallback(gwd, focusFunc);
    glfwSetCursorPosCallback(gwd, mouseMotionFunc);
    glfwSetScrollCallback(gwd, scrollFunc);
    glfwSetKeyCallback(gwd, keyCallback);
    glfwSetMouseButtonCallback(gwd, mouseClickFunc);
/*
    // should be mouse enter/leave events,
    // but implemented in freeglut with get/loose focus
    glutEntryFunc(focusFunc);
*/

    // The last thing is to assign this as the user pointer:
    glfwSetWindowUserPointer(gwd, (void*)this);


    glewExperimental = GL_TRUE;
    glfwMakeContextCurrent(gwd);
    assert(glGetError() == 0);
    
    GLenum err = glewInit(); 
    // Check and flush glGetError(). We can often get an 1280 here,
    // which is tolerated, but should be caught here.
    GLenum errAfterGlewInit = glGetError();
    if (GLEW_OK != err || !(errAfterGlewInit==GL_NO_ERROR || errAfterGlewInit==GL_INVALID_ENUM))
    {
        /*  Problem: glewInit failed, something is seriously wrong. */
        if(Logger::ERROR_LOGGER != NULL)
        {
            Logger::ERROR_LOGGER->logf("UI", "Could not init GLEW, Error: %s", glewGetErrorString(err));
            Logger::ERROR_LOGGER->flush();
        }
        glfwTerminate();
        return;
 
    }
                                
        

    Logger::INFO_LOGGER->logf("UI", "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    Logger::INFO_LOGGER->flush(); 

    if (params.debug()) {
        assert(glDebugMessageCallbackARB != NULL);
        glDebugMessageCallbackARB(debugCallback, NULL);
    }
    
    
    // Lars F addtion 16.05.2016
    assert(glGetError()==0);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    assert(glGetError()==0);

    // do immeadiate swap
    this->waitForVSync(false);

}

GlfwWindow::~GlfwWindow()
{
    // Lars F addition 16.05.2016
    glDeleteVertexArrays(1, &vao);

}    



int GlfwWindow::getWidth() const
{
    return size.x;
}

int GlfwWindow::getHeight() const
{
    return size.y;
}

void GlfwWindow::getMousePosition(int* x, int* y)
{
    GLFWwindow* gwd = (GLFWwindow*)glfwWindowHandle;
    double xpos, ypos;
    glfwGetCursorPos(gwd, &xpos, &ypos);
    *x = static_cast<int>(xpos);
    *y = static_cast<int>(ypos);
}

void GlfwWindow::start()
{
    // Do the reshape call, as a lot of the ork/proland
    // apps are setting variables based on this:
    this->reshape(this->getWidth(), this->getHeight());
    
    do{
           
        this->redisplay(t, dt);

        glfwPollEvents();
        
        
        
    } while(glfwWindowShouldClose((GLFWwindow*)glfwWindowHandle)==0);

    glfwTerminate();
}

void GlfwWindow::redisplay(double t, double dt)
{
    glfwSwapBuffers((GLFWwindow*)glfwWindowHandle);
    double newT = timer.end();
    this->dt = newT - this->t;
    this->t = newT;
}

void GlfwWindow::reshape(int x, int y)
{
    size = vec2i(x, y);
}

void GlfwWindow::idle(bool damaged)
{
    //glutPostRedisplay();
}

void GlfwWindow::redisplayFunc()
{
    throw std::runtime_error("Not implemented");
    // TODO: modify for multiple windows
    //GlfwWindow* window = INSTANCES[1];
    ////window->redisplay(window->t, window->dt);
}

void GlfwWindow::reshapeFunc(GLFWwindow* window, int w, int h)
{
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    gw->reshape(w, h);
}

/*void GlfwWindow::idleFunc()
{
    
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    // glutLayerGet(GLUT_NORMAL_DAMAGED) is not implemented in freeglut
    //window->idle(glutLayerGet(GLUT_NORMAL_DAMAGED) == 1);
    gw->idle(window->damaged);
    gw->damaged = false;
}
*/
void GlfwWindow::mouseClickFunc(GLFWwindow* window, int mousebutton, int action, int mods)
{
    // Translate to Ork mouse messages
    EventHandler::button b;
    switch(mousebutton) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            b = EventHandler::RIGHT_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            b = EventHandler::LEFT_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            b = EventHandler::MIDDLE_BUTTON;
            break;
        default:
            break;
    }
    
    EventHandler::state s;
    if(action==GLFW_PRESS)
        s = EventHandler::DOWN;
    else if(action==GLFW_RELEASE)
        s = EventHandler::UP;

    // Check for modifier keys:
    EventHandler::modifier m = getModifiers(window);

    // Get the mous cordinates:
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);


    // Call the class method:
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    gw->mouseClick(b, s, m, static_cast<int>(xpos),static_cast<int>(ypos));


}

void GlfwWindow::scrollFunc(GLFWwindow* window, double scrollx,double scrolly)
{
    // Get modifiers:
    EventHandler::modifier m = getModifiers(window);

    // Mouse wheel provides y-scroll:
    // We discard x-scroll for now...
    EventHandler::wheel w;
    if(scrolly != 0.0) {

        if(scrolly < 0.0)
            w = WHEEL_DOWN; // Correct orientation?
        if(scrolly > 0.0)
            w = WHEEL_UP;

        // Get the mous cordinates:
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
          
          
        // Call the class method:
        GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        gw->mouseWheel(w, m, static_cast<int>(xpos),static_cast<int>(ypos));
    }
}

void GlfwWindow::mouseMotionFunc(GLFWwindow* window, double  x, double y)
{
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    // as the GLUT windows, we call mouseMotion when the mouse is moved
    // WHILE a button is clicked, an mousePassiveMotion when no buttons
    // are clicked.
    // I dont like this setup, and would prefer one motion func regardless
    // of button presses, but the ork/proland apps heavily use both of these
    // two methods, so I chose to implement them for GLFW as well
    
    // "Poll" the mouse buttons (actually not a poll, but queries
    // the caches state from glfw):
    int sl = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int sm = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    int sr = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (sl==GLFW_PRESS || sm==GLFW_PRESS || sr==GLFW_PRESS) {
        gw->mouseMotion(
            static_cast<int>(x),
            static_cast<int>(y));
    }
    else {
        gw->mousePassiveMotion(
            static_cast<int>(x),
            static_cast<int>(y));

    }
}

void GlfwWindow::mouseEnterLeaveFunc(GLFWwindow* window, int entered)
{
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    // TO BE IMPLEMENTED
}

void GlfwWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int     mods)
{
    // Get modifiers:
    EventHandler::modifier m = getModifiers(window);
    
    // Get the mous cordinates:
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    
    // Translate the key.
    // If 32 <= key <= 92, we pass through as a char
    if(key >= 32 && key <= 96)
    {
        char c = (unsigned char)key;

        if(action==GLFW_PRESS)
            gw->keyTyped(c, m,
                static_cast<int>(xpos),
                static_cast<int>(ypos));
        else
            gw->keyReleased(c, m,
                static_cast<int>(xpos),
                static_cast<int>(ypos));
    } else if(key >= 256 && key <= 348) 
    {
        // GLFWs 256 and up are special keys
        EventHandler::key k = static_cast<EventHandler::key>(key);
    
        if(action==GLFW_PRESS)
            gw->specialKey(k, m,
                static_cast<int>(xpos),
                static_cast<int>(ypos));
        else
            gw->specialKeyReleased(k, m,
                static_cast<int>(xpos),
                static_cast<int>(ypos));
    } else
        throw runtime_error("key decoder failed");
}



void GlfwWindow::focusFunc(GLFWwindow* window, int focus)
{
    GlfwWindow* gw = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));  
    gw->damaged = (focus != 0);

    // From glfw docs:
    if(focus){
        // Window got focus
    } else
    {
        // Window lost focus
    }
}

void GlfwWindow::errorCallback(int error, const char* message)
{
    throw std::runtime_error(message);
}

EventHandler::modifier GlfwWindow::getModifiers(GLFWwindow* window)
{
    int key1, key2; EventHandler::modifier m = EventHandler::NONE;
    key1 = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    key2 = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
    if(key1 == GLFW_PRESS || key2 == GLFW_PRESS)
        m = EventHandler::SHIFT;
                    
    key1 = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    key2 = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    if(key1 == GLFW_PRESS || key2 == GLFW_PRESS)
        m = EventHandler::CTRL;
                                   
    key1 = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    key2 = glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    if(key1 == GLFW_PRESS || key2 == GLFW_PRESS)
        m = EventHandler::ALT;

    return m;
}

void    GlfwWindow::waitForVSync(bool wait)
{
    if(wait)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

}

}
