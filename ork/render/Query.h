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
