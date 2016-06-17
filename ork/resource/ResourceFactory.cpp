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

#include "ork/resource/ResourceFactory.h"

#include "ork/resource/ResourceManager.h"

namespace ork
{

ResourceFactory *ResourceFactory::INSTANCE = NULL;

ResourceFactory *ResourceFactory::getInstance()
{
    if (INSTANCE == NULL) {
        INSTANCE = new ResourceFactory();
    }
    return INSTANCE;
}

void ResourceFactory::addType(const string &type, createFunc f)
{
    types[type] = f;
}

ptr<Object> ResourceFactory::create(ptr<ResourceManager> manager, const string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e)
{
    e = e == NULL ? desc->descriptor : e;
    map<string, createFunc>::iterator i = types.find(e->ValueStr());
    if (i != types.end()) {
        return i->second(manager, name, desc, e);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Unknown resource type '" + e->ValueStr() + "'");
        }
        throw exception();
    }
}

}
