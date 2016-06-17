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

#include "ork/scenegraph/SetStateTask.h"

#include "ork/render/FrameBuffer.h"
#include "ork/resource/ResourceTemplate.h"
#include "ork/scenegraph/SceneManager.h"

using namespace std;

namespace ork
{

BufferId getBufferFromName(const char *v) {
    if (strcmp(v, "NONE") == 0) {
        return BufferId(0);
    } else if (strcmp(v, "COLOR0") == 0) {
        return COLOR0;
    } else if (strcmp(v, "COLOR1") == 0) {
        return COLOR1;
    } else if (strcmp(v, "COLOR2") == 0) {
        return COLOR2;
    } else if (strcmp(v, "COLOR3") == 0) {
        return COLOR3;
    } else if (strcmp(v, "COLOR4") == 0) {
        return COLOR4;
    } else if (strcmp(v, "COLOR5") == 0) {
        return COLOR5;
    } else if (strcmp(v, "COLOR6") == 0) {
        return COLOR6;
    } else if (strcmp(v, "COLOR7") == 0) {
        return COLOR7;
    } else if (strcmp(v, "DEPTH") == 0) {
        return DEPTH;
    } else {
        throw exception();
    }
}

SetStateTask::Runnable::~Runnable()
{
}

class SetViewport : public SetStateTask::Runnable
{
public:
    SetViewport(vec4<GLint> vp)
    {
        this-> viewport = vp;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setViewport(viewport);
    }

private:
    vec4<GLint> viewport;
};

class SetDepthRange : public SetStateTask::Runnable
{
public:
    SetDepthRange(GLfloat n, GLfloat f)
    {
        this->near = n;
        this->far = f;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setDepthRange(near, far);
    }

private:
    GLfloat near;

    GLfloat far;
};

class SetClipDistances : public SetStateTask::Runnable
{
public:
    SetClipDistances(int d)
    {
        this->clipDistances = d;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setClipDistances(clipDistances);
    }

private:
    int clipDistances;
};

class SetClearColor : public SetStateTask::Runnable
{
public:
    SetClearColor(vec4<GLfloat> &c)
    {
        this->color = c;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setClearColor(color);
    }

private:
    vec4<GLfloat> color;
};

class SetClearDepth : public SetStateTask::Runnable
{
public:
    SetClearDepth(GLfloat clearDepth)
    {
        depth = clearDepth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setClearDepth(depth);
    }

private:
    GLfloat depth;
};

class SetClearStencil : public SetStateTask::Runnable
{
public:
    SetClearStencil(GLint clearStencil)
    {
        stencil = clearStencil;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setClearStencil(stencil);
    }

private:
    GLint stencil;
};

class SetPointSize : public SetStateTask::Runnable
{
public:
    SetPointSize(GLfloat pointSize)
    {
        size = pointSize;
    }
    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPointSize(size);
    }

private:
    GLfloat size;
};

class SetPointFadeThresholdSize: public SetStateTask::Runnable
{
public:
    SetPointFadeThresholdSize(GLfloat tSize)
    {
        size = tSize;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPointFadeThresholdSize(size);
    }

private:
    GLfloat size;
};

class SetPointLowerLeftOrigin : public SetStateTask::Runnable
{
public:
    SetPointLowerLeftOrigin(bool pointLowerLeftOrigin)
    {
        origin = pointLowerLeftOrigin;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPointLowerLeftOrigin(origin);
    }

private:
    bool origin;
};

class SetLineWidth : public SetStateTask::Runnable
{
public:
    SetLineWidth(GLfloat lineWidth)
    {
        width = lineWidth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setLineWidth(width);
    }

private:
    GLfloat width;
};

class SetLineSmooth : public SetStateTask::Runnable
{
public:
    SetLineSmooth(bool lineSmooth)
    {
        smooth = lineSmooth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setLineSmooth(smooth);
    }

private:
    bool smooth;
};

class SetFrontFaceCW : public SetStateTask::Runnable
{
public:
    SetFrontFaceCW(bool frontFaceCW)
    {
        this->frontFaceCW = frontFaceCW;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setFrontFaceCW(frontFaceCW);
    }

private:
    bool frontFaceCW;
};

class SetPolygonMode : public SetStateTask::Runnable
{
public:
    SetPolygonMode(PolygonMode polygonFront, PolygonMode polygonBack)
    {
        this->polygonFront = polygonFront;
        this->polygonBack = polygonBack;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPolygonMode(polygonFront, polygonBack);
    }

private:
    PolygonMode polygonFront;
    PolygonMode polygonBack;
};

class SetPolygonSmooth: public SetStateTask::Runnable
{
public:
    SetPolygonSmooth(bool polygonSmooth)
    {
        this->polygonSmooth = polygonSmooth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPolygonSmooth(polygonSmooth);
    }

private:
    bool polygonSmooth;
};

class SetPolygonOffset : public SetStateTask::Runnable
{
public:
    SetPolygonOffset(GLfloat factor, GLfloat units)
    {
        this->factor = factor;
        this->units = units;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPolygonOffset(factor, units);
    }

private:
    GLfloat factor;
    GLfloat units;
};

class SetPolygonOffsets : public SetStateTask::Runnable
{
public:
    SetPolygonOffsets(bool pointOffset, bool lineOffset, bool polygonOffset)
    {
        this->pointOffset = pointOffset;
        this->lineOffset = lineOffset;
        this->polygonOffset = polygonOffset;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setPolygonOffset(pointOffset, lineOffset, polygonOffset);
    }

private:
    bool pointOffset;
    bool lineOffset;
    bool polygonOffset;
};

class SetMultisample: public SetStateTask::Runnable
{
public:
    SetMultisample(bool multiSample)
    {
        this->multiSample = multiSample;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setMultisample(multiSample);
    }

private:
    bool multiSample;
};

class SetSampleAlpha: public SetStateTask::Runnable
{
public:
    SetSampleAlpha(bool sampleAlphaToCoverage, bool sampleAlphaToOne)
    {
        this->sampleAlphaToCoverage = sampleAlphaToCoverage;
        this->sampleAlphaToOne = sampleAlphaToOne;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setSampleAlpha(sampleAlphaToCoverage, sampleAlphaToOne);
    }

private:
    bool sampleAlphaToCoverage;
    bool sampleAlphaToOne;
};

class SetSampleCoverage: public SetStateTask::Runnable
{
public:
    SetSampleCoverage(GLfloat sampleCoverage)
    {
        this->sampleCoverage = sampleCoverage;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setSampleCoverage(sampleCoverage);
    }

private:
    GLfloat sampleCoverage;
};

class SetSampleMask : public SetStateTask::Runnable
{
public:
    SetSampleMask(GLuint sampleMask)
    {
        this->sampleMask = sampleMask;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setSampleMask(sampleMask);
    }

private:
    GLuint sampleMask;
};

class SetSampleShading : public SetStateTask::Runnable
{
public:
    SetSampleShading(bool sampleShading, GLfloat minSamples)
    {
        this->sampleShading = sampleShading;
        this->minSamples = minSamples;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setSampleShading(sampleShading, minSamples);
    }

private:
    bool sampleShading;
    GLfloat minSamples;
};

class SetOcclusionTest: public SetStateTask::Runnable
{
public:
    SetOcclusionTest(ptr<Query> occlusionQuery, QueryMode occlusionMode)
    {
        this->occlusionQuery = occlusionQuery;
        this->occlusionMode = occlusionMode;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setOcclusionTest(occlusionQuery, occlusionMode);
    }

private:
    ptr<Query> occlusionQuery;
    QueryMode occlusionMode;
};

class SetScissorTest: public SetStateTask::Runnable
{
public:
    SetScissorTest(bool enableScissor)
    {
        this->enableScissor = enableScissor;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setScissorTest(enableScissor);
    }

private:
    bool enableScissor;
};

class SetScissorTestValue: public SetStateTask::Runnable
{
public:
    SetScissorTestValue(bool enableScissor, vec4<GLint> &scissor)
    {
        this->enableScissor = enableScissor;
        this->scissor = scissor;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setScissorTest(enableScissor, scissor);
    }

private:
    bool enableScissor;
    vec4<GLint> scissor;
};

class SetStencilTest : public SetStateTask::Runnable
{
public:
    SetStencilTest(bool enableStencil,
    Function ff = (Function)-1, GLint fref = -1, GLuint fmask = 0, StencilOperation ffail = (StencilOperation)-1, StencilOperation fdpfail = (StencilOperation)-1, StencilOperation fdppass = (StencilOperation)-1,
    Function bf = (Function)-1, GLint bref = -1, GLuint bmask = 0, StencilOperation bfail = (StencilOperation)-1, StencilOperation bdpfail = (StencilOperation)-1, StencilOperation bdppass = (StencilOperation)-1)
    {
        this->enableStencil = enableStencil;
        this->ff = ff;
        this->fref = fref;
        this->fmask = fmask;
        this->ffail = ffail;
        this->fdpfail = fdpfail;
        this->fdppass = fdppass;
        this->bf = bf;
        this->bref = bref;
        this->bmask = bmask;
        this->bfail = bfail;
        this->bdpfail = bdpfail;
        this->bdppass = bdppass;
        // checks that correct parameters are provided if a function is set
        assert(ff == (Function)-1 || (fref != (StencilOperation)-1 && ffail != (StencilOperation)-1 && fdpfail != (StencilOperation)-1 && fdppass != (StencilOperation)-1));
        assert(bf == (Function)-1 || (bref != (StencilOperation)-1 && bfail != (StencilOperation)-1 && bdpfail != (StencilOperation)-1 && bdppass != (StencilOperation)-1));
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (ff == (Function)-1 && bf == (Function)-1) {
            fb->setStencilTest(enableStencil);
        } else if (bf == (Function)-1) {
            fb->setStencilTest(enableStencil, ff, fref, fmask, ffail, fdpfail, fdppass);
        } else {
            fb->setStencilTest(enableStencil, ff, fref, fmask, ffail, fdpfail, fdppass, bf, bref, bmask, bfail, bdpfail, bdppass);
        }
    }

private:
    bool enableStencil;

    Function ff;

    GLint fref;

    GLuint fmask;

    StencilOperation ffail;

    StencilOperation fdpfail;

    StencilOperation fdppass;

    Function bf;

    GLint bref;

    GLuint bmask;

    StencilOperation bfail;

    StencilOperation bdpfail;

    StencilOperation bdppass;
};

class SetDepthTest : public SetStateTask::Runnable
{
public:
    SetDepthTest(bool enableDepth, Function depth = (Function) -1)
    {
        this->enableDepth = enableDepth;
        this->depth = depth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (depth == (Function) -1) {
            fb->setDepthTest(enableDepth);
        } else {
            fb->setDepthTest(enableDepth, depth);
        }
    }

private:
    bool enableDepth;

    Function depth;
};

class SetBlend : public SetStateTask::Runnable
{
public:
    SetBlend(BufferId buffer, bool enableBlend, BlendEquation e = (BlendEquation)-1, BlendArgument src = (BlendArgument)-1, BlendArgument dst = (BlendArgument)-1,
        BlendEquation eAlpha = (BlendEquation)-1, BlendArgument srcAlpha = (BlendArgument)-1, BlendArgument dstAlpha = (BlendArgument)-1)
    {
        this->buffer = buffer;
        this->enableBlend = enableBlend;
        this->rgb = e;
        this->alpha = eAlpha;
        this->srgb = src;
        this->drgb = dst;
        this->salpha = srcAlpha;
        this->dalpha = dstAlpha;
        // checks that src & dst arguments are set when the equations are set.
        assert(e == BlendEquation(-1) || (src != BlendArgument(-1) && dst != BlendArgument(-1)));
        assert(eAlpha == BlendEquation(-1) || (srcAlpha != BlendArgument(-1) && dstAlpha != BlendArgument(-1)));
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (rgb == BlendEquation(-1) && buffer == BufferId(-1)) {
            fb->setBlend(enableBlend);
        } else if (rgb == BlendEquation(-1)) {
            fb->setBlend(buffer, enableBlend);
        } else if (alpha == BlendEquation(-1) && buffer == BufferId(-1)) {
            fb->setBlend(enableBlend, rgb, srgb, drgb);
        } else if (alpha == BlendEquation(-1)) {
            fb->setBlend(buffer, enableBlend, rgb, srgb, drgb);
        } else if (buffer == BufferId(-1)) {
            fb->setBlend(enableBlend, rgb, srgb, drgb, alpha, salpha, dalpha);
        } else {
            fb->setBlend(buffer, enableBlend, rgb, srgb, drgb, alpha, salpha, dalpha);
        }
    }

private:
    BufferId buffer;

    bool enableBlend;

    BlendEquation rgb;

    BlendEquation alpha;

    BlendArgument srgb;

    BlendArgument drgb;

    BlendArgument salpha;

    BlendArgument dalpha;
};

class SetBlendColor : public SetStateTask::Runnable
{
public:
    SetBlendColor(vec4<GLfloat> &color)
    {
        this->color = color;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setBlendColor(color);
    }

private:
    vec4<GLfloat> color;
};

class SetDither : public SetStateTask::Runnable
{
public:
    SetDither(bool enableDither)
    {
        this->enableDither = enableDither;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setDither(enableDither);
    }

private:
    bool enableDither;
};

class SetLogicOp: public SetStateTask::Runnable
{
public:
    SetLogicOp(bool enableLogic, LogicOperation logicOp = (LogicOperation)-1)
    {
        this->enableLogic = enableLogic;
        this->logicOp = logicOp;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (logicOp == (LogicOperation)-1) {
            fb->setLogicOp(enableLogic);
        } else {
            fb->setLogicOp(enableLogic, logicOp);
        }
    }

private:
    bool enableLogic;

    LogicOperation logicOp;
};

class SetColorMask : public SetStateTask::Runnable
{
public:
    SetColorMask(BufferId buffer, bool r, bool g, bool b, bool a)
    {
        this->buffer = buffer;
        this-> r = r;
        this-> g = g;
        this-> b = b;
        this-> a = a;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (buffer != BufferId(-1)) {
            fb->setColorMask(buffer, r, g, b, a);
        } else {
            fb->setColorMask(r, g, b, a);
        }
    }

private:
    BufferId buffer;

    bool r;

    bool g;

    bool b;

    bool a;
};

class SetDepthMask : public SetStateTask::Runnable
{
public:
    SetDepthMask(bool d)
    {
        this->d = d;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setDepthMask(d);
    }

private:
    bool d;
};

class SetStencilMask : public SetStateTask::Runnable
{
public:
    SetStencilMask(GLuint frontMask, GLuint backMask)
    {
        this->frontMask = frontMask;
        this->backMask = backMask;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->setStencilMask(frontMask, backMask);
    }

private:
    GLuint frontMask;

    GLuint backMask;
};

class SetClearState : public SetStateTask::Runnable
{
public:
    SetClearState(bool color, bool stencil, bool depth)
    {
        this->color = color;
        this->stencil = stencil;
        this->depth = depth;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        fb->clear(color, stencil, depth);
    }

private:
    bool color;

    bool stencil;

    bool depth;
};

class SetBuffers : public SetStateTask::Runnable
{
public:
    SetBuffers(BufferId rb, BufferId db)
    {
        this->rb = rb;
        this->db = db;
    }

    virtual void run(ptr<FrameBuffer> fb)
    {
        if (rb != BufferId(-1)) {
            fb->setReadBuffer(rb);
        }
        if (db != BufferId(-1)) {
            if (db == COLOR0 || db == COLOR1 || db == COLOR2 || db == COLOR3 ||
                db == COLOR4 || db == COLOR5 || db == COLOR6 || db == COLOR7)
            {
                fb->setDrawBuffer(db);
            } else {
                fb->setDrawBuffers(db);
            }
        }
    }

private:
    BufferId rb;

    BufferId db;
};

SetStateTask::SetStateTask() : AbstractTask("SetStateTask")
{
}

SetStateTask::~SetStateTask()
{
    for (unsigned int i = 0; i < runnables.size(); ++i) {
        delete runnables[i];
    }
    runnables.clear();
}

void SetStateTask::addRunnable(Runnable *r)
{
    runnables.push_back(r);
}

ptr<Task> SetStateTask::getTask(ptr<Object> context)
{
    return new Impl(this);
}

void SetStateTask::run()
{
    ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();
    for (unsigned int i = 0; i < runnables.size(); ++i) {
        runnables[i]->run(fb);
    }
}

void SetStateTask::setViewport(vec4<GLint> vp)
{
    addRunnable(new SetViewport(vp));
}

void SetStateTask::setDepthRange(GLfloat n, GLfloat f)
{
    addRunnable(new SetDepthRange(n, f));
}

void SetStateTask::setClipDistances(GLint d)
{
    addRunnable(new SetClipDistances(d));
}

void SetStateTask::setClearColor(vec4<GLfloat> &c)
{
    addRunnable(new SetClearColor(c));
}

void SetStateTask::setClearDepth(GLfloat clearDepth)
{
    addRunnable(new SetClearDepth(clearDepth));
}

void SetStateTask::setClearStencil(GLint clearStencil)
{
    addRunnable(new SetClearStencil(clearStencil));
}

void SetStateTask::setPointSize(GLfloat pointSize)
{
    addRunnable(new SetPointSize(pointSize));
}

void SetStateTask::setPointFadeThresholdSize(GLfloat tSize)
{
    addRunnable(new SetPointFadeThresholdSize(tSize));
}

void SetStateTask::setPointLowerLeftOrigin(bool pointLowerLeftOrigin)
{
    addRunnable(new SetPointLowerLeftOrigin(pointLowerLeftOrigin));
}

void SetStateTask::setLineWidth(GLfloat lineWidth)
{
    addRunnable(new SetLineWidth(lineWidth));
}

void SetStateTask::setLineSmooth(bool lineSmooth)
{
    addRunnable(new SetLineSmooth(lineSmooth));
}

void SetStateTask::setFrontFaceCW(bool frontFaceCW)
{
    addRunnable(new SetFrontFaceCW(frontFaceCW));
}

void SetStateTask::setPolygonMode(PolygonMode polygonFront, PolygonMode polygonBack)
{
    addRunnable(new SetPolygonMode(polygonFront, polygonBack));
}

void SetStateTask::setPolygonSmooth(bool polygonSmooth)
{
    addRunnable(new SetPolygonSmooth(polygonSmooth));
}

void SetStateTask::setPolygonOffset(GLfloat factor, GLfloat units)
{
    addRunnable(new SetPolygonOffset(factor, units));
}

void SetStateTask::setPolygonOffset(bool pointOffset, bool lineOffset, bool polygonOffset)
{
    addRunnable(new SetPolygonOffsets(pointOffset, lineOffset, polygonOffset));
}

void SetStateTask::setMultisample(bool multiSample)
{
    addRunnable(new SetMultisample(multiSample));
}

void SetStateTask::setSampleAlpha(bool sampleAlphaToCoverage, bool sampleAlphaToOne)
{
    addRunnable(new SetSampleAlpha(sampleAlphaToCoverage, sampleAlphaToOne));
}

void SetStateTask::setSampleCoverage(GLfloat sampleCoverage)
{
    addRunnable(new SetSampleCoverage(sampleCoverage));
}

void SetStateTask::setSampleMask(GLint sampleMask)
{
    addRunnable(new SetSampleMask(sampleMask));
}

void SetStateTask::setSampleShading(bool sampleShading, GLfloat minSamples)
{
    addRunnable(new SetSampleShading(sampleShading, minSamples));
}

void SetStateTask::setOcclusionTest(ptr<Query> occlusionQuery, QueryMode occlusionMode)
{
    addRunnable(new SetOcclusionTest(occlusionQuery, occlusionMode));
}

void SetStateTask::setScissorTest(bool enableScissor)
{
    addRunnable(new SetScissorTest(enableScissor));
}

void SetStateTask::setScissorTest(bool enableScissor, vec4<GLint> &scissor)
{
    addRunnable(new SetScissorTestValue(enableScissor, scissor));
}

void SetStateTask::setStencilTest(bool enableStencil,
    Function ff, GLint fref, GLint fmask, StencilOperation ffail, StencilOperation fdpfail, StencilOperation fdppass,
    Function bf, GLint bref, GLint bmask, StencilOperation bfail, StencilOperation bdpfail, StencilOperation bdppass)
{
    addRunnable(new SetStencilTest(enableStencil,
        ff, fref, fmask, ffail, fdpfail, fdppass,
        bf, bref, bmask, bfail, bdpfail, bdppass));
}

void SetStateTask::setDepthTest(bool enableDepth, Function depth)
{
    addRunnable(new SetDepthTest(enableDepth, depth));
}

void SetStateTask::setBlend(BufferId buffer, bool enableBlend, BlendEquation e, BlendArgument src, BlendArgument dst,
    BlendEquation eAlpha, BlendArgument srcAlpha, BlendArgument dstAlpha)
{
    addRunnable(new SetBlend(buffer, enableBlend, e, src, dst, eAlpha, srcAlpha, dstAlpha));
}

void SetStateTask::setBlendColor(vec4<GLfloat> &color)
{
    addRunnable(new SetBlendColor(color));
}

void SetStateTask::setDither(bool enableDither)
{
    addRunnable(new SetDither(enableDither));
}

void SetStateTask::setLogicOp(bool enableLogic, LogicOperation logicOp)
{
    addRunnable(new SetLogicOp(enableLogic, logicOp));
}

void SetStateTask::setColorMask(BufferId buffer, bool r, bool g, bool b, bool a)
{
    addRunnable(new SetColorMask(buffer, r, g, b, a));
}

void SetStateTask::setDepthMask(bool d)
{
    addRunnable(new SetDepthMask(d));
}

void SetStateTask::setStencilMask(GLint frontMask, GLint backMask)
{
    addRunnable(new SetStencilMask(frontMask, backMask));
}

void SetStateTask::setClearState(bool color, bool stencil, bool depth)
{
    addRunnable(new SetClearState(color, stencil, depth));
}

void SetStateTask::setBuffers(BufferId rb, BufferId db)
{
    addRunnable(new SetBuffers(rb, db));
}

void SetStateTask::swap(ptr<SetStateTask> t)
{
    std::swap(runnables, t->runnables);
}

SetStateTask::Impl::Impl(ptr<SetStateTask> source) :
    Task("SetState", true, 0), source(source)
{
}

SetStateTask::Impl::~Impl()
{
}

bool SetStateTask::Impl::run()
{
    if (Logger::DEBUG_LOGGER != NULL) {
        Logger::DEBUG_LOGGER->log("SCENEGRAPH", "SetState");
    }
    source->run();
    return true;
}

/// @cond RESOURCES

class SetStateTaskResource : public ResourceTemplate<40, SetStateTask>
{
public:
    BlendEquation getBlendEquation(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "ADD") == 0) {
                return ADD;
            }
            if (strcmp(e->Attribute(name), "SUBTRACT") == 0) {
                return SUBTRACT;
            }
            if (strcmp(e->Attribute(name), "REVERSE_SUBTRACT") == 0) {
                return REVERSE_SUBTRACT;
            }
            if (strcmp(e->Attribute(name), "MIN") == 0) {
                return MIN;
            }
            if (strcmp(e->Attribute(name), "MAX") == 0) {
                return MAX;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid blend equation");
        }
        throw exception();
    }

    BlendArgument getBlendArgument(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "ZERO") == 0) {
                return ZERO;
            }
            if (strcmp(e->Attribute(name), "ONE") == 0) {
                return ONE;
            }
            if (strcmp(e->Attribute(name), "SRC_COLOR") == 0) {
                return SRC_COLOR;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_SRC_COLOR") == 0) {
                return ONE_MINUS_SRC_COLOR;
            }
            if (strcmp(e->Attribute(name), "DST_COLOR") == 0) {
                return DST_COLOR;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_DST_COLOR") == 0) {
                return ONE_MINUS_DST_COLOR;
            }
            if (strcmp(e->Attribute(name), "SRC_ALPHA") == 0) {
                return SRC_ALPHA;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_SRC_ALPHA") == 0) {
                return ONE_MINUS_SRC_ALPHA;
            }
            if (strcmp(e->Attribute(name), "DST_ALPHA") == 0) {
                return DST_ALPHA;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_DST_ALPHA") == 0) {
                return ONE_MINUS_DST_ALPHA;
            }
            if (strcmp(e->Attribute(name), "CONSTANT_COLOR") == 0) {
                return CONSTANT_COLOR;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_CONSTANT_COLOR") == 0) {
                return ONE_MINUS_CONSTANT_COLOR;
            }
            if (strcmp(e->Attribute(name), "CONSTANT_ALPHA") == 0) {
                return CONSTANT_ALPHA;
            }
            if (strcmp(e->Attribute(name), "ONE_MINUS_CONSTANT_ALPHA") == 0) {
                return ONE_MINUS_CONSTANT_ALPHA;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid blend argument");
        }
        throw exception();
    }

    Function getFunction(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "NEVER") == 0) {
                return NEVER;
            }
            if (strcmp(e->Attribute(name), "ALWAYS") == 0) {
                return ALWAYS;
            }
            if (strcmp(e->Attribute(name), "LESS") == 0) {
                return LESS;
            }
            if (strcmp(e->Attribute(name), "LEQUAL") == 0) {
                return LEQUAL;
            }
            if (strcmp(e->Attribute(name), "EQUAL") == 0) {
                return EQUAL;
            }
            if (strcmp(e->Attribute(name), "GREATER") == 0) {
                return GREATER;
            }
            if (strcmp(e->Attribute(name), "GEQUAL") == 0) {
                return GEQUAL;
            }
            if (strcmp(e->Attribute(name), "NOTEQUAL") == 0) {
                return NOTEQUAL;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid function");
        }
        throw exception();
    }

    StencilOperation getStencilOperation(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "KEEP") == 0) {
                return KEEP;
            }
            if (strcmp(e->Attribute(name), "RESET") == 0) {
                return RESET;
            }
            if (strcmp(e->Attribute(name), "REPLACE") == 0) {
                return REPLACE;
            }
            if (strcmp(e->Attribute(name), "INCR") == 0) {
                return INCR;
            }
            if (strcmp(e->Attribute(name), "DECR") == 0) {
                return DECR;
            }
            if (strcmp(e->Attribute(name), "INVERT") == 0) {
                return INVERT;
            }
            if (strcmp(e->Attribute(name), "INCR_WRAP") == 0) {
                return INCR_WRAP;
            }
            if (strcmp(e->Attribute(name), "DECR_WRAP") == 0) {
                return DECR_WRAP;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid Stencil Operation");
        }
        throw exception();
    }

    LogicOperation getLogicOperation(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "CLEAR") == 0) {
                return CLEAR;
            }
            if (strcmp(e->Attribute(name), "AND") == 0) {
                return AND;
            }
            if (strcmp(e->Attribute(name), "AND_REVERSE") == 0) {
                return AND_REVERSE;
            }
            if (strcmp(e->Attribute(name), "COPY") == 0) {
                return COPY;
            }
            if (strcmp(e->Attribute(name), "AND_INVERTED") == 0) {
                return AND_INVERTED;
            }
            if (strcmp(e->Attribute(name), "NOOP") == 0) {
                return NOOP;
            }
            if (strcmp(e->Attribute(name), "XOR") == 0) {
                return XOR;
            }
            if (strcmp(e->Attribute(name), "OR") == 0) {
                return OR;
            }
            if (strcmp(e->Attribute(name), "NOR") == 0) {
                return NOR;
            }
            if (strcmp(e->Attribute(name), "EQUIV") == 0) {
                return EQUIV;
            }
            if (strcmp(e->Attribute(name), "NOT") == 0) {
                return NOT;
            }
            if (strcmp(e->Attribute(name), "OR_REVERSE") == 0) {
                return OR_REVERSE;
            }
            if (strcmp(e->Attribute(name), "COPY_INVERTED") == 0) {
                return COPY_INVERTED;
            }
            if (strcmp(e->Attribute(name), "OR_INVERTED") == 0) {
                return OR_INVERTED;
            }
            if (strcmp(e->Attribute(name), "NAND") == 0) {
                return NAND;
            }
            if (strcmp(e->Attribute(name), "SET") == 0) {
                return SET;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid Logic Operation");
        }
        throw exception();
    }

    QueryType getQueryType(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "PRIMITIVES_GENERATED") == 0) {
                return PRIMITIVES_GENERATED;
            }
            if (strcmp(e->Attribute(name), "TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN") == 0) {
                return TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
            }
            if (strcmp(e->Attribute(name), "SAMPLES_PASSED") == 0) {
                return SAMPLES_PASSED;
            }
            if (strcmp(e->Attribute(name), "ANY_SAMPLES_PASSED") == 0) {
                return ANY_SAMPLES_PASSED;
            }
            if (strcmp(e->Attribute(name), "TIME_STAMP") == 0) {
                return TIME_STAMP;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid Query Type");
        }
        throw exception();
    }

    QueryMode getQueryMode(ptr<ResourceDescriptor> desc, const TiXmlElement *e, const char *name)
    {
        try {
            if (strcmp(e->Attribute(name), "WAIT") == 0) {
                return WAIT;
            }
            if (strcmp(e->Attribute(name), "NO_WAIT") == 0) {
                return NO_WAIT;
            }
            if (strcmp(e->Attribute(name), "REGION_WAIT") == 0) {
                return REGION_WAIT;
            }
            if (strcmp(e->Attribute(name), "REGION_NO_WAIT") == 0) {
                return REGION_NO_WAIT;
            }
        } catch (...) {
        }
        if (Logger::ERROR_LOGGER != NULL) {
            log(Logger::ERROR_LOGGER, desc, e, "Invalid Query Mode");
        }
        throw exception();
    }

    SetStateTaskResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, SetStateTask>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "readBuffer,drawBuffer,clearColor,clearStencil,clearDepth,");

        BufferId rb = (BufferId) -1;
        BufferId db = (BufferId) -1;
        bool clearColor = e->Attribute("clearColor") != NULL && strcmp(e->Attribute("clearColor"), "true") == 0;
        bool clearStencil = e->Attribute("clearStencil") != NULL && strcmp(e->Attribute("clearStencil"), "true") == 0;
        bool clearDepth = e->Attribute("clearDepth") != NULL && strcmp(e->Attribute("clearDepth"), "true") == 0;

        try {
            if (e->Attribute("readBuffer") != NULL) {
                rb = getBufferFromName(e->Attribute("readBuffer"));
            }
            if (e->Attribute("drawBuffer") != NULL) {
                db = BufferId(0);
                string names = getParameter(desc, e, "drawBuffer") + ",";
                string::size_type start = 0;
                string::size_type index;
                while ((index = names.find(',', start)) != string::npos) {
                    string name = names.substr(start, index - start);
                    db = BufferId(db | getBufferFromName(name.c_str()));
                    start = index + 1;
                }
            }
        } catch (...) {
            if (Logger::ERROR_LOGGER != NULL) {
                log(Logger::ERROR_LOGGER, desc, e, "Invalid buffer name");
            }
            throw exception();
        }

        addRunnable(new SetBuffers(rb, db));

        const TiXmlNode *n = e->FirstChild();
        while (n != NULL) {
            const TiXmlElement *f = n->ToElement();
            if (f == NULL) {
                n = n->NextSibling();
                continue;
            }
            if (strcmp(f->Value(), "viewport") == 0) {
                checkParameters(desc, f, "x,y,width,height,");
                vec4<GLint> vp = vec4<GLint>::ZERO;
                getIntParameter(desc, f, "x", &vp.x);
                getIntParameter(desc, f, "y", &vp.y);
                getIntParameter(desc, f, "width", &vp.z);
                getIntParameter(desc, f, "height", &vp.w);
                addRunnable(new SetViewport(vp));
            } else if (strcmp(f->Value(), "depthRange") == 0) {
                checkParameters(desc, f, "near,far,");
                float near;
                float far;
                getFloatParameter(desc, f, "near", &near);
                getFloatParameter(desc, f, "far", &far);
                addRunnable(new SetDepthRange(near, far));
            } else if (strcmp(f->Value(), "clipDistances") == 0) {
                checkParameters(desc, f, "value,");
                int val;
                getIntParameter(desc, f, "value", &val);
                addRunnable(new SetClipDistances(val));
            } else if (strcmp(f->Value(), "blend") == 0) {
                if (f->FirstAttribute() == NULL) {
                    addRunnable(new SetBlend(BufferId(-1), false));
                } else {
                    checkParameters(desc, f, "buffer,enable,r,g,b,a,eq,alphaeq,src,dst,alphasrc,alphadst,");
                    BufferId b = BufferId(-1);
                    bool enable = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;
                    BlendEquation rgbEq = BlendEquation(-1);
                    BlendEquation alphaEq = BlendEquation(-1);
                    BlendArgument srgbf = BlendArgument(-1);
                    BlendArgument drgbf = BlendArgument(-1);
                    BlendArgument salphaf = BlendArgument(-1);
                    BlendArgument dalphaf = BlendArgument(-1);

                    if (f->Attribute("buffer") != NULL) {
                        b = BufferId(0);
                        string names = getParameter(desc, f, "buffer") + ",";
                        string::size_type start = 0;
                        string::size_type index;
                        while ((index = names.find(',', start)) != string::npos) {
                            string name = names.substr(start, index - start);
                            b = BufferId(b | getBufferFromName(name.c_str()));
                            start = index + 1;
                        }
                    }
                    if (f->Attribute("eq") != NULL) {
                        rgbEq = getBlendEquation(desc, f, "eq");
                        srgbf = getBlendArgument(desc, f, "src");
                        drgbf = getBlendArgument(desc, f, "dst");
                        if (f->Attribute("alphaeq") != NULL) {
                            alphaEq = getBlendEquation(desc, f, "alphaeq");
                            salphaf = getBlendArgument(desc, f, "alphasrc");
                            dalphaf = getBlendArgument(desc, f, "alphadst");
                        }
                    }
                    addRunnable(new SetBlend(b, enable, rgbEq, srgbf, drgbf, alphaEq, salphaf, dalphaf));

                    if (f->Attribute("r") != NULL) {
                        vec4<GLfloat> color;
                        getFloatParameter(desc, f, "r", &color.x);
                        getFloatParameter(desc, f, "g", &color.y);
                        getFloatParameter(desc, f, "b", &color.z);
                        getFloatParameter(desc, f, "a", &color.w);

                        addRunnable(new SetBlendColor(color));
                    }
                }
            } else if (strcmp(f->Value(), "clear") == 0) {
                vec4<GLfloat> c = vec4<GLfloat>(0.0, 0.0, 0.0, 0.0);
                GLint s = 0;
                GLfloat d = 1.0;
                checkParameters(desc, f, "r,g,b,a,stencil,depth,");
                if (f->Attribute("r") != NULL) {
                    getFloatParameter(desc, f, "r", &c.x);
                    getFloatParameter(desc, f, "g", &c.y);
                    getFloatParameter(desc, f, "b", &c.z);
                    getFloatParameter(desc, f, "a", &c.w);
                    addRunnable(new SetClearColor(c));
                }

                if (f->Attribute("stencil") != NULL) {
                    getIntParameter(desc, f, "stencil", &s);
                    addRunnable(new SetClearStencil(s));
                }
                if (f->Attribute("depth") != NULL) {
                    getFloatParameter(desc, f, "depth", &d);
                    addRunnable(new SetClearDepth(d));
                }
            } else if (strcmp(f->Value(), "point") == 0) {
                checkParameters(desc, f, "size,threshold,lowerleftorigin,");
                GLfloat s;

                if (f->Attribute("size") != NULL) {
                    getFloatParameter(desc, f, "size", &s);
                    addRunnable(new SetPointSize(s));
                }
                if (f->Attribute("threshold") != NULL) {
                    getFloatParameter(desc, f, "threshold", &s);
                    addRunnable(new SetPointFadeThresholdSize(s));
                }
                if (f->Attribute("lowerleftorigin") != NULL) {
                    bool llo = strcmp(f->Attribute("clearColor"), "true") == 0;
                    addRunnable(new SetPointLowerLeftOrigin(llo));
                }

            } else if (strcmp(f->Value(), "line") == 0) {
                checkParameters(desc, f, "width,smooth,");
                if (f->Attribute("smooth") != NULL) {
                    bool smooth = strcmp(f->Attribute("smooth"), "true") == 0;
                    addRunnable(new SetLineSmooth(smooth));
                }
                if (f->Attribute("width") != NULL) {
                    GLfloat width;
                    getFloatParameter(desc, f, "width", &width);
                    addRunnable(new SetLineWidth(width));
                }
            } else if (strcmp(f->Value(), "polygon") == 0) {
                string v[2];
                bool frontCW;
                bool smooth;
                bool pointOffset;
                bool lineOffset;
                bool polygonOffset;
                GLfloat factor;
                GLfloat units;
                PolygonMode cull[2];
                checkParameters(desc, f, "frontCW,front,back,smooth,offsetFactor,offsetUnits,pointOffset,lineOffset,polygonOffset,");

                if (f->Attribute("front") != NULL) {
                    v[0] = getParameter(desc, f, "front");
                    v[1] = getParameter(desc, f, "back");
                    for (int i = 0; i < 2; ++i) {
                        if (strcmp(v[i].c_str(), "CULL") == 0) {
                            cull[i] = CULL;
                        } else if (strcmp(v[i].c_str(), "LINE") == 0) {
                            cull[i] = LINE;
                        } else if (strcmp(v[i].c_str(), "FILL") == 0) {
                            cull[i] = FILL;
                        } else {
                            if (Logger::ERROR_LOGGER != NULL) {
                                log(Logger::ERROR_LOGGER, desc, f, "Invalid cull value");
                            }
                            throw exception();
                        }
                    }
                    addRunnable(new SetPolygonMode(cull[0], cull[1]));
                }
                if (f->Attribute("frontCW") != NULL) {
                    frontCW = strcmp(f->Attribute("frontCW"), "true") == 0;
                    addRunnable(new SetFrontFaceCW(frontCW));
                }
                if (f->Attribute("smooth") != NULL) {
                    smooth = strcmp(f->Attribute("smooth"), "true") == 0;
                    addRunnable(new SetPolygonSmooth(smooth));
                }
                if (f->Attribute("offsetFactor") != NULL) {
                    getFloatParameter(desc, f, "factor", &factor);
                    getFloatParameter(desc, f, "units", &units);

                    addRunnable(new SetPolygonOffset(factor, units));
                }
                if (f->Attribute("pointOffset") != NULL) {
                    pointOffset = strcmp(f->Attribute("pointOffset"), "true") == 0;
                    lineOffset = strcmp(f->Attribute("lineOffset"), "true") == 0;
                    polygonOffset = strcmp(f->Attribute("polygonOffset"), "true") == 0;
                    addRunnable(new SetPolygonOffsets(pointOffset, lineOffset, polygonOffset));
                }

            } else if (strcmp(f->Value(), "depth") == 0) {
                checkParameters(desc, f, "enable,value,");
                Function depth = Function(-1);
                bool enable;
                enable = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;
                if (f->Attribute("value") != NULL) {
                    depth = getFunction(desc, f, "value");
                }

                addRunnable(new SetDepthTest(enable, depth));
            } else if (strcmp(f->Value(), "stencil") == 0) {
                checkParameters(desc, f, "enable,ffunction,bfunction,fref,bref,fmask,bmask,ffail,bfail,fdpfail,bdpfail,fdppass,bdppass,");
                bool enableStencil = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;

                Function ff = Function(-1);
                GLint fref = -1;
                int fmask = 0;
                StencilOperation ffail = StencilOperation(-1);
                StencilOperation fdpfail = StencilOperation(-1);
                StencilOperation fdppass = StencilOperation(-1);

                Function bf = Function(-1);
                GLint bref = -1;
                int bmask = 0;
                StencilOperation bfail = StencilOperation(-1);
                StencilOperation bdpfail = StencilOperation(-1);
                StencilOperation bdppass = StencilOperation(-1);
                if (f->Attribute("ffunction") != NULL) {
                    ff = getFunction(desc, f, "ffunction");
                    getIntParameter(desc, f, "fref", &fref);
                    getIntParameter(desc, f, "fmask", &fmask);
                    ffail = getStencilOperation(desc, f, "ffail");
                    ffail = getStencilOperation(desc, f, "fdpfail");
                    ffail = getStencilOperation(desc, f, "fdppass");
                }
                if (f->Attribute("bfunction") != NULL) {
                    bf = getFunction(desc, f, "bfunction");
                    getIntParameter(desc, f, "bref", &bref);
                    getIntParameter(desc, f, "bmask", &bmask);
                    bfail = getStencilOperation(desc, f, "bfail");
                    bfail = getStencilOperation(desc, f, "bdpfail");
                    bfail = getStencilOperation(desc, f, "bdppass");
                }

                addRunnable(new SetStencilTest(enableStencil, ff, fref, GLuint(fmask), ffail, fdpfail, fdppass, bf, bref, GLuint(bmask), bfail, bdpfail, bdppass));
            } else if (strcmp(f->Value(), "write") == 0) {
                checkParameters(desc, f, "buffer,r,g,b,a,d,fs,bs,");
                BufferId id = BufferId(-1);
                if (f->Attribute("buffer") != NULL) {
                    id = BufferId(0);
                    string names = getParameter(desc, f, "buffer") + ",";
                    string::size_type start = 0;
                    string::size_type index;
                    while ((index = names.find(',', start)) != string::npos) {
                        string name = names.substr(start, index - start);
                        id = BufferId(id | getBufferFromName(name.c_str()));
                        start = index + 1;
                    }
                }

                if(f->Attribute("r") != NULL) {
                    bool r = strcmp(f->Attribute("r"), "true") == 0;
                    bool g = strcmp(f->Attribute("g"), "true") == 0;
                    bool b = strcmp(f->Attribute("b"), "true") == 0;
                    bool a = strcmp(f->Attribute("a"), "true") == 0;
                    addRunnable(new SetColorMask(id, r, g, b, a));
                }
                if (f->Attribute("d") != NULL) {
                    bool d = strcmp(f->Attribute("d"), "true") == 0;
                    addRunnable(new SetDepthMask(d));
                }
                if (f->Attribute("fs") != NULL) {
                    int fs;
                    int bs;
                    getIntParameter(desc, f, "fs", &fs);
                    getIntParameter(desc, f, "bs", &bs);
                    addRunnable(new SetStencilMask(GLuint(fs), GLuint(bs)));
                }
            } else if (strcmp(f->Value(), "logic") == 0) {
                checkParameters(desc, f, "enable,value,");
                LogicOperation l = LogicOperation(-1);
                bool enable = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;
                if (f->Attribute("value") != NULL) {
                    l = getLogicOperation(desc, f, "value");
                }
                addRunnable(new SetLogicOp(enable, l));
            } else if (strcmp(f->Value(), "scissor") == 0) {
                checkParameters(desc, f, "enable,x,y,width,height,");
                bool enable = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;
                if (f->Attribute("x") != NULL) {
                    vec4<GLint> vp;
                    getIntParameter(desc, f, "x", &vp.x);
                    getIntParameter(desc, f, "y", &vp.y);
                    getIntParameter(desc, f, "width", &vp.z);
                    getIntParameter(desc, f, "height", &vp.w);
                    addRunnable(new SetScissorTestValue(enable, vp));
                } else {
                    addRunnable(new SetScissorTest(enable));
                }
            } else if (strcmp(f->Value(), "occlusion") == 0) {
                checkParameters(desc, f, "query,mode,");
                ptr<Query> q;
                QueryMode m = QueryMode(-1);

                m = getQueryMode(desc, f, "mode");
                q = new Query(getQueryType(desc, f, "query"));
                addRunnable(new SetOcclusionTest(q, m));

            } else if (strcmp(f->Value(), "multisampling") == 0) {
                checkParameters(desc, f, "enable,alphaToCoverage,alphaToOne,coverage,mask,shading,min,");
                bool enable = f->Attribute("enable") != NULL && strcmp(f->Attribute("enable"), "true") == 0;
                addRunnable(new SetMultisample(enable));

                if (f->Attribute("alphaToCoverage") != NULL) {
                    bool ac = strcmp(f->Attribute("alphaToCoverage"), "true") == 0;
                    bool ao = strcmp(f->Attribute("alphaToOne"), "true") == 0;
                    addRunnable(new SetSampleAlpha(ac, ao));
                }

                if (f->Attribute("coverage") != NULL) {
                    GLfloat c;
                    getFloatParameter(desc, f, "coverage", &c);
                    addRunnable(new SetSampleCoverage(c));
                }

                if (f->Attribute("mask") != NULL) {
                    int m;
                    getIntParameter(desc, f, "mask", &m);
                    addRunnable(new SetSampleMask(GLuint(m)));
                }

                if (f->Attribute("shading") != NULL) {
                    GLfloat m;
                    enable = strcmp(f->Attribute("shading"), "true") == 0;
                    getFloatParameter(desc, f, "min", &m);
                    addRunnable(new SetSampleShading(enable, m));
                }
            } else {
                if (Logger::ERROR_LOGGER != NULL) {
                    log(Logger::ERROR_LOGGER, desc, f, "Invalid sub element");
                }
                throw exception();
            }
            n = n->NextSibling();
        }

        addRunnable(new SetClearState(clearColor, clearStencil, clearDepth));
    }
};

extern const char setState[] = "setState";

static ResourceFactory::Type<setState, SetStateTaskResource> SetStateTaskType;

/// @endcond

}
