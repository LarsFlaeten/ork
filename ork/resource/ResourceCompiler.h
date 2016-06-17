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

#ifndef _ORK_RESOURCE_COMPILER_H_
#define _ORK_RESOURCE_COMPILER_H_

#include <fstream>

#include "ork/resource/XMLResourceLoader.h"

namespace ork
{

/**
 * An XMLResourceLoader that produces compiled resources for a
 * CompiledResourceLoader. This class concatenates and stores the
 * resources it loads into two files. The first file
 * contains source code that builds the XML descriptors of the resources.
 * The second file contains the %resource data (shader source code, texture
 * data, mesh data, etc). The first file can be included in the source code
 * of a CompiledResourceLoader subclass, and the second file can be passed
 * as argument to the constructor of this subclass:
\code
class MyResourceLoader : public CompiledResourceLoader
{
public:
    MyResourceLoader(const string &resourceDataFile) :
        CompiledResourceLoader(resourceDataFile)
    {
#include "resourceFile"
    }
};
\endcode
 *
 * @ingroup resource
 */
class ResourceCompiler : public XMLResourceLoader
{
public:
    /**
     * Creates a new ResourceCompiler.
     *
     * @param resourceFile the file that will contain the source code to build
     *      the XML descriptors of the loaded resources.
     * @param resourceDataFile the file that will contain the data of the
     *      loaded resources.
     */
    ResourceCompiler(const string &resourceFile, const string &resourceDataFile);

    /**
     * Deletes this ResourceCompiler.
     */
    virtual ~ResourceCompiler();

    /**
     * Returns the path of the resource of the given name.
     *
     * @param name the name of a resource.
     * @return the path of this resource.
     * @throw exception if the resource is not found.
     */
    virtual string findResource(const string &name);

    /**
     * Loads the ResourceDescriptor of the given name.
     *
     * @param name the name of the ResourceDescriptor to be loaded.
     * @return the ResourceDescriptor of the given name, or NULL if the %resource
     *      is not found.
     */
    virtual ptr<ResourceDescriptor> loadResource(const string &name);

private:
    /**
     * The stream to write into the resourceFile file.
     */
    ofstream out;

    /**
     * The stream to write into the resourceDataFile file.
     */
    ofstream dout;

    /**
     * The number of bytes currently written into dout.
     */
    unsigned int offset;
};

}

#endif
