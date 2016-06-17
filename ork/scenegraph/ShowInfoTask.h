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

#ifndef _ORK_SHOW_INFO_TASK_H_
#define _ORK_SHOW_INFO_TASK_H_

#include <time.h>
#include "ork/render/Mesh.h"
#include "ork/render/Program.h"
#include "ork/scenegraph/AbstractTask.h"
#include "ork/util/Font.h"

namespace ork
{

/**
 * An AbstractTask to display the framerate and other information.
 * @ingroup scenegraph
 */
class ORK_API ShowInfoTask : public AbstractTask
{
public:
    /**
     * Creates a new ShowInfoTask.
     *
     * @param font the Font used to display Text.
     * @param p the program to be used to draw characters.
     * @param color the font color in RGBA8 format.
     * @param size the font height.
     * @param pos x,y position and maximum number of lines of text to display.
     */
    ShowInfoTask(ptr<Font> font, ptr<Program> p, int color, float size, vec3i pos);

    /**
     * Deletes this ShowInfoTask.
     */
    virtual ~ShowInfoTask();

    virtual ptr<Task> getTask(ptr<Object> context);

    /**
     * Adds an information to display. The information has a topic and replaces
     * the previous information in this topic. All the topics are cleared after
     * each frame (you have to set them at each frame if you want them to
     * persist on screen).
     *
     * @param topic the topic of the information.
     * @param info an information message.
     */
    static void setInfo(const std::string &topic, const std::string &info);

protected:
    /**
     * The mesh used to draw character quads, in order to display text.
     */
    static static_ptr< Mesh<Font::Vertex, unsigned int> > fontMesh;

    /**
     * The current information messages, associated with their topic.
     */
    static std::map<std::string, std::string> infos;

    /**
     * The program use to draw characters.
     */
    ptr<Program> fontProgram;

    /**
     * The uniform in #fontProgram used to control the font texture.
     */
    ptr<UniformSampler> fontU;

    /**
     * The Font used to display Text.
     */
    ptr<Font> font;

    /**
     * The font color in RGBA8 format.
     */
    int fontColor;

    /**
     * The used font height.
     */
    float fontHeight;

    /**
     * The x,y position and the maximum number of lines of text to be displayed.
     */
    vec3i position;

    /**
     * Creates an uninitialized ShowInfoTask.
     */
    ShowInfoTask();

    /**
     * Initializes this ShowInfoTask.
     *
     * @param font the Font used to display Text.
     * @param p the program to be used to draw characters.
     * @param color the font color in RGBA8 format.
     * @param size the font height.
     * @param pos x,y position and maximum number of lines of text to display.
     */
    virtual void init(ptr<Font> font, ptr<Program> p, int color, float size, vec3i pos);

    /**
     * Swaps this ShowInfoTask with another one.
     *
     * @param t a ShowInfoTask.
     */
    virtual void swap(ptr<ShowInfoTask> t);

    /**
     * Draws a line of text.
     *
     * @param vp the framebuffer viewport, in pixels.
     * @param xs the x coordinate of the first character to display.
     * @param ys the y coordinate of the first character to display.
     * @param color the color of this line of text, in RGBA8 format.
     * @param s the line of text to display.
     */
    virtual void drawLine(const vec4f &vp, float xs, float ys, int color, const std::string &s);

    /**
     * Draws the framerate and the information messages.
     *
     * @param context the method to which this task belongs.
     */
    virtual void draw(ptr<Method> context);

private:
    /**
     * The current framerate.
     */
    int fps;

    /**
     * The number of frames displayed since #start. This counter is periodically
     * reset to 0.
     */
    int frames;

    /**
     * The time at which the #frames counter was reset to 0.
     */
    double start;

    /**
     * A Task to display the framerate and other information.
     */
    class Impl : public Task
    {
    public:
        /**
         * Creates a new ShowInfoTask::Impl task.
         *
         * @param context the method to which 'source' belongs.
         * @param source the ShowInfoTask that created this task.
         */
        Impl(ptr<Method> context, ptr<ShowInfoTask> source);

        /**
         * Deletes this ShowInfoTask::Impl.
         */
        virtual ~Impl();

        virtual bool run();

    protected:
        virtual const std::type_info *getTypeInfo();

    private:
        /**
         * The method to which #source belongs.
         */
        ptr<Method> context;

        /**
         * The ShowInfoTask that created this task.
         */
        ptr<ShowInfoTask> source;
    };
};

}

#endif
