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

#ifndef _ORK_TRANSFORM_FEEDBACK_H_
#define _ORK_TRANSFORM_FEEDBACK_H_

#include "ork/render/GPUBuffer.h"
#include "ork/render/Program.h"

namespace ork
{

/**
 * A set of Buffer objects to collect the result of a transform feedback.
 * In OpenGL 3.3 only a single TransformFeedback instance can be used, the
 * one returned by#getDefault. With OpenGL 4 other instances can be used,
 * which can be created with the constructor. In any case, only one transform
 * feedback can be performed at a time, with the static #begin, #transform
 * and #end methods.
 *
 * @ingroup render
 */
class ORK_API TransformFeedback : public Object
{
public:
    /**
     * Creates a new TransformFeedback object. Only works with OpenGL 4.0 or
     * more.
     */
    TransformFeedback();

    /**
     * Deletes this TransformFeedback object.
     */
    virtual ~TransformFeedback();

    /**
     * Returns the default TransformFeedback instance.
     */
    static ptr<TransformFeedback> getDefault();

    /**
     * Removes all the buffers associated with this object.
     */
    void reset();

    /**
     * Attachs the given GPUBuffer to collect the transformed output varying
     * of the given index.
     *
     * @param index the index of a recorded output varying variable.
     * @param b the GPUBuffer to use to store the recorded values of this varying.
     */
    void setVertexBuffer(int index, ptr<GPUBuffer> b);

    /**
     * Attachs the given GPUBuffer to collect the transformed output varying
     * of the given index.
     *
     * @param index the index of a recorded output varying variable.
     * @param b the GPUBuffer to use to store the recorded values of this varying.
     * @param offset the offset at which the first recorded value must be stored.
     * @param size the maximum size of the recorded values.
     */
    void setVertexBuffer(int index, ptr<GPUBuffer> b, GLuint offset, GLuint size);

    /**
     * Starts a transform feedback session. Actual transforms are performed
     * with the #transform methods. The transformation is ended with the #end()
     * method. In OpenGL 4.0, #pause and #resume can be called between #begin()
     * and #end(), for instance to change the TransformFeedback instance used
     * to record the varyings.
     *
     * @param fb the framebuffer to use for this session.
     * @param transform the program to use for this session.
     * @param m how the mesh vertices must be interpreted in #transform methods.
     * @param tfb the set of buffers to use to store the results of the session,
     *      i.e., the transformed output varying variables.
     * @param rasterize true to rasterize the transformed primitives, or false
     *      to disable the rasterization stage during this session.
     */
    static void begin(ptr<FrameBuffer> fb, ptr<Program> transform, MeshMode m, ptr<TransformFeedback> tfb, bool rasterize);

    /**
     * Transforms a part of a mesh one or more times.
     *
     * @param mesh the mesh to transform.
     * @param first the first vertex to draw, or the first indice to draw if
     *      this mesh has indices.
     * @param count the number of vertices to draw, or the number of indices
     *      to draw if this mesh has indices.
     * @param primCount the number of times this mesh must be drawn (with
     *      geometry instancing).
     * @param base the base vertex to use. Only used for meshes with indices.
     */
    static void transform(const MeshBuffers &mesh, GLint first, GLsizei count, GLsizei primCount = 1, GLint base = 0);

    /**
     * Transforms several parts of a mesh. Each part is specified with a first
     * and count parameter as in #transform(). These values are passed in arrays
     * of primCount values.
     *
     * @param mesh the mesh to transform.
     * @param firsts an array of primCount 'first vertex' to draw, or an array
     *      of 'first indice' to draw if this mesh has indices.
     * @param counts an array of number of vertices to draw, or an array of
     *      number of indices to draw if this mesh has indices.
     * @param primCount the number of parts of this mesh to draw.
     * @param bases the base vertices to use. Only used for meshes with indices.
     */
    static void multiTransform(const MeshBuffers &mesh, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint* bases = 0);

    /**
     * Transforms a part of a mesh one or more times.
     *
     * @param mesh the mesh to transform.
     * @param buf a CPU or GPU buffer containing the 'count', 'primCount',
     *      'first' and 'base' parameters, in this order, as 32 bit integers.
     */
    static void transformIndirect(const MeshBuffers &mesh, const Buffer &buf);

    /**
     * Retransforms a mesh resulting from a previous transform feedback session.
     * Only available with OpenGL 4.0 or more.
     *
     * @param mesh the mesh to retransform.
     * @param tfb a TransformFeedback containing the results of a previous transform
     *      feedback session.
     * @param stream the stream to draw.
     */
    void transformFeedback(const MeshBuffers &mesh, const TransformFeedback &tfb, int stream = 0);

    /**
     * Pauses the current transform feedback session.
     * Only available with OpenGL 4.0 or more.
     */
    static void pause();

    /**
     * Resumes the current transform feedback session.
     * Only available with OpenGL 4.0 or more.
     *
     * @param tfb the set of buffers to use to store the results of the session,
     *      i.e., the transformed output varying variables.
     */
    static void resume(ptr<TransformFeedback> tfb);

    /**
     * Ends the current transform feedback session.
     */
    static void end();

private:
    /**
     * The id of this transform feedback object.
     */
    GLuint id;

    /**
     * The default transform feedback instance.
     */
    static static_ptr<TransformFeedback> DEFAULT;

    /**
     * The program to use for the current transform feedback session.
     */
    static ptr<Program> TRANSFORM;

    /**
     * How the mesh vertices must be interpreted in #transform methods.
     */
    static MeshMode MODE;

    /**
     * Creates a new TransformFeedback object.
     *
     * @param main true to create the default instance.
     */
    TransformFeedback(bool main);

    /**
     * Binds the transform feedback object whose id is given.
     */
    static void bind(GLuint id);

    friend class FrameBuffer;
};

}

#endif
