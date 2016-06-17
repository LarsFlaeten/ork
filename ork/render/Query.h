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

#ifndef _ORK_QUERY_H_
#define _ORK_QUERY_H_

#include "ork/core/Object.h"
#include "ork/render/Types.h"

namespace ork
{

/**
 * An asynchronous GPU query. A query measures some value, depending on its type,
 * between the calls to #begin() and #end(). After #end() has been called, the
 * result is available asynchronously. Its availability can be tested with
 * #available(), and its value with #getResult().
 *
 * @ingroup render
 */
class ORK_API Query : public Object
{
public:
    /**
     * Creates a new Query of the given type.
     *
     * @param type the query type.
     */
    Query(QueryType type);

    /**
     * Deletes this query.
     */
    virtual ~Query();

    /**
     * Returns the type of this query.
     */
    QueryType getType();

    /**
     * Returns the id of this query.
     */
    GLuint getId();

    /**
     * Starts this query.
     */
    void begin();

    /**
     * Ends this query.
     */
    void end();

    /**
     * Returns true if the result of this query is available.
     */
    bool available();

    /**
     * Returns the result of this query. This may block the caller
     * until the result is available.
     */
    GLuint64 getResult();

private:
    /**
     * The type of this query.
     */
    QueryType type;

    /**
     * The OpenGL target for this query.
     */
    GLenum target;

    /**
     * The id of this query.
     */
    GLuint id;

    /**
     * True if the result of this query is available.
     */
    bool resultAvailable;

    /**
     * True if the result of this query had been read.
     */
    bool resultRead;

    /**
     * The result of this query.
     */
    GLuint64 result;
};

}

#endif
