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

#include "ork/scenegraph/SceneNode.h"

#include <algorithm>

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

SceneNode::SceneNode() : Object("SceneNode"), owner(NULL)
{
    localToParent = mat4d::IDENTITY;
    localToWorld = mat4d::IDENTITY;
    worldToLocalUpToDate = false;
    localBounds = box3d(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    localToScreen = mat4d::IDENTITY;
}

SceneNode::~SceneNode()
{
    MethodIterator i = getMethods();
    while (i.hasNext()) {
        i.next()->owner = NULL;
    }
}

ptr<SceneManager> SceneNode::getOwner()
{
    return owner;
}

mat4d SceneNode::getLocalToParent()
{
    return localToParent;
}

void SceneNode::setLocalToParent(const mat4d &t)
{
    localToParent = t;
}

mat4d SceneNode::getLocalToWorld()
{
    return localToWorld;
}

mat4d SceneNode::getWorldToLocal()
{
    if (!worldToLocalUpToDate) {
        worldToLocal = localToWorld.inverse();
        worldToLocalUpToDate = true;
    }
    return worldToLocal;
}

mat4d SceneNode::getLocalToCamera()
{
    return localToCamera;
}

mat4d SceneNode::getLocalToScreen()
{
    return localToScreen;
}

box3d SceneNode::getLocalBounds()
{
    return localBounds;
}

void SceneNode::setLocalBounds(const box3d &bounds)
{
    localBounds = bounds;
}

box3d SceneNode::getWorldBounds()
{
    return worldBounds;
}

vec3d SceneNode::getWorldPos()
{
    return worldPos;
}

SceneNode::FlagIterator SceneNode::getFlags()
{
    return SceneNode::FlagIterator(flags);
}

bool SceneNode::hasFlag(const string &flag)
{
    return flags.find(flag) != flags.end();
}

void SceneNode::addFlag(const string &flag)
{
    flags.insert(flag);
    if (owner != NULL) {
        owner->clearNodeMap();
    }
}

void SceneNode::removeFlag(const string &flag)
{
    flags.erase(flag);
    if (owner != NULL) {
        owner->clearNodeMap();
    }
}

SceneNode::ValueIterator SceneNode::getValues()
{
    return SceneNode::ValueIterator(values);
}

ptr<Value> SceneNode::getValue(const string &name)
{
    map<string, ptr<Value> >::iterator i = values.find(name);
    return i == values.end() ? NULL : (*i).second;
}

void SceneNode::addValue(ptr<Value> value)
{
    values.insert(make_pair(value->getName(), value));
}

void SceneNode::removeValue(const string &name)
{
    values.erase(name);
}

SceneNode::ModuleIterator SceneNode::getModules()
{
    return SceneNode::ModuleIterator(modules);
}

ptr<Module> SceneNode::getModule(const string &name)
{
    map<string, ptr<Module> >::iterator i = modules.find(name);
    return i == modules.end() ? NULL : (*i).second;
}

void SceneNode::addModule(const string &name, ptr<Module> s)
{
    modules[name] = s;
}

void SceneNode::removeModule(const string &name)
{
    modules.erase(name);
}

SceneNode::MeshIterator SceneNode::getMeshes()
{
    return SceneNode::MeshIterator(meshes);
}

ptr<MeshBuffers> SceneNode::getMesh(const string &name)
{
    map<string, ptr<MeshBuffers> >::iterator i = meshes.find(name);
    return i == meshes.end() ? NULL : (*i).second;
}

void SceneNode::addMesh(const string &name, ptr<MeshBuffers> m)
{
    meshes[name] = m;
    localBounds = localBounds.enlarge(m->bounds.cast<double>());
}

void SceneNode::removeMesh(const string &name)
{
    meshes.erase(name);
}

SceneNode::FieldIterator SceneNode::getFields()
{
    return SceneNode::FieldIterator(fields);
}

ptr<Object> SceneNode::getField(const string &name)
{
    map<string, ptr<Object> >::iterator i = fields.find(name);
    return i == fields.end() ? NULL : (*i).second;
}

void SceneNode::addField(const string &name, ptr<Object> f)
{
    removeField(name);
    fields[name] = f;
}

void SceneNode::removeField(const string &name)
{
    map<string, ptr<Object> >::iterator i = fields.find(name);
    if (i != fields.end()) {
        fields.erase(i);
    }
}

SceneNode::MethodIterator SceneNode::getMethods()
{
    return SceneNode::MethodIterator(methods);
}

ptr<Method> SceneNode::getMethod(const string &name)
{
    map<string, ptr<Method> >::iterator i = methods.find(name);
    return i == methods.end() ? NULL : (*i).second;
}

void SceneNode::addMethod(const string &name, ptr<Method> m)
{
    removeMethod(name);
    methods[name] = m;
    m->owner = this;
}

void SceneNode::removeMethod(const string &name)
{
    map<string, ptr<Method> >::iterator i = methods.find(name);
    if (i != methods.end()) {
        methods.erase(i);
        (*i).second->owner = NULL;
    }
}

unsigned int SceneNode::getChildrenCount()
{
    return (unsigned int) children.size();
}

ptr<SceneNode> SceneNode::getChild(unsigned int index)
{
    return children[index];
}

void SceneNode::addChild(ptr<SceneNode> child)
{
    if (child->owner == NULL) {
        children.push_back(child);
        child->setOwner(owner);
        if (owner != NULL) {
            owner->clearNodeMap();
        }
    }
}

void SceneNode::removeChild(unsigned int index)
{
    children.erase(children.begin() + index);
}

void SceneNode::swap(ptr<SceneNode> n)
{
    std::swap(localToParent, n->localToParent);
    std::swap(flags, n->flags);
    std::swap(values, n->values);
    std::swap(modules, n->modules);
    std::swap(meshes, n->meshes);
    std::swap(methods, n->methods);
    std::swap(children, n->children);
    map<string, ptr<Method> >::iterator i;
    i = methods.begin();
    while (i != methods.end()) {
        i->second->owner = this;
    }
    i = n->methods.begin();
    while (i != n->methods.end()) {
        i->second->owner = n.get();
    }
    if (owner != NULL) {
        owner->clearNodeMap();
    }
    setOwner(owner);
    n->setOwner(NULL);
}

void SceneNode::setOwner(SceneManager *owner)
{
    this->owner = owner;
    vector< ptr<SceneNode> >::iterator end = children.end();
    vector< ptr<SceneNode> >::iterator i = children.begin();
    while (i != end) {
        (*i)->setOwner(owner);
        ++i;
    }
}

void SceneNode::updateLocalToWorld(ptr<SceneNode> parent)
{
    if (parent != NULL) {
        localToWorld = parent->localToWorld * localToParent;
    }

    vector< ptr<SceneNode> >::iterator end = children.end();
    vector< ptr<SceneNode> >::iterator i = children.begin();
    while (i != end) {
        (*i)->updateLocalToWorld(this);
        ++i;
    }
    mat4d localToWorld0 = localToWorld;

    worldBounds = localToWorld0 * localBounds;
    worldPos = localToWorld0 * vec3d::ZERO;
    i = children.begin();
    while (i != end) {
        worldBounds = worldBounds.enlarge((*i)->worldBounds);
        ++i;
    }
    worldToLocalUpToDate = false;
}

void SceneNode::updateLocalToCamera(const mat4d &worldToCamera, const mat4d &cameraToScreen)
{
    localToCamera = worldToCamera * localToWorld;
    localToScreen = cameraToScreen * localToCamera;

    vector< ptr<SceneNode> >::iterator end = children.end();
    vector< ptr<SceneNode> >::iterator i = children.begin();
    while (i != end) {
        (*i)->updateLocalToCamera(worldToCamera, cameraToScreen);
        ++i;
    }
}

/// @cond RESOURCES

bool isIntegerTexture(ptr<Texture> t)
{
    switch (t->getInternalFormat()) {
    case R8:
    case R8_SNORM:
    case R16:
    case R16_SNORM:
    case RG8:
    case RG8_SNORM:
    case RG16:
    case RG16_SNORM:
    case R3_G3_B2:
    case RGB4:
    case RGB5:
    case RGB8:
    case RGB8_SNORM:
    case RGB10:
    case RGB12:
    case RGB16:
    case RGB16_SNORM:
    case RGBA2:
    case RGBA4:
    case RGB5_A1:
    case RGBA8:
    case RGBA8_SNORM:
    case RGB10_A2:
    case RGB10_A2UI:
    case RGBA12:
    case RGBA16:
    case RGBA16_SNORM:
    case SRGB8:
    case SRGB8_ALPHA8:
    case R16F:
    case RG16F:
    case RGB16F:
    case RGBA16F:
    case R32F:
    case RG32F:
    case RGBA32F:
    case R11F_G11F_B10F:
    case RGB9_E5:
        return false;
    case R8I:
    case R16I:
    case R32I:
    case RG8I:
    case RG16I:
    case RG32I:
    case RGB8I:
    case RGB16I:
    case RGB32I:
    case RGBA8I:
    case RGBA16I:
    case RGBA32I:
        return true;
    case R8UI:
    case R16UI:
    case R32UI:
    case RG8UI:
    case RG16UI:
    case RG32UI:
    case RGB8UI:
    case RGB16UI:
    case RGB32UI:
    case RGBA8UI:
    case RGBA16UI:
    case RGBA32UI:
    case COMPRESSED_RED:
    case COMPRESSED_RG:
    case COMPRESSED_RGB:
    case COMPRESSED_RGBA:
    case COMPRESSED_SRGB:
    case COMPRESSED_RED_RGTC1:
    case COMPRESSED_SIGNED_RED_RGTC1:
    case COMPRESSED_RG_RGTC2:
    case COMPRESSED_SIGNED_RG_RGTC2:
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return false;
    default:
        assert(false);
        return false;
    }
}

bool isUnsignedIntegerTexture(ptr<Texture> t)
{
    switch (t->getInternalFormat()) {
    case R8:
    case R8_SNORM:
    case R16:
    case R16_SNORM:
    case RG8:
    case RG8_SNORM:
    case RG16:
    case RG16_SNORM:
    case R3_G3_B2:
    case RGB4:
    case RGB5:
    case RGB8:
    case RGB8_SNORM:
    case RGB10:
    case RGB12:
    case RGB16:
    case RGB16_SNORM:
    case RGBA2:
    case RGBA4:
    case RGB5_A1:
    case RGBA8:
    case RGBA8_SNORM:
    case RGB10_A2:
    case RGB10_A2UI:
    case RGBA12:
    case RGBA16:
    case RGBA16_SNORM:
    case SRGB8:
    case SRGB8_ALPHA8:
    case R16F:
    case RG16F:
    case RGB16F:
    case RGBA16F:
    case R32F:
    case RG32F:
    case RGBA32F:
    case R11F_G11F_B10F:
    case RGB9_E5:
    case R8I:
    case R16I:
    case R32I:
    case RG8I:
    case RG16I:
    case RG32I:
    case RGB8I:
    case RGB16I:
    case RGB32I:
    case RGBA8I:
    case RGBA16I:
    case RGBA32I:
        return false;
    case R8UI:
    case R16UI:
    case R32UI:
    case RG8UI:
    case RG16UI:
    case RG32UI:
    case RGB8UI:
    case RGB16UI:
    case RGB32UI:
    case RGBA8UI:
    case RGBA16UI:
    case RGBA32UI:
        return true;
    case COMPRESSED_RED:
    case COMPRESSED_RG:
    case COMPRESSED_RGB:
    case COMPRESSED_RGBA:
    case COMPRESSED_SRGB:
    case COMPRESSED_RED_RGTC1:
    case COMPRESSED_SIGNED_RED_RGTC1:
    case COMPRESSED_RG_RGTC2:
    case COMPRESSED_SIGNED_RG_RGTC2:
    case COMPRESSED_RGBA_BPTC_UNORM_ARB:
    case COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB:
    case COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB:
    case COMPRESSED_RGB_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case COMPRESSED_RGBA_S3TC_DXT5_EXT:
        return false;
    default:
        assert(false);
        return false;
    }
}

class SceneNodeResource : public ResourceTemplate<50, SceneNode>
{
public:
    SceneNodeResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<50, SceneNode>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,flags,value,");

        const char* flags = e->Attribute("flags");
        if (flags != NULL) {
            string f = string(flags) + ',';
            string::size_type start = 0;
            string::size_type index;
            while ((index = f.find(',', start)) != string::npos) {
                addFlag(f.substr(start, index - start));
                start = index + 1;
            }
        }
        if (e->Attribute("name") != NULL) {
            addFlag(string(e->Attribute("name")));
        }

        mat4d ltop = mat4d::IDENTITY;

        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f == NULL) {
                n = n->NextSibling();
                continue;
            }
            if (strcmp(f->Value(), "translate") == 0) {
                float x, y, z;
                checkParameters(desc, f, "x,y,z,");
                getFloatParameter(desc, f, "x", &x);
                getFloatParameter(desc, f, "y", &y);
                getFloatParameter(desc, f, "z", &z);
                ltop = ltop * mat4d::translate(vec3d(x, y, z));

            } else if (strcmp(f->Value(), "rotatex") == 0) {
                float a;
                checkParameters(desc, f, "angle,");
                getFloatParameter(desc, f, "angle", &a);
                ltop = ltop * mat4d::rotatex(a);

            } else if (strcmp(f->Value(), "rotatey") == 0) {
                float a;
                checkParameters(desc, f, "angle,");
                getFloatParameter(desc, f, "angle", &a);
                ltop = ltop * mat4d::rotatey(a);

            } else if (strcmp(f->Value(), "rotatez") == 0) {
                float a;
                checkParameters(desc, f, "angle,");
                getFloatParameter(desc, f, "angle", &a);
                ltop = ltop * mat4d::rotatez(a);

            } else if (strcmp(f->Value(), "bounds") == 0) {
                int n = 0;
                float xmin, xmax, ymin, ymax, zmin, zmax;
                checkParameters(desc, f, "xmin,xmax,ymin,ymax,zmin,zmax,");
                n += getFloatParameter(desc, f, "xmin", &xmin);
                n += getFloatParameter(desc, f, "xmax", &xmax);
                n += getFloatParameter(desc, f, "ymin", &ymin);
                n += getFloatParameter(desc, f, "ymax", &ymax);
                n += getFloatParameter(desc, f, "zmin", &zmin);
                n += getFloatParameter(desc, f, "zmax", &zmax);
                if (n != 6) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Invalid bounds");
                    }
                    throw exception();
                }
                setLocalBounds(box3d(xmin, xmax, ymin, ymax, zmin, zmax));

            } else if (strncmp(f->Value(), "uniform", 7) == 0) {
                checkParameters(desc, f, "name,id,x,y,z,w,sampler,texture,type,");
                string type = f->Attribute("type") == NULL ? "FLOAT" : f->Attribute("type");
                int t;
                if (strcmp(type.c_str(), "BOOL") == 0) {
                    t = 0;
                } else if (strcmp(type.c_str(), "INT") == 0) {
                    t = 1;
                } else if (strcmp(type.c_str(), "UINT") == 0) {
                    t = 2;
                } else if (strcmp(type.c_str(), "FLOAT") == 0) {
                    t = 3;
                } else {
                    t = 4;
                }
                float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
                ptr<Texture> texture = NULL;
                string id = getParameter(desc, f, "id");
                int paramCount = 0;
                if (f->Attribute("x") != NULL) {
                    getFloatParameter(desc, f, "x", &x);
                    paramCount++;
                    if (f->Attribute("y") != NULL) {
                        getFloatParameter(desc, f, "y", &y);
                        paramCount++;
                        if (f->Attribute("z") != NULL) {
                            getFloatParameter(desc, f, "z", &z);
                            paramCount++;
                            if (f->Attribute("w") != NULL) {
                                getFloatParameter(desc, f, "w", &w);
                                paramCount++;
                            }
                        }
                    }
                    switch(t) {
                        case 0:
                            switch(paramCount) {
                                case 1:
                                    addValue(new Value1b(id, bool(x)));
                                    break;
                                case 2:
                                    addValue(new Value2b(id, vec2<bool>(x, y)));
                                    break;
                                case 3:
                                    addValue(new Value3b(id, vec3<bool>(x, y, z)));
                                    break;
                                case 4:
                                    addValue(new Value4b(id, vec4<bool>(x, y, z, w)));
                                    break;
                            }
                            break;

                        case 1:
                            switch(paramCount) {
                                case 1:
                                    addValue(new Value1i(id, GLint(x)));
                                    break;
                                case 2:
                                    addValue(new Value2i(id, vec2<GLint>(GLint(x), GLint(y))));
                                    break;
                                case 3:
                                    addValue(new Value3i(id, vec3<GLint>(GLint(x), GLint(y), GLint(z))));
                                    break;
                                case 4:
                                    addValue(new Value4i(id, vec4<GLint>(GLint(x), GLint(y), GLint(z), GLint(w))));
                                    break;
                            }
                            break;
                        case 2:
                            switch(paramCount) {
                                case 1:
                                    addValue(new Value1ui(id, GLuint(x)));
                                    break;
                                case 2:
                                    addValue(new Value2ui(id, vec2<GLuint>(GLuint(x), GLuint(y))));
                                    break;
                                case 3:
                                    addValue(new Value3ui(id, vec3<GLuint>(GLuint(x), GLuint(y), GLuint(z))));
                                    break;
                                case 4:
                                    addValue(new Value4ui(id, vec4<GLuint>(GLuint(x), GLuint(y), GLuint(z), GLuint(w))));
                                    break;
                            }
                            break;
                        case 3:
                            switch(paramCount) {
                                case 1:
                                    addValue(new Value1f(id, GLfloat(x)));
                                    break;
                                case 2:
                                    addValue(new Value2f(id, vec2<GLfloat>(x, y)));
                                    break;
                                case 3:
                                    addValue(new Value3f(id, vec3<GLfloat>(x, y, z)));
                                    break;
                                case 4:
                                    addValue(new Value4f(id, vec4<GLfloat>(x, y, z, w)));
                                    break;
                            }
                            break;
                        case 4:
                            switch(paramCount) {
                                case 1:
                                    addValue(new Value1d(id, GLdouble(x)));
                                    break;
                                case 2:
                                    addValue(new Value2d(id, vec2<GLdouble>(x, y)));
                                    break;
                                case 3:
                                    addValue(new Value3d(id, vec3<GLdouble>(x, y, z)));
                                    break;
                                case 4:
                                    addValue(new Value4d(id, vec4<GLdouble>(x, y, z, w)));
                                    break;
                            }
                            break;
                    }
                } else {
                    if (f->Attribute("texture") != NULL) {
                        texture = manager->loadResource(getParameter(desc, f, "texture")).cast<Texture>();
                        UniformType type = SAMPLER_1D;
                        if (texture.cast<Texture1D>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_1D;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_1D;
                            } else {
                                type = SAMPLER_1D;
                            }
                        } else if (texture.cast<Texture1DArray>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_1D_ARRAY;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_1D_ARRAY;
                            } else {
                                type = SAMPLER_1D_ARRAY;
                            }
                        } else if (texture.cast<Texture2D>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_2D;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_2D;
                            } else {
                                type = SAMPLER_2D;
                            }
                        } else if (texture.cast<Texture2DArray>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_2D_ARRAY;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_2D_ARRAY;
                            } else {
                                type = SAMPLER_2D_ARRAY;
                            }
                        } else if (texture.cast<Texture2DMultisample>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_2D_MULTISAMPLE;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
                            } else {
                                type = SAMPLER_2D_MULTISAMPLE;
                            }
                        } else if (texture.cast<Texture2DMultisampleArray>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
                            } else {
                                type = SAMPLER_2D_MULTISAMPLE_ARRAY;
                            }
                        } else if (texture.cast<Texture3D>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_3D;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_3D;
                            } else {
                                type = SAMPLER_3D;
                            }
                        } else if (texture.cast<TextureBuffer>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_BUFFER;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_BUFFER;
                            } else {
                                type = SAMPLER_BUFFER;
                            }
                        } else if (texture.cast<TextureCube>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_CUBE;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_CUBE;
                            } else {
                                type = SAMPLER_CUBE;
                            }
                        } else if (texture.cast<TextureCubeArray>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_CUBE_MAP_ARRAY;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY;
                            } else {
                                type = SAMPLER_CUBE_MAP_ARRAY;
                            }
                        } else if (texture.cast<TextureRectangle>() != NULL) {
                            if (isIntegerTexture(texture)) {
                                type = INT_SAMPLER_2D_RECT;
                            } else if (isUnsignedIntegerTexture(texture)) {
                                type = UNSIGNED_INT_SAMPLER_2D_RECT;
                            } else {
                                type = SAMPLER_2D_RECT;
                            }
                        } else {
                            assert(false);
                        }
                        addValue(new ValueSampler(type, id, texture));
                    } else {
                        printf("%s\n", f->Value());
                        assert(false);
                    }
                }
            } else if (strcmp(f->Value(), "module") == 0) {
                string id = getParameter(desc, f, "id");
                string value = getParameter(desc, f, "value");
                ptr<Module> module = manager->loadResource(string(value)).cast<Module>();
                addModule(id, module);

            } else if (strcmp(f->Value(), "mesh") == 0) {
                checkParameters(desc, f, "id,value,");
                string id = getParameter(desc, f, "id");
                string value = getParameter(desc, f, "value");
                ptr<MeshBuffers> mesh = manager->loadResource(string(value)).cast<MeshBuffers>();
                addMesh(id, mesh);

            } else if (strcmp(f->Value(), "field") == 0) {
                checkParameters(desc, f, "id,value,");
                string id = getParameter(desc, f, "id");
                string value = getParameter(desc, f, "value");
                ptr<Object> field = manager->loadResource(string(value));
                addField(string(id), field);
            } else if (strcmp(f->Value(), "method") == 0) {
                checkParameters(desc, f, "id,value,enabled,");
                string id = getParameter(desc, f, "id");
                string value = getParameter(desc, f, "value");
                ptr<TaskFactory> meth = manager->loadResource(string(value)).cast<TaskFactory>();
                ptr<Method> method = new Method(meth);
                const char *enable = f->Attribute("enabled");
                if (enable != NULL && strcmp(enable, "false") == 0) {
                    method->setIsEnabled(false);
                }
                addMethod(string(id), method);

            } else if (strcmp(f->Value(), "node") == 0) {
                ptr<SceneNode> child = NULL;
                const char *value = f->Attribute("value");
                if (value != NULL) {
                    child = manager->loadResource(string(value)).cast<SceneNode>();
                } else {
                    child = new SceneNodeResource(manager, "", desc, f);
                }
                addChild(child);

            } else {
                string id = getParameter(desc, f, "id");
                ptr<Object> field = ResourceFactory::getInstance()->create(manager, f->Value(), desc, f);//manager->loadResource(string(value));
                if (field != NULL) {
                    addField(string(id), field);
                }  else {
                    if (Logger::WARNING_LOGGER != NULL) {
                        log(Logger::WARNING_LOGGER, desc, f, "Unknown scene node element '" + f->ValueStr() + "'");
                    }
                }
            }
            n = n->NextSibling();
        }

        setLocalToParent(ltop);
    }
};

extern const char node[] = "node";

static ResourceFactory::Type<node, SceneNodeResource> SceneNodeType;

/// @endcond

}
