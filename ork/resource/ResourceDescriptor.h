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
