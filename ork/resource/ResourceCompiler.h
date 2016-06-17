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
    MyResourceLoader(const std::string &resourceDataFile) :
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
    ResourceCompiler(const std::string &resourceFile, const std::string &resourceDataFile);

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
    virtual std::string findResource(const std::string &name);

    /**
     * Loads the ResourceDescriptor of the given name.
     *
     * @param name the name of the ResourceDescriptor to be loaded.
     * @return the ResourceDescriptor of the given name, or NULL if the %resource
     *      is not found.
     */
    virtual ptr<ResourceDescriptor> loadResource(const std::string &name);

private:
    /**
     * The stream to write into the resourceFile file.
     */
    std::ofstream out;

    /**
     * The stream to write into the resourceDataFile file.
     */
    std::ofstream dout;

    /**
     * The number of bytes currently written into dout.
     */
    unsigned int offset;
};

}

#endif
