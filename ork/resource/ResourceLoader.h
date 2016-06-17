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

#ifndef _ORK_RESOURCE_LOADER_H_
#define _ORK_RESOURCE_LOADER_H_

#include "ork/resource/ResourceDescriptor.h"

namespace ork
{

/**
 * An abstract %resource loader, loads ResourceDescriptor from disk or other
 * locations.
 *
 * @ingroup resource
 */
class ORK_API ResourceLoader : public Object
{
public:
    /**
     * Creates a new %resource loader.
     */
    ResourceLoader();

    /**
     * Deletes this %resource loader.
     */
    virtual ~ResourceLoader();

    /**
     * Returns the path of the resource of the given name.
     *
     * @param name the name of a resource.
     * @return the path of this resource.
     * @throw exception if the resource is not found.
     */
    virtual string findResource(const string &name) = 0;

    /**
     * Loads the ResourceDescriptor of the given name.
     *
     * @param name the name of the ResourceDescriptor to be loaded.
     * @return the ResourceDescriptor of the given name, or NULL if the %resource
     *      is not found.
     */
    virtual ptr<ResourceDescriptor> loadResource(const string &name) = 0;

    /**
     * Reloads the ResourceDescriptor of the given name.
     *
     * @param name the name of the ResourceDescriptor to be loaded.
     * @param currentValue the current value of this ResourceDescriptor.
     * @return the new value of this ResourceDescriptor, or NULL if this value
     *      has not changed.
     */
    virtual ptr<ResourceDescriptor> reloadResource(const string &name, ptr<ResourceDescriptor> currentValue) = 0;
};

}

#endif
