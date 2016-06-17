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

#ifndef _ORK_RESOURCE_H_
#define _ORK_RESOURCE_H_

#include "ork/core/Logger.h"
#include "ork/resource/ResourceDescriptor.h"

namespace ork
{

/**
 * @defgroup resource resource
 * @ingroup ork
 * Provides a framework to load resources (textures, shaders, etc) from disk.
 * This framework provides automatic %resource updates when resources are
 * modified on disk. This is particularly useful for shaders. Indeed you don't
 * need to restart the application to see the effect of a modification in a
 * shader (and likewise for textures and other %resource types).
 */

class ResourceManager;

/**
 * An abstract %resource (texture, shader, mesh, etc).
 * @ingroup resource
 */
class ORK_API Resource
{
public:
    /**
     * Creates a new Resource.
     *
     * @param manager the manager that will keep track of this %resource.
     * @param name the name of this %resource.
     * @param desc the descriptor of this %resource.
     */
    Resource(ptr<ResourceManager> manager,
            const string &name, ptr<ResourceDescriptor> desc);

    /**
     * Deletes this %resource.
     */
    virtual ~Resource();

    /**
     * Returns the name of this %resource.
     */
    virtual string getName();

    /**
     * Returns the update order of this %resource. In order to be updated
     * correctly a %resource must be updated after the %resource it depends on are
     * updated. For instance a program must be updated after the shaders it is
     * made of, which must themselves be updated after the textures they may
     * depend on, and so on. This order is computed by sorting the resources
     * according to their "update order" (0 means "update first").
     */
    virtual int getUpdateOrder() = 0;

    /**
     * Prepares the update of this %resource. In order to update all resources
     * atomically (see ResourceManager#updateResources) a two phase commit is
     * used. In the first phase all resources test if they can be updated
     * successfully or not. If all resources can do so then they are all updated,
     * otherwise none is updated. This method tests is a %resource can be updated
     * successfully. Returning true means that it is guaranteed that the actual
     * update will not fail. NOTE: the default implementation of this method
     * does not follow this contract. Indeed it returns true if the descriptor
     * of this %resource has changed. This method MUST therefore be overriden in
     * subclasses of this class.
     *
     * @return true if it is guaranteed that a call #doUpdate with a true
     *      parameter will not fail.
     */
    virtual bool prepareUpdate();

    /**
     * Do an actual update of this %resource, or reverts the work of
     * #prepareUpdate.
     *
     * @param commit true to do the actual update, or false to revert the work
     *      #prepareUpdate.
     */
    virtual void doUpdate(bool commit) = 0;

    /**
     * Returns true if the descriptor of this %resource has changed.
     */
    virtual bool changed();

    /**
     * Utility method to check the attributes of an XML element.
     *
     * @param desc a %resource descriptor.
     * @param e an element of the XML part of the %resource descriptor desc.
     * @param params the authorized XML attributes for e, separated by commas.
     * @throw exception if e has an attribute which is not in the list specified
     *      by params.
     */
    static void checkParameters(const ptr<ResourceDescriptor> desc,
            const TiXmlElement *e, const string &params);

    /**
     * Utility method to get the int value of an XML element attribute.
     *
     * @param desc a %resource descriptor.
     * @param e an element of the XML part of the %resource descriptor desc.
     * @param name the attribute whose value must be returned.
     * @param[out] i the value of the requested attribute.
     * @throw exception if the attribute is missing or has a wrong format.
     */
    static void getIntParameter(const ptr<ResourceDescriptor> desc,
            const TiXmlElement *e, const string &name, int *i);

    /**
     * Utility method to get the float value of an XML element attribute.
     *
     * @param desc a %resource descriptor.
     * @param e an element of the XML part of the %resource descriptor desc.
     * @param name the attribute whose value must be returned.
     * @param[out] value the value of the requested attribute.
     * @throw exception if the attribute has a wrong format.
     * @return false is the attribute is missing.
     */
    static bool getFloatParameter(const ptr<ResourceDescriptor> desc,
            const TiXmlElement *e, const string &name, float *value);

    /**
     * Utility method to get the value of an XML element attribute.
     *
     * @param desc a %resource descriptor.
     * @param e an element of the XML part of the %resource descriptor desc.
     * @param name the attribute whose value must be returned.
     * @throw exception if the attribute is missing.
     * @return the attribute's value.
     */
    static string getParameter(ptr<ResourceDescriptor> desc,
            const TiXmlElement *e, const char* name);

    /**
     * Logs a message related to a %resource.
     *
     * @param logger the logger to be used to log the message.
     * @param desc the descriptor of the %resource.
     * @param e an optional element of the XML part of the %resource descriptor.
     * @param msg the message to be logged.
     */
    static void log(ptr<Logger> logger, ptr<ResourceDescriptor> desc,
            const TiXmlElement *e, const string &msg);

    /**
     * Logs a message related to a %resource.
     *
     * @param logger the logger to be used to log the message.
     * @param desc the XML part of the descriptor of the %resource.
     * @param e an optional element of the XML part of the %resource descriptor.
     * @param msg the message to be logged.
     */
    static void log(ptr<Logger> logger, const TiXmlElement *desc,
            const TiXmlElement *e, const string &msg);

protected:
    /**
     * The manager that keeps track of this %resource. May be NULL for an unused
     * %resource (see ResourceManager#releaseResource).
     */
    ptr<ResourceManager> manager;

    /**
     * The name of this %resource.
     */
    const string name;

    /**
     * The descriptor of this %resource.
     */
    ptr<ResourceDescriptor> desc;

    /**
     * The new value of the descriptor of this %resource. This field is set in
     * the #prepareUpdate method. If it is not NULL it means that the descriptor
     * has changed, and the value is stored in this field. This field is set
     * back to NULL in #doUpdate.
     */
    ptr<ResourceDescriptor> newDesc;

    friend class ResourceManager;
};

}

#endif
