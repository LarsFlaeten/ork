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

#ifndef _ORK_RESOURCE_DESCRIPTOR_H_
#define _ORK_RESOURCE_DESCRIPTOR_H_

#include "ork/core/Object.h"

#include "tinyxml/tinyxml.h"

namespace ork
{

/**
 * A %resource descriptor, contains all the data to create an actual Resource.
 * This data is described with an XML element and with an optional ASCII or
 * binary data section. For example, for a texture, the XML part describes the
 * texture options (internal format, min and mag filter, min and max LOD, etc),
 * while the binary data part contains the texture data itself. For a shader
 * the XML part describes default values for the shader uniforms, while the
 * binary data part contains the shader source code. And so on for other
 * resources.
 *
 * @ingroup resource
 */
class ORK_API ResourceDescriptor : public Object
{
public:
    /**
     * The XML part of this %resource descriptor. This part can describe
     * optional elements that cannot be stored in the %resource itself, such as
     * the internal format for a texture, default uniform values for a shader,
     * etc. The tag of the descriptor is the type of the %resource (e.g.
     * texture1D, texture2D, shader, program, mesh, etc).
     */
    const TiXmlElement *descriptor;

    /**
     * Creates a new %resource descriptor.
     *
     * @param descriptor the XML part of this %resource descriptor.
     * @param data the ASCII of binary data part of the descriptor.
     * @param size the size of the ASCII or binary part in bytes.
     */
    ResourceDescriptor(const TiXmlElement *descriptor, unsigned char *data, unsigned int size);

    /**
     * Deletes this %resource descriptor. This deletes both the XML and the
     * binary data part.
     */
    virtual ~ResourceDescriptor();

    /**
     * Returns the ASCII or binary data part of this %resource descriptor.
     */
    unsigned char *getData() const;

    /**
     * Returns the size in bytes of the ASCII or binary data part of this
     * %resource descriptor.
     */
    unsigned int getSize() const;

    /**
     * Deletes the ASCII or binary data part of this %resource descriptor.
     */
    virtual void clearData();

private:
    /**
     * The ASCII or binary data part of this %resource descriptor.
     */
    unsigned char* data;

    /**
     * Size in bytes of the ASCII or binary data part of this %resource descriptor.
     */
    unsigned int size;
};

}

#endif
