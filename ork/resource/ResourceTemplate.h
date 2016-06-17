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

#ifndef _ORK_RESOURCE_TEMPLATE_H_
#define _ORK_RESOURCE_TEMPLATE_H_

#include "ork/resource/ResourceFactory.h"
#include "ork/resource/ResourceManager.h"

namespace ork
{

/**
 * A template Resource class to ease the implementation of concrete Resource
 * subclasses. This template class takes care of the two phase commit for the
 * update of resources, provided that the actual %resource class implements a
 * swap(ptr<C>) method that can swap two instances of resources of class C.
 *
 * @ingroup resource
 *
 * @tparam o the update order of resources of class C (see #getUpdateOrder).
 * @tparam C the concrete %resource class.
 */
template<int o, class C>
class ResourceTemplate :  public C, public Resource
{
public:
    /**
     * Creates a new %resource of class C.
     *
     * @param manager the manager that will keep track of this %resource.
     * @param name the name of this %resource.
     * @param desc the descriptor of this %resource.
     */
    ResourceTemplate(ptr<ResourceManager> manager,
            const std::string &name, ptr<ResourceDescriptor> desc);

    /**
     * Returns the template parameter o.
     */
    virtual int getUpdateOrder();

    /**
     * If the descriptor of this method has not changed, does nothing and
     * returns true. Otherwise creates a new instance of this %resource using the
     * new descriptor #newDesc and then swaps it with this instance, saving the
     * current value in #oldValue. If the %resource creation fails, does nothing
     * and returns false.
     */
    virtual bool prepareUpdate();

    /**
     * If commit is true swaps the #desc and #newDesc fields, and sets the
     * #oldValue to NULL. If commit is false reverts the changes made in
     * #prepareUpdate by swapping again this instance with #oldValue.
     */
    virtual void doUpdate(bool commit);

    /**
     * Returns true if this %resource has changed.
     */
    virtual bool changed();

protected:
    /**
     * The old value of this %resource.
     */
    ptr<C> oldValue;

    /**
     * Calls ResourceManager#releaseResource to release this %resource.
     */
    virtual void doRelease();
};

template<int o, class C>
ResourceTemplate<o, C>::ResourceTemplate(ptr<ResourceManager> manager, const std::string &name, ptr<ResourceDescriptor> desc) :
    C(), Resource(manager, name, desc)
{
}

template<int o, class C>
int ResourceTemplate<o, C>::getUpdateOrder()
{
    return o;
}

template<int o, class C>
bool ResourceTemplate<o, C>::prepareUpdate()
{
    if (Resource::prepareUpdate()) { // if the descriptor has changed
        oldValue = NULL;
        try {
            // creates a new resource using this new descriptor
            oldValue = ResourceFactory::getInstance()->create(Resource::manager, Resource::name, newDesc).template cast<C>();
        } catch (std::exception& e) {
            Logger::ERROR_LOGGER->log("RESSOURCE", std::string(e.what()));
        } catch (...) {
            // dont't do that, throw a regular exception please
            assert(false);
        }
        if (oldValue != NULL) { // if the creation is a success
            this->swap(oldValue); // swaps the current value with the new one
            return true;
        }
        return false; // if the creation fails, do nothing
    }
    return true; // nothing to do if the descriptor has not changed
}

template<int o, class C>
void ResourceTemplate<o, C>::doUpdate(bool commit)
{
    if (commit) { // if we must commit changes
        if (newDesc != NULL) { // and if there was some changes
            desc = newDesc; // we set the descriptor to its new value
            if (Logger::INFO_LOGGER != NULL) {
                log(Logger::INFO_LOGGER, desc, NULL, "Resource '" + Resource::getName() + "' updated");
            }
            // nothing to do for the actual resource,
            // this has already been done in prepareUpdate with the swap
        }
    } else { // if we must abort changes
        if (oldValue != NULL) { // and if there was some changes
            this->swap(oldValue); // we revert them with a swap to cancel the first one
        }
    }
    // in all cases we set oldValue and newDesc to NULL to release memory
    oldValue = NULL;
    newDesc = NULL;
}

template<int o, class C>
bool ResourceTemplate<o, C>::changed()
{
    return oldValue != NULL;
}

template<int o, class C>
void ResourceTemplate<o, C>::doRelease()
{
    if (Resource::manager != NULL) {
        Resource::manager->releaseResource(this);
    } else {
        delete this;
    }
}

}

#endif
