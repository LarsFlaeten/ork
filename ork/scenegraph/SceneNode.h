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

#ifndef _ORK_SCENE_NODE_H_
#define _ORK_SCENE_NODE_H_

#include <set>
#include <map>
#include <vector>
#include <string>
#include "ork/math/box3.h"
#include "ork/core/Iterator.h"
#include "ork/render/MeshBuffers.h"
#include "ork/render/Module.h"
#include "ork/scenegraph/Method.h"

using namespace std;

namespace ork
{

/**
 * @defgroup scenegraph scenegraph
 * @ingroup ork
 * Provides a "toy" generic scene graph framework. This framework is fully
 * generic and extensible, but this genericity makes it inefficient to
 * be used with large scene graphs containing hundreds of thousands of
 * nodes.
 */

class SceneManager;

/**
 * A scene graph node. A scene graph is a tree of generic scene nodes, where
 * each node can be seen as an object with a state (fields) and a behavior
 * (methods). The state is made of a reference frame (relatively to the parent
 * node), some meshes, modules and uniforms (that can reference textures), and
 * any other user defined values. The behavior is made of methods, completely
 * defined by the user by combining basic tasks (draw a mesh, set a projection
 * matrix, etc) with control structures (sequences, loops, etc).
 *
 * @ingroup scenegraph
 */
class ORK_API SceneNode : public Object
{
public:
    /**
     * An iterator to iterate over a set of flags.
     */
    typedef SetIterator<string> FlagIterator;

    /**
     * An iterator to iterate over a map of Value.
     */
    typedef MapIterator<string, ptr<Value> > ValueIterator;

    /**
     * An iterator to iterate over a map of Module.
     */
    typedef MapIterator<string, ptr<Module> > ModuleIterator;

    /**
     * An iterator to iterate over a map of Mesh.
     */
    typedef MapIterator<string, ptr<MeshBuffers> > MeshIterator;

    /**
     * An iterator to iterate over a map of SceneNode fields.
     */
    typedef MapIterator<string, ptr<Object> > FieldIterator;

    /**
     * An iterator to iterate over a map of SceneNode Method.
     */
    typedef MapIterator<string, ptr<Method> > MethodIterator;

    /**
     * True if this scene node is visible, false otherwise.
     */
    bool isVisible;

    /**
     * Creates an empty SceneNode.
     */
    SceneNode();

    /**
     * Deletes this SceneNode.
     */
    virtual ~SceneNode();

    /**
     * Returns the SceneManager that manages the scene graph to which this node
     * belongs.
     */
    ptr<SceneManager> getOwner();

    /**
     * Returns the transformation from this node to its parent node.
     */
    mat4d getLocalToParent();

    /**
     * Sets the transformation from this node to its parent node.
     *
     * @param t the new localToParent transformation.
     */
    void setLocalToParent(const mat4d &t);

    /**
     * Returns the transformation from this node to the root node.
     */
    mat4d getLocalToWorld();

    /**
     * Returns the transformation from the root node to this node.
     */
    mat4d getWorldToLocal();

    /**
     * Returns the transformation from this node to the camera node.
     */
    mat4d getLocalToCamera();

    /**
     * Returns the tranformation from this node to the screen. This is the
     * transformation from this node to the camera node, followed by the
     * transformation from the camera space to the screen space (defined by the
     * cameraToScreen mat4 uniform of the camera node).
     */
    mat4d getLocalToScreen();

    /**
     * Returns the bounding box of this node in local coordinates.
     */
    box3d getLocalBounds();

    /**
     * Sets the bounding box of this node in local coordinates.
     */
    void setLocalBounds(const box3d &bounds);

    /**
     * Returns the bounding box of this node in world coordinates.
     */
    box3d getWorldBounds();

    /**
     * Returns the origin of the local reference frame in world coordinates.
     */
    vec3d getWorldPos();

    /**
     * Returns the flags of this node.
     */
    FlagIterator getFlags();

    /**
     * Returns true is this node has the given flag.
     *
     * @param flag a flag.
     */
    bool hasFlag(const string &flag);

    /**
     * Adds the given flag to the flags of this node.
     *
     * @param flag the flag to be added to this node.
     */
    void addFlag(const string &flag);

    /**
     * Removes the given flag from the flags of this node.
     *
     * @param flag the flag to be removed from this node.
     */
    void removeFlag(const string &flag);

    /**
     * Returns the values of this node.
     */
    ValueIterator getValues();

    /**
     * Returns the value of this node whose local name is given.
     *
     * @param name the local name of a value.
     */
    ptr<Value> getValue(const string &name);

    /**
     * Adds a value to this node under the given local name.
     *
     * @param value a value.
     */
    void addValue(ptr<Value> value);

    /**
     * Removes the value whose local name is given from this node.
     *
     * @param name the local name of the value.
     */
    void removeValue(const string &name);

    /**
     * Returns the modules of this node.
     */
    ModuleIterator getModules();

    /**
     * Returns the module of this node whose local name is given.
     *
     * @param name the local name of a module.
     */
    ptr<Module> getModule(const string &name);

    /**
     * Adds a module to this node under the given local name.
     *
     * @param name a local name.
     * @param s a Module.
     */
    void addModule(const string &name, ptr<Module> s);

    /**
     * Removes the module whose local name is given from this node.
     *
     * @param name the local name of the module.
     */
    void removeModule(const string &name);

    /**
     * Returns the meshes of this node.
     */
    MeshIterator getMeshes();

    /**
     * Returns the mesh of this node whose local name is given.
     *
     * @param name the local name of a mesh.
     */
    ptr<MeshBuffers> getMesh(const string &name);

    /**
     * Adds a mesh to this node under the given local name.
     *
     * @param name a local name.
     * @param m a MeshBuffers.
     */
    void addMesh(const string &name, ptr<MeshBuffers> m);

    /**
     * Removes the mesh whose local name is given from this node.
     *
     * @param name the local name of the mesh.
     */
    void removeMesh(const string &name);

    /**
     * Returns the fields of this node.
     */
    FieldIterator getFields();

    /**
     * Returns the field of this node whose name is given.
     *
     * @param name the name of a field.
     */
    ptr<Object> getField(const string &name);

    /**
     * Adds a field to this node under the given name.
     *
     * @param name the field name.
     * @param f the field value.
     */
    void addField(const string &name, ptr<Object> f);

    /**
     * Removes the field whose name is given from this node.
     *
     * @param name the name of the field.
     */
    void removeField(const string &name);

    /**
     * Returns the methods of this node.
     */
    MethodIterator getMethods();

    /**
     * Returns the method of this node whose name is given.
     *
     * @param name the name of a method.
     */
    ptr<Method> getMethod(const string &name);

    /**
     * Adds a method to this node under the given name.
     *
     * @param name the method name.
     * @param m the method.
     */
    void addMethod(const string &name, ptr<Method> m);

    /**
     * Removes the method whose name is given from this node.
     *
     * @param name the name of the method.
     */
    void removeMethod(const string &name);

    /**
     * Returns the number of child node of this node.
     */
    unsigned int getChildrenCount();

    /**
     * Returns the child node of this node whose index is given.
     *
     * @param index a child node index between 0 and #getChildrenCount - 1.
     */
    ptr<SceneNode> getChild(unsigned int index);

    /**
     * Adds a child node to this node.
     *
     * @param child a child node.
     */
    void addChild(ptr<SceneNode> child);

    /**
     * Removes a child node from this node.
     *
     * @param index a child node index between 0 and #getChildrenCount - 1.
     */
    void removeChild(unsigned int index);

protected:
    /**
     * Swaps this scene node with the given one.
     */
    void swap(ptr<SceneNode> n);

private:
    /**
     * The SceneManager that manages the scene graph to which this node belongs.
     */
    SceneManager *owner;

    /**
     * The transformation from this node to its parent node.
     */
    mat4d localToParent;

    /**
     * The transformation from this node to the root node.
     */
    mat4d localToWorld;

    /**
     * The transformation from the root node to this node.
     */
    mat4d worldToLocal;

    /**
     * The transformation from this node to the camera node.
     */
    mat4d localToCamera;

    /**
     * The transformation from this node to the screen.
     */
    mat4d localToScreen;

    /**
     * The bounding box of this node in local coordinates.
     */
    box3d localBounds;

    /**
     * The bounding box of this node in world coordinates.
     */
    box3d worldBounds;

    /**
     * The origin of the local reference frame of this node in world coordinates.
     */
    vec3d worldPos;

    /**
     * True if the #worldToLocal transform is up to date.
     */
    bool worldToLocalUpToDate;

    /**
     * The flags of this node.
     */
    set<string> flags;

    /**
     * The values of this node.
     */
    map<string, ptr<Value> > values;

    /**
     * The modules of this node.
     */
    map<string, ptr<Module> > modules;

    /**
     * The meshes of this node.
     */
    map<string, ptr<MeshBuffers> > meshes;

    /**
     * The fields of this node.
     */
    map<string, ptr<Object> > fields;

    /**
     * The methods of this node.
     */
    map<string, ptr<Method> > methods;

    /**
     * The child nodes of this node.
     */
    vector< ptr<SceneNode> > children;

    /**
     * Sets the SceneManager that manages the scene graph to which this node
     * belongs.
     *
     * @param owner a SceneManager.
     */
    void setOwner(SceneManager *owner);

    /**
     * Updates the #localToWorld transform. This method also updates #worldBounds
     * and #worldPos.
     *
     * @param parent the parent node of this node.
     */
    void updateLocalToWorld(ptr<SceneNode> parent);

    /**
     * Updates the #localToCamera and the #localToScreen transforms.
     *
     * @param worldToCamera the world to camera transform.
     * @param cameraToScreen the camera to screen transform.
     */
    void updateLocalToCamera(const mat4d &worldToCamera, const mat4d &cameraToScreen);

    friend class SceneManager;
};

}

#endif
