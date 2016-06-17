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

#ifndef _ORK_RESOURCE_MANAGER_H_
#define _ORK_RESOURCE_MANAGER_H_

#include <map>
#include <list>
#include "ork/resource/ResourceLoader.h"
#include "ork/resource/ResourceFactory.h"

namespace ork
{

/**
 * A %resource manager, loads, unloads and updates a set of resources. A manager
 * uses a ResourceLoader to load ResourceDescriptor, then uses a ResourceFactory
 * to create actual Resource. A manager keeps track of the resources it has
 * loaded: it can update (i.e. reload) them when their descriptors change, and
 * it automatically deletes them when they are unused (i.e. unreferenced).
 * Alternatively a manager can cache unused resources so that they can be loaded
 * quickly if they are needed again.
 *
 * @ingroup resource
 */
class ORK_API ResourceManager : public Object
{
public:
    /**
     * Creates a new ResourceManager.
     *
     * @param loader the object used to load the ResourceDescriptor.
     * @param cacheSize the size of the cache of unused resources.
     */
    ResourceManager(ptr<ResourceLoader> loader, unsigned int cacheSize = 0);

    /**
     * Deletes this %resource manager. This deletes the cached unused resources,
     * if any.
     */
    virtual ~ResourceManager();

    /**
     * Returns the object used to load the ResourceDescriptor.
     */
    ptr<ResourceLoader> getLoader();

    /**
     * Loads the given %resource. This method first loads its descriptor with
     * #getLoader, then creates the %resource with ResourceFactory, and finally
     * returns the result. Unless the %resource has already been loaded, in which
     * case it is returned directly.
     *
     * @param name the name of the %resource to be loaded.
     * @return the %resource corresponding to the given name, or NULL if the %resource is not
     *      found.
     */
    ptr<Object> loadResource(const std::string &name);

    /**
     * Loads the given %resource. This method first loads its descriptor with
     * #getLoader, then creates the %resource with ResourceFactory, and finally
     * returns the result. Unless the %resource has already been loaded, in which
     * case it is returned directly.
     *
     * @param desc descriptor of the resource to load.
     * @param f the XML part of a ResourceDescriptor.
     * @return the %resource corresponding to the given name, or NULL if the %resource is not
     *      found.
     */
    ptr<Object> loadResource(ptr<ResourceDescriptor> desc, const TiXmlElement *f);

    /**
     * Updates the already loaded resources if their descriptors have changed.
     * This update is atomic, i.e. either all resources are updated, or none are
     * updated.
     *
     * @return true if the resources have been updated successfully.
     */
    bool updateResources();

    /**
     * Closes this manager. This method disables the cache of unused resources.
     */
    void close();

protected:
    /**
     * Releases an unused %resource. If there is a cache of unused resources
     * then this %resource is put in this cache (the oldest %resource in the
     * cache is evicted if the cache is full). Otherwise if there is no cache,
     * the %resource is deleted directly.
     *
     * @param resource an unused %resource, i.e. an unreferenced %resource.
     */
    void releaseResource(Resource *resource);

    /**
     * Removes a %resource from this manager. This method is called from the
     * Resource destructor when a %resource gets deleted (for example when a
     * %resource is deleted in the #releaseResource method).
     *
     * @param resource a %resource which is currently being deleted.
     */
    void removeResource(Resource *resource);

    template<int o, class C> friend class ResourceTemplate;

    friend class Resource;

private:
    /**
     * The object used to load the ResourceDescriptor.
     */
    ptr<ResourceLoader> loader;

    /**
     * The resources currently managed by this manager. This map contains both
     * the resources currently in use and the unused resources. It maps %resource
     * names to %resource instances (together with their update order - see
     * Resource#getUpdateOrder).
     */
    std::map<std::string, std::pair<int, Resource*> > resources;

    /**
     * The resources currently managed by this manager. This map contains both
     * the resources currently in use and the unused resources. It maps %resource
     * names (together with their update order - see Resource#getUpdateOrder) to
     * %resource instances.
     */
    std::map<std::pair<int, std::string>, Resource*> resourceOrder;

    /**
     * The cache of unused resources. This map maps %resource instances to
     * positions in the sorted list of unused resources #unusedResourcesOrder.
     */
    std::map<Resource*, std::list<Resource*>::iterator> unusedResources;

    /**
     * The unused resources, sorted by date of last use. This list is used to
     * implement a LRU cache.
     */
    std::list<Resource*> unusedResourcesOrder;

    /**
     * The maximum number of unused resources that can be stored in cache.
     */
    unsigned int cacheSize;
};

}

#endif
