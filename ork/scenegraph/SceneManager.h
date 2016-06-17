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

#ifndef _ORK_SCENE_MANAGER_H_
#define _ORK_SCENE_MANAGER_H_

#include "ork/resource/ResourceManager.h"
#include "ork/taskgraph/Scheduler.h"
#include "ork/scenegraph/SceneNode.h"

namespace ork
{

/**
 * A manager to manage a scene graph.
 * @ingroup scenegraph
 */
class ORK_API  SceneManager : public Object
{
public:
    /**
     * The visibility of a bounding box in a frustum.
     */
    enum visibility {
        FULLY_VISIBLE, //!< The bounding box is fully visible
        PARTIALLY_VISIBLE, //!< The bounding box is partially visible
        INVISIBLE //!< The bounding box is invisible
    };

    /**
     * An iterator over a map of SceneNode.
     */
    typedef MultiMapIterator<string, ptr<SceneNode> > NodeIterator;

    /**
     * Creates an empty SceneManager.
     */
    SceneManager();

    /**
     * Deletes this SceneManager.
     */
    virtual ~SceneManager();

    /**
     * Returns the root node of the scene graph managed by this manager.
     */
    ptr<SceneNode> getRoot();

    /**
     * Sets the root node of the scene graph managed by this manager.
     */
    void setRoot(ptr<SceneNode> root);

    /**
     * Returns the camera node of the scene graph managed by this manager.
     */
    ptr<SceneNode> getCameraNode();

    /**
     * Sets the camera node of the scene graph managed by this manager. This
     * camera node must have a UniformMatrix4f defining the projection from
     * camera space to screen space.
     *
     * @param node a SceneNode flag that identifies the camera node.
     */
    void setCameraNode(const string &node);

    /**
     * Returns the name of the camera node method to be called to draw the scene.
     */
    string getCameraMethod();

    /**
     * Sets the name of the camera node method to be called to draw the scene.
     *
     * @param method a method name.
     */
    void setCameraMethod(const string &method);

    /**
     * Returns the nodes of the scene graph that have the given flag.
     *
     * @param flag a SceneNode flag.
     */
    NodeIterator getNodes(const string &flag);

    /**
     * Returns the SceneNode currently bound to the given loop variable.
     *
     * @param name a loop variable.
     */
    ptr<SceneNode> getNodeVar(const string &name);

    /**
     * Sets the node currently bound to the given loop variable.
     *
     * @param name a loop variable.
     * @param node the new node bound to this loop variable.
     */
    void setNodeVar(const string &name, ptr<SceneNode> node);

    /**
     * Returns the ResourceManager used to manage the resources of the scene
     * graph.
     */
    ptr<ResourceManager> getResourceManager();

    /**
     * Sets the ResourceManager used to manage the resources of the scene graph.
     *
     * @param resourceManager a resource manager.
     */
    void setResourceManager(ptr<ResourceManager> resourceManager);

    /**
     * Returns the Scheduler used to schedule the Task to draw the scene.
     */
    ptr<Scheduler> getScheduler();

    /**
     * Sets the Scheduler to schedule the Task to draw the scene.
     */
    void setScheduler(ptr<Scheduler> scheduler);

    /**
     * Returns the transformation from camera space to screen space.
     */
    mat4d getCameraToScreen();

    /**
     * Sets the transformation from camera space to screen space.
     */
    void setCameraToScreen(const mat4d &cameraToScreen);

    /**
     * Returns the transformation from world space to screen space.
     */
    mat4d getWorldToScreen();

    /**
     * Returns true if the given point is visible from the camera node.
     *
     * @param worldPoint a point in world space.
     */
    bool isVisible(const vec3d &worldPoint);

    /**
     * Returns the visibility of the given bounding box from the camera node.
     *
     * @param worldBounds a bounding box in world space.
     */
    visibility getVisibility(const box3d &worldBounds);

    /**
     * Returns the visibility of the given bounding box in the given frustum.
     *
     * @param frustumPlanes the frustum plane equations.
     * @param b a bounding box, in the same reference frame as the frustum
     *     planes.
     */
    static visibility getVisibility(const vec4d frustumPlanes[6], const box3d &b);

    /**
     * Returns the frustum plane equations from a projection matrix.
     *
     * @param toScreen a camera to screen projection matrix.
     * @param[out] frustumPlanes the frustum plane equations in camera space.
     */
    static void getFrustumPlanes(const mat4d &toScreen, vec4d frustumPlanes[6]);

    /**
     * Updates all the transformation matrices in the scene graph.
     *
     * @param t the current time in micro-seconds.
     * @param dt the elapsed time in micro-seconds since the last call to #update.
     */
    void update(double t, double dt);

    /**
     * Executes the #getCameraMethod of the #getCameraNode node.
     */
    void draw();

    /**
     * Returns the current frame number. This number is incremented after each
     * call to #draw.
     */
    unsigned int getFrameNumber();

    /**
     * Returns the time of the current frame in micro-seconds. This time is the
     * one passed as argument to the last call to #update.
     */
    double getTime();

    /**
     * Returns the elapsed time of between the two previous frames. This time
     * is the one passed as argument to the last call to #update.
     */
    double getElapsedTime();

	/**
     * Returns the current FrameBuffer.
     */
    static ptr<FrameBuffer> getCurrentFrameBuffer();

	/**
     * Sets the current FrameBuffer. This can then be used in any module to retrieve
     * a target on which the user wants to render to.
     */
    static void setCurrentFrameBuffer(ptr<FrameBuffer> fb);

	/**
     * Returns the current Program.
     */
    static ptr<Program> getCurrentProgram();

	/**
     * Sets the current GLSL Program. This can then be used in any module to retrieve a given
     * Program for further drawings.
     */
    static void setCurrentProgram(ptr<Program> prog);

private:
    /**
     * The current framebuffer.
     */
    static FrameBuffer* CURRENTFB;

    /**
     * The current GLSL program.
     */
    static Program *CURRENTPROG;

    /**
     * The root node of the scene graph managed by this manager.
     */
    ptr<SceneNode> root;

    /**
     * The camera node of the scene graph.
     */
    ptr<SceneNode> camera;

    /**
     * The camera to screen transformation.
     */
    mat4d cameraToScreen;

    /**
     * The world to screen transformation.
     */
    mat4d worldToScreen;

    /**
     * The camera frustum planes in world space.
     */
    vec4d worldFrustumPlanes[6];

    /**
     * The flag that identifies the camera node in the scene graph.
     */
    string cameraNode;

    /**
     * The name of the camera node method to be called to draw the scene.
     */
    string cameraMethod;

    /**
     * The last task or task graph that was used to draw the scene.
     */
    ptr<Task> currentTask;

    /**
     * A multimap that associates to each flag all the nodes having this flag.
     */
    multimap<string, ptr<SceneNode> > nodeMap;

    /**
     * A map that associates to each loop variable its current value.
     */
    map<string, ptr<SceneNode> > nodeVariables;

    /**
     * The ResourceManager that manages the resources of the scene graph.
     */
    ptr<ResourceManager> resourceManager;

    /**
     * The Scheduler used to schedule the Task to draw the scene.
     */
    ptr<Scheduler> scheduler;

    /**
     * The current frame number.
     */
    unsigned int frameNumber;

    /**
     * The value of the t argument of the last call to #update.
     */
    double t;

    /**
     * The value of the dt argument of the last call to #update.
     */
    double dt;

    /**
     * Returns the visibility of a bounding box with respect to a frustum plane.
     *
     * @param clip a frustum plane equation.
     * @param b a bounding box in the same reference frame as the frustum plane.
     */
    static visibility getVisibility(const vec4d &clip, const box3d &b);

    /**
     * Computes the SceneNode#isVisible flag of the given SceneNode and of its
     * child node (and so on recursively).
     *
     * @param n a SceneNode.
     * @param v the visibility of its parent node.
     */
    void computeVisibility(ptr<SceneNode> n, visibility v);

    /**
     * Clears the #nodeMap map.
     */
    void clearNodeMap();

    /**
     * Builds the #nodeMap map for the given scene graph.
     *
     * @param node the root node of a scene graph.
     */
    void buildNodeMap(ptr<SceneNode> node);

    friend class SceneNode;
};

}

#endif
