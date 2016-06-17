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

#include "ork/scenegraph/SceneManager.h"

#include "ork/render/FrameBuffer.h"

namespace ork
{

FrameBuffer* SceneManager::CURRENTFB = NULL;
Program* SceneManager::CURRENTPROG = NULL;

ptr<FrameBuffer> SceneManager::getCurrentFrameBuffer()
{
    if (CURRENTFB == NULL) {
        CURRENTFB = FrameBuffer::getDefault().get();
    }
    return CURRENTFB;
}

ptr<Program> SceneManager::getCurrentProgram()
{
    return CURRENTPROG;
}

void SceneManager::setCurrentFrameBuffer(ptr<FrameBuffer> fb)
{
    CURRENTFB = fb.get();
}

void SceneManager::setCurrentProgram(ptr<Program> prog)
{
    CURRENTPROG = prog.get();
}

SceneManager::SceneManager()
  : Object("SceneManager"),
    worldToScreen(mat4d::ZERO), // should call update before using
    frameNumber(0)
{

}

SceneManager::~SceneManager()
{
    if (root != NULL) {
        root->setOwner(NULL);
    }
    resourceManager->close();
}

ptr<SceneNode> SceneManager::getRoot()
{
    return root;
}

void SceneManager::setRoot(ptr<SceneNode> root)
{
    if (this->root != NULL) {
        this->root->setOwner(NULL);
    }
    this->root = root;
    this->root->setOwner(this);
    this->camera = NULL;
}

ptr<SceneNode> SceneManager::getCameraNode()
{
    if (camera == NULL || camera->owner != this) {
        camera = NULL;
        SceneManager::NodeIterator i = getNodes(cameraNode);
        if (i.hasNext()) {
            camera = i.next();
        }
    }
    return camera;
}

void SceneManager::setCameraNode(const string &node)
{
    camera = NULL;
    cameraNode = node;
    SceneManager::NodeIterator i = getNodes(node);
    if (i.hasNext()) {
        camera = i.next();
    }
}

string SceneManager::getCameraMethod()
{
    return cameraMethod;
}

void SceneManager::setCameraMethod(const string &method)
{
    cameraMethod = method;
}

SceneManager::NodeIterator SceneManager::getNodes(const string &flag)
{
    if (nodeMap.size() == 0 && root != NULL) {
        buildNodeMap(root);
    }
    return SceneManager::NodeIterator(flag, nodeMap);
}

ptr<SceneNode> SceneManager::getNodeVar(const string &name)
{
    map<string, ptr<SceneNode> >::iterator i = nodeVariables.find(name);
    if (i != nodeVariables.end()) {
        return i->second;
    }
    return NULL;
}

void SceneManager::setNodeVar(const string &name, ptr<SceneNode> node)
{
    nodeVariables[name] = node;
}

ptr<ResourceManager> SceneManager::getResourceManager()
{
    return resourceManager;
}

void SceneManager::setResourceManager(ptr<ResourceManager> resourceManager)
{
    this->resourceManager = resourceManager;
}

ptr<Scheduler> SceneManager::getScheduler()
{
    return scheduler;
}

void SceneManager::setScheduler(ptr<Scheduler> scheduler)
{
    this->scheduler = scheduler;
}

mat4d SceneManager::getCameraToScreen()
{
    return cameraToScreen;
}

void SceneManager::setCameraToScreen(const mat4d &cameraToScreen)
{
    this->cameraToScreen = cameraToScreen;
}

mat4d SceneManager::getWorldToScreen()
{
    return worldToScreen;
}

bool SceneManager::isVisible(const vec3d &worldPoint)
{
    for (int i = 0; i < 5; ++i) {
        if (worldFrustumPlanes[i].dotproduct(worldPoint) <= 0) {
            return false;
        }
    }
    return true;
}

SceneManager::visibility SceneManager::getVisibility(const box3d &worldBounds)
{
    return getVisibility(worldFrustumPlanes, worldBounds);
}

SceneManager::visibility SceneManager::getVisibility(const vec4d frustumPlanes[6], const box3d &b)
{
    visibility v0 = getVisibility(frustumPlanes[0], b);
    if (v0 == INVISIBLE) {
        return INVISIBLE;
    }
    visibility v1 = getVisibility(frustumPlanes[1], b);
    if (v1 == INVISIBLE) {
        return INVISIBLE;
    }
    visibility v2 = getVisibility(frustumPlanes[2], b);
    if (v2 == INVISIBLE) {
        return INVISIBLE;
    }
    visibility v3 = getVisibility(frustumPlanes[3], b);
    if (v3 == INVISIBLE) {
        return INVISIBLE;
    }
    visibility v4 = getVisibility(frustumPlanes[4], b);
    if (v4 == INVISIBLE) {
        return INVISIBLE;
    }
    if (v0 == FULLY_VISIBLE && v1 == FULLY_VISIBLE &&
        v2 == FULLY_VISIBLE && v3 == FULLY_VISIBLE &&
        v4 == FULLY_VISIBLE)
    {
        return FULLY_VISIBLE;
    }
    return PARTIALLY_VISIBLE;
}

void SceneManager::getFrustumPlanes(const mat4d &toScreen, vec4d frustumPlanes[6])
{
    const double *m = toScreen.coefficients();
    // Extract the LEFT plane
    frustumPlanes[0].x = m[12] + m[0];
    frustumPlanes[0].y = m[13] + m[1];
    frustumPlanes[0].z = m[14] + m[2];
    frustumPlanes[0].w = m[15] + m[3];
    // Extract the RIGHT plane
    frustumPlanes[1].x = m[12] - m[0];
    frustumPlanes[1].y = m[13] - m[1];
    frustumPlanes[1].z = m[14] - m[2];
    frustumPlanes[1].w = m[15] - m[3];
    // Extract the BOTTOM plane
    frustumPlanes[2].x = m[12] + m[4];
    frustumPlanes[2].y = m[13] + m[5];
    frustumPlanes[2].z = m[14] + m[6];
    frustumPlanes[2].w = m[15] + m[7];
    // Extract the TOP plane
    frustumPlanes[3].x = m[12] - m[4];
    frustumPlanes[3].y = m[13] - m[5];
    frustumPlanes[3].z = m[14] - m[6];
    frustumPlanes[3].w = m[15] - m[7];
    // Extract the NEAR plane
    frustumPlanes[4].x = m[12] + m[8];
    frustumPlanes[4].y = m[13] + m[9];
    frustumPlanes[4].z = m[14] + m[10];
    frustumPlanes[4].w = m[15] + m[11];
    // Extract the FAR plane
    frustumPlanes[5].x = m[12] - m[8];
    frustumPlanes[5].y = m[13] - m[9];
    frustumPlanes[5].z = m[14] - m[10];
    frustumPlanes[5].w = m[15] - m[11];
}

void SceneManager::update(double t, double dt)
{
    this->t = t;
    this->dt = dt;

    if (root != NULL) {
        root->updateLocalToWorld(NULL);
        mat4d cameraToScreen = getCameraToScreen();
        worldToScreen = cameraToScreen * getCameraNode()->getWorldToLocal();
        root->updateLocalToCamera(getCameraNode()->getWorldToLocal(), cameraToScreen);
        getFrustumPlanes(worldToScreen, worldFrustumPlanes);
        computeVisibility(root, PARTIALLY_VISIBLE);
    }
}

void SceneManager::draw()
{
    if (camera != NULL) {
        ptr<Method> m = camera->getMethod(cameraMethod);
        if (m != NULL) {
            ptr<Task> newTask = NULL;
            try {
                newTask = m->getTask();
            } catch (...) {
            }
            if (newTask != NULL) {
                scheduler->run(newTask);
                currentTask = newTask;
            } else if (currentTask != NULL) {
                scheduler->run(currentTask);
            }
        }
    }
    ++frameNumber;
}

unsigned int SceneManager::getFrameNumber()
{
    return frameNumber;
}

double SceneManager::getTime()
{
    return t;
}

double SceneManager::getElapsedTime()
{
    return dt;
}

SceneManager::visibility SceneManager::getVisibility(const vec4d &clip, const box3d &b)
{
    double x0 = b.xmin * clip.x;
    double x1 = b.xmax * clip.x;
    double y0 = b.ymin * clip.y;
    double y1 = b.ymax * clip.y;
    double z0 = b.zmin * clip.z + clip.w;
    double z1 = b.zmax * clip.z + clip.w;
    double p1 = x0 + y0 + z0;
    double p2 = x1 + y0 + z0;
    double p3 = x1 + y1 + z0;
    double p4 = x0 + y1 + z0;
    double p5 = x0 + y0 + z1;
    double p6 = x1 + y0 + z1;
    double p7 = x1 + y1 + z1;
    double p8 = x0 + y1 + z1;
    if (p1 <= 0 && p2 <= 0 && p3 <= 0 && p4 <= 0 && p5 <= 0 && p6 <= 0 && p7 <= 0 && p8 <= 0) {
        return INVISIBLE;
    }
    if (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0 && p5 > 0 && p6 > 0 && p7 > 0 && p8 > 0) {
        return FULLY_VISIBLE;
    }
    return PARTIALLY_VISIBLE;
}

void SceneManager::computeVisibility(ptr<SceneNode> n, visibility v)
{
    if (v == PARTIALLY_VISIBLE) {
        v = getVisibility(n->getWorldBounds());
    }
    n->isVisible = v != INVISIBLE;

    for (unsigned int i = 0; i < n->getChildrenCount(); ++i) {
        computeVisibility(n->getChild(i), v);
    }
}

void SceneManager::clearNodeMap()
{
    nodeMap.clear();
}

void SceneManager::buildNodeMap(ptr<SceneNode> node)
{
    SceneNode::FlagIterator i = node->getFlags();
    while (i.hasNext()) {
        nodeMap.insert(make_pair(i.next(), node));
    }
    unsigned int n = node->getChildrenCount();
    for (unsigned int i = 0; i < n; ++i) {
        buildNodeMap(node->getChild(i));
    }
}

}
