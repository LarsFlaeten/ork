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

#include "ork/resource/ResourceDescriptor.h"

namespace ork
{

ResourceDescriptor::ResourceDescriptor(const TiXmlElement *descriptor, unsigned char *data, unsigned int size) :
    Object("ResourceDescriptor"), data(data), size(size)
{
    this->descriptor = descriptor;
    assert(this->descriptor != NULL);
}

ResourceDescriptor::~ResourceDescriptor()
{
    delete descriptor;
    clearData();
}

unsigned char *ResourceDescriptor::getData() const
{
    return data;
}

unsigned int ResourceDescriptor::getSize() const
{
    return size;
}

void ResourceDescriptor::clearData()
{
    if (data != NULL) {
        delete[] data;
        data = NULL;
    }
}

}
