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

#include "ork/scenegraph/SetTargetTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

namespace ork
{

BufferId getBufferFromName(const char *v);

static_ptr<FrameBuffer> SetTargetTask::FRAME_BUFFER;

static_ptr<FrameBuffer> SetTargetTask::TARGET_BUFFER;

SetTargetTask::SetTargetTask() : AbstractTask("SetTargetTask")
{
}

SetTargetTask::SetTargetTask(const vector<Target> &targets, bool autoResize) :
    AbstractTask("SetTargetTask")
{
    init(targets, autoResize);
}

void SetTargetTask::init(const vector<Target> &targets, bool autoResize)
{
    this->targets = targets;
    this->autoResize = autoResize;
}

SetTargetTask::~SetTargetTask()
{
}

ptr<Task> SetTargetTask::getTask(ptr<Object> context)
{
    vector< ptr<Texture> > textures;
    ptr<SceneNode> n = context.cast<Method>()->getOwner();
    try {
        for (unsigned int i = 0; i < targets.size(); ++i) {
            Target *target = &(targets[i]);
            string name = target->texture.name;
            ptr<SceneNode> owner = target->texture.getTarget(n);
            if (owner != NULL) {
//                ptr<Uniform> u = NULL;
                string::size_type index = name.find(':');
                ptr<Texture> t = NULL;
                if (index == string::npos) {
                    t = owner->getValue(name).cast<ValueSampler>()->get();
                } else {
                    ptr<Module> module = owner->getModule(name.substr(0, index));
                    set<Program *> progs = module->getUsers();
                    t = (*progs.begin())->getUniformSampler(name.substr(index + 1))->get();
                }
                textures.push_back(t);
            } else {
                textures.push_back(n->getOwner()->getResourceManager()->loadResource(name).cast<Texture>());
            }
            if (textures[i] == NULL) {
                throw exception();
            }
            if (autoResize) {
                vec4<GLint> viewport = FrameBuffer::getDefault()->getViewport();
                ptr<Texture2D> t = textures[i].cast<Texture2D>();
                assert(t != NULL);
                if (t->getWidth() != viewport.z || t->getHeight() != viewport.w) {
                    t->setImage(viewport.z, viewport.w, t->getFormat(), FLOAT, CPUBuffer(NULL));
                }
            }
        }
    } catch (...) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("SCENEGRAPH", "SetTarget: cannot find attachment textures");
        }
        throw exception();
    }

    return new Impl(this, textures);
}

ptr<FrameBuffer> SetTargetTask::getOffscreenBuffer()
{
    if (FRAME_BUFFER == NULL) {
        FRAME_BUFFER = new FrameBuffer();
        ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
        SceneManager::setCurrentFrameBuffer(FRAME_BUFFER);
        FRAME_BUFFER->setReadBuffer(BufferId(0));
        FRAME_BUFFER->setDrawBuffer(BufferId(0));
        SceneManager::setCurrentFrameBuffer(fb);
    }
    return FRAME_BUFFER;
}

ptr<FrameBuffer> SetTargetTask::getTargetBuffer()
{
    if (TARGET_BUFFER == NULL) {
        ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
        TARGET_BUFFER = new FrameBuffer();
        SceneManager::setCurrentFrameBuffer(TARGET_BUFFER);
        TARGET_BUFFER->setReadBuffer(BufferId(0));
        TARGET_BUFFER->setDrawBuffer(BufferId(0));
        SceneManager::setCurrentFrameBuffer(fb);
    }
    return TARGET_BUFFER;
}

void SetTargetTask::swap(ptr<SetTargetTask> t)
{
    std::swap(targets, t->targets);
}

SetTargetTask::Impl::Impl(ptr<SetTargetTask> source, vector< ptr<Texture> > textures) :
    Task("SetTarget", true, 0), source(source), textures(textures)
{
}

SetTargetTask::Impl::~Impl()
{
}

bool SetTargetTask::Impl::run()
{
    if (Logger::DEBUG_LOGGER != NULL) {
        ostringstream os;
        os << "SetTarget";
        for (unsigned int i = 0; i < textures.size(); ++i) {
            BufferId b = source->targets[i].buffer;
            switch (b) {
                case COLOR0:
                    os << " COLOR0";
                    break;
                case COLOR1:
                    os << " COLOR1";
                    break;
                case COLOR2:
                    os << " COLOR2";
                    break;
                case COLOR3:
                    os << " COLOR3";
                    break;
                case STENCIL:
                    os << " STENCIL";
                    break;
                case DEPTH:
                    os << " DEPTH";
                    break;
            }
            Resource* r = dynamic_cast<Resource*>(&(*(textures[i])));
            if (r != NULL) {
                os << " '" << r->getName() << "'";
            }
        }
        if (textures.size() == 0) {
            os << " default framebuffer";
        }
        Logger::DEBUG_LOGGER->log("SCENEGRAPH", os.str());
    }

    ptr<FrameBuffer> fb = getTargetBuffer();
    if (textures.size() == 0) {
        BufferId bufs[6] = {
            COLOR0,
            COLOR1,
            COLOR2,
            COLOR3,
            STENCIL,
            DEPTH
        };
        for (int i = 0; i < 6; ++i) {
            ptr<Texture> t = fb->getTextureBuffer(bufs[i]);
            if (t != NULL) {
                t->generateMipMap();
            }
        }
        SceneManager::setCurrentFrameBuffer(FrameBuffer::getDefault());
        return true;
    }
    int w = 0, h = 0;

    for (unsigned int i = 0; i < textures.size(); ++i) {
        Target *target = &(source->targets[i]);
        ptr<Texture> texture = textures[i];
        if (texture.cast<Texture2D>() != NULL) {
            ptr<Texture2D> t = texture.cast<Texture2D>();
            fb->setTextureBuffer(target->buffer, t, target->level);
            //a.setColorBuffer(target->buffer, t, target->level);
            w = t->getWidth();
            h = t->getHeight();
        } else if (texture.cast<Texture2DArray>() != NULL) {
            ptr<Texture2DArray> t = texture.cast<Texture2DArray>();
            fb->setTextureBuffer(target->buffer, t, target->level, target->layer);
            //a.setColorBuffer(target->buffer, t, target->level, target->layer);
            w = t->getWidth();
            h = t->getHeight();
        } else if (texture.cast<TextureCube>() != NULL) {
            ptr<TextureCube> t = texture.cast<TextureCube>();
            fb->setTextureBuffer(target->buffer, t, target->level, CubeFace(target->layer));
            //a.setColorBuffer(target->buffer, t, target->level, TextureCube::face(target->layer));
            w = t->getWidth();
            h = t->getHeight();
        } else if (texture.cast<Texture3D>() != NULL) {
            ptr<Texture3D> t = texture.cast<Texture3D>();
            fb->setTextureBuffer(target->buffer, t, target->level, target->layer);
            //a.setColorBuffer(target->buffer, t, target->level, target->layer);
            w = t->getWidth();
            h = t->getHeight();
        } else {
            ptr<Texture1D> t = texture.cast<Texture1D>();
            fb->setTextureBuffer(target->buffer, t, target->level);
            //a.setColorBuffer(target->buffer, t, target->level);
            w = t->getWidth();
            h = 1;
        }
    }
    SceneManager::setCurrentFrameBuffer(fb);
    fb->setViewport(vec4<GLint>(0, 0, w, h));
    return true;
}

/// @cond RESOURCES

class SetTargetTaskResource : public ResourceTemplate<40, SetTargetTask>
{
public:
    SetTargetTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, SetTargetTask>(manager, name, desc)
    {
        vector<Target> targets;
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,autoResize,");
        bool autoResize = false;
        if (e->Attribute("autoResize") != NULL) {
            autoResize = strcmp(e->Attribute("autoResize"), "true") == 0;
        }
        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f != NULL) {
                Target t;
                if (strcmp(f->Value(), "buffer") != 0) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Invalid subelement");
                    }
                    throw exception();
                }
                checkParameters(desc, f, "name,texture,level,layer,");
                string name = getParameter(desc, f, "name");
                try {
                    t.buffer = getBufferFromName(name.c_str());
                } catch (...) {
                    if (Logger::ERROR_LOGGER != NULL) {
                        log(Logger::ERROR_LOGGER, desc, f, "Invalid buffer name");
                    }
                    throw exception();
                }
                t.texture = QualifiedName(getParameter(desc, f, "texture"));
                t.level = 0;
                if (f->Attribute("level") != NULL) {
                    getIntParameter(desc, f, "level", &t.level);
                }
                t.layer = 0;
                if (f->Attribute("layer") != NULL) {
                    getIntParameter(desc, f, "layer", &t.layer);
                }
                targets.push_back(t);
            }
            n = n->NextSibling();
        }
        init(targets, autoResize);
    }
};

extern const char setTarget[] = "setTarget";

static ResourceFactory::Type<setTarget, SetTargetTaskResource> SetTargetTaskType;

/// @endcond

}
