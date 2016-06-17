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

#ifndef _ORK_RESOURCE_FACTORY_H_
#define _ORK_RESOURCE_FACTORY_H_

#include <exception>
#include <map>

#include "ork/resource/Resource.h"

namespace ork
{

/**
 * A Resource factory, creates resources from ResourceDescriptor. There is only
 * one instance of this class, which registers a creation function for each
 * %resource type name.
 *
 * @ingroup resource
 */
class ORK_API ResourceFactory
{
public:
    /**
     * A function that creates a Resource from a ResourceDescriptor.
     *
     * @param manager the manager that will manage the created %resource.
     * @param name the %resource name.
     * @param desc the %resource descriptor.
     * @param e an optional XML element providing contextual information (such
     *      as the XML element in which the %resource descriptor was found).
     * @return the created %resource.
     */
    typedef ptr<Object> (*createFunc) (ptr<ResourceManager> manager, const std::string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e);

    /**
     * Returns the unique instance of this class.
     */
    static ResourceFactory *getInstance();

    /**
     * Registers a new %resource type with this factory.
     *
     * @param type a %resource type, as it appears in the tag of a
     *      ResourceDescriptor (e.g. texture1D, texture2D, shader, program, etc).
     * @param f a function that can create %resources of this type.
     */
    void addType(const std::string &type, createFunc f);

    /**
     * Creates a Resource from the given ResourceDescriptor.
     *
     * @param manager the manager that will manage the created %resource.
     * @param name the %resoure name.
     * @param desc the %resource descriptor.
     * @param e an optional XML element providing contextual information (such
     *      as the XML element in which the %resource descriptor was found).
     * @return the created %resource.
     */
    ptr<Object> create(ptr<ResourceManager> manager, const std::string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL);

    /**
     * Utility function to dump all the types of resources
     * Used for debugging
     * TODO: remove?
     * */
    
    void dumpTypes();

    /**
     * A utility template to automate the registration of new %resource types.
     * @tparam t a %resource type (e.g. texture1D, shader, mesh, etc).
     * @tparam T the corresponding concrete Resource class. This class must be
     *      instantiated for all resources of type t.
     */
    template <const char* t, class T>
    class Type
    {
    public:
        /**
         * Creation function for resources of class T. This function
         * just calls new T(manager, name, desc, e).
         * See ResourceFactory::createFunc
         */
        static ptr<Object> ctor(ptr<ResourceManager> manager, const std::string &name,
                ptr<ResourceDescriptor> desc, const TiXmlElement *e)
        {
            return new T(manager, name, desc, e);
        }

        /**
         * Creates a new Type instance. This constructor registers the creation
         * function #ctor (encapsulating a call to new T) with the %resource
         * type t in the ResourceFactory instance. Hence declaring a static
         * variable of type Type<t, T> automatically registers a new %resource
         * type.
         */
        Type()
        {
            ResourceFactory::getInstance()->addType(std::string(t), ctor);
        }
    };

private:
    /**
     * The registered creation functions. Maps %resource types (such as shader,
     * program, mesh, etc) to %resource creation functions.
     */
    std::map<std::string, createFunc> types;

    /**
     * The unique instance of this class.
     */
    static ResourceFactory *INSTANCE;
};

}

#endif
