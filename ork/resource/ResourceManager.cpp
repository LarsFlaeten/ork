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

#include "ork/resource/ResourceManager.h"

using namespace std;

namespace ork
{

ResourceManager::ResourceManager(ptr<ResourceLoader> loader, unsigned int cacheSize) :
    Object("ResourceManager"), loader(loader), cacheSize(cacheSize)
{
}

ResourceManager::~ResourceManager()
{
    // since a Resource has a ptr to its manager, the manager cannot be
    // deleted until all the resources it manages are deleted or unused (when
    // a Resource gets released its pointer to its manager is set to NULL)
    // Hence, at this point, all the managed resources should be unused.
    assert(unusedResources.size() == resources.size());
    // we can then safely delete the unused resources
    map<Resource*, list<Resource*>::iterator>::iterator j = unusedResources.begin();
    while (j != unusedResources.end()) {
        delete j->first;
        ++j;
    }
}

ptr<ResourceLoader> ResourceManager::getLoader()
{
    return loader;
}

ptr<Object> ResourceManager::loadResource(const string &name)
{
    map<string, pair<int, Resource*> >::iterator i = resources.find(name);
    if (i != resources.end()) { // if the requested resource has already been loaded
        Resource *r = i->second.second;
        map<Resource*, list<Resource*>::iterator>::iterator j = unusedResources.find(r);
        // and if it is currently unused
        if (j != unusedResources.end()) {
            // we remove it from the cache of unused resources
            unusedResourcesOrder.erase(j->second);
            unusedResources.erase(j);
        }
        // we restore the link from the resource to the manager, which may have
        // been set to null if the resource was unused (see #releaseResource)
        r->manager = this;
        // and we return the resource
        return dynamic_cast<Object*>(r);
    }
    if (Logger::INFO_LOGGER != NULL) {
        Logger::INFO_LOGGER->log("RESOURCE", "Loading resource '" + name + "'");
    }
    // otherwise the resource is not already loaded; we first load its descriptor
    ptr<Object> r = NULL;
    ptr<ResourceDescriptor> d = loader->loadResource(name);

    if (d != NULL) {
        // then we create the actual resource from this descriptor
        try {
            r = ResourceFactory::getInstance()->create(this, name, d).cast<Object>();
        } catch (...) {
        }
        if (r != NULL) {
            // and we register this resource with this manager
            Resource *res = dynamic_cast<Resource*>(r.get());
            resources[name] = make_pair(res->getUpdateOrder(), res);
            resourceOrder[make_pair(res->getUpdateOrder(), res->getName())] = res;
            return r;
        }
    }
    if (Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->log("RESOURCE", "Missing or invalid resource '" + name + "'");
    }
    throw exception();
}

ptr<Object> ResourceManager::loadResource(ptr<ResourceDescriptor> desc, const TiXmlElement *f)
{
    string name;
    const char *nm = f->Attribute("name");
    if (nm == NULL) {
        char n[20];
        sprintf(n, "%s%d",f->Value(), (int)resources.size());
        name = n;
    } else {
        name = nm;
    }
    ptr<Object> r = NULL;

    if (desc != NULL) {
        // then we create the actual resource from this descriptor
        try {
            r = ResourceFactory::getInstance()->create(this, name, desc, f).cast<Object>();
        } catch (...) {
        }
        if (r != NULL) {
            // and we register this resource with this manager
            Resource *res = dynamic_cast<Resource*>(r.get());
            resources[name] = make_pair(res->getUpdateOrder(), res);
            resourceOrder[make_pair(res->getUpdateOrder(), res->getName())] = res;
            return r;
        }
    }
    if (Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->log("RESOURCE", "Missing or invalid resource '" + name + "'");
    }
    throw exception();
}

bool ResourceManager::updateResources()
{
    if (Logger::INFO_LOGGER != NULL) {
        Logger::INFO_LOGGER->log("RESOURCE", "Updating resources");
    }

    // in order to atomically update all resources we use a two phase commit

    // in the first phase we prepare the update of each resource, without doing
    // the actual update. If this preparation succeeds it means that the actual
    // update will succeed. Otherwise, if at least one prepare fails, then no
    // actual update will be performed. Note that resources are handled in a
    // predefined order, so that resources that depend on other resources are
    // updated after their dependent resources (for instance a program resource
    // is updated after its shader resources, itself updated after the texture
    // resources it may depend on, and so on).
    bool commit = true;
    map<pair<int, string>, Resource*>::iterator i = resourceOrder.begin();
    while (i != resourceOrder.end()) {
        commit &= i->second->prepareUpdate();
        ++i;
    }

    // in the second phase we either do all actual updates (and we now that they
    // cannot fail), or we revert all the preparation done in the first step.
    i = resourceOrder.begin();
    while (i != resourceOrder.end()) {
        i->second->doUpdate(commit);
        ++i;
    }

    if (!commit && Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->log("RESOURCE", "Resources update failed");
    }
    if (Logger::INFO_LOGGER != NULL) {
        ostringstream os;
        os << resources.size() << " resources used, " << unusedResources.size() << " unused.";
        Logger::INFO_LOGGER->log("RESOURCE", os.str());
    }
    return commit;
}

void ResourceManager::close()
{
    cacheSize = 0;
}

void ResourceManager::releaseResource(Resource *resource)
{
    if (cacheSize > 0) {
        map<string, pair<int, Resource*> >::iterator i;
        i = resources.find(resource->getName());
        if (i == resources.end() || i->second.second != resource) {
            // if this resource is not managed by this manager, we delete it
            delete resource;
            return;
        }
        // otherwise we put it in the cache of unused resources
        list<Resource*>::iterator j;
        if (unusedResourcesOrder.size() == cacheSize) {
            // before that, if the cache is full, we evict and delete the last
            // recently (un)used resource
            j = unusedResourcesOrder.begin();
            Resource *r = *j;
            unusedResources.erase(r);
            unusedResourcesOrder.erase(j);
            delete r;
        }
        j = unusedResourcesOrder.insert(unusedResourcesOrder.end(), resource);
        unusedResources.insert(make_pair(resource, j));
        // we remove the link from the resource to its manager so that the
        // manager gets deleted when there are no resources in use, even if
        // there are still some unused resources.
        resource->manager = NULL;
    } else {
        // if there is no cache of unused resources, then we delete resources as
        // soon as they become unused
        delete resource;
    }
}

void ResourceManager::removeResource(Resource *resource)
{
    int order = 0;
    // removes this resource from the #resources map
    map<string, pair<int, Resource*> >::iterator i;
    i = resources.find(resource->getName());
    if (i != resources.end() && i->second.second == resource) {
        order = i->second.first;
        resources.erase(i);
    }
    // removes this resource from the #resourceOrder map
    map<pair<int, string>, Resource*>::iterator j;
    j = resourceOrder.find(make_pair(order, resource->getName()));
    if (j != resourceOrder.end() && j->second == resource) {
        resourceOrder.erase(j);
    }
    // it is not necessary to remove the resource from the unused resource cache
    // indeed this should have been done already (see #releaseResource)
}

}
