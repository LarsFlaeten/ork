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

#include "ork/resource/CompiledResourceLoader.h"

#include <fstream>

using namespace std;

namespace ork
{

CompiledResourceLoader::StaticResourceDescriptor::StaticResourceDescriptor(const TiXmlElement *descriptor, unsigned char *data, unsigned int size) :
    ResourceDescriptor(descriptor, data, size)
{
}

CompiledResourceLoader::StaticResourceDescriptor::~StaticResourceDescriptor()
{
}

void CompiledResourceLoader::StaticResourceDescriptor::clearData()
{
}

CompiledResourceLoader::CompiledResourceLoader(const string &resourceDataFile) : ResourceLoader()
{
    ifstream fs(resourceDataFile.c_str(), ios::binary);
    fs.seekg(0, ios::end);
    unsigned int size = fs.tellg();
    data = new unsigned char[size];
    fs.seekg(0);
    fs.read((char*) data, size);
    fs.close();
}

CompiledResourceLoader::~CompiledResourceLoader()
{
    delete[] data;
}

string CompiledResourceLoader::findResource(const string &name)
{
    return paths[name];
}

ptr<ResourceDescriptor> CompiledResourceLoader::loadResource(const string &name)
{
    return resources[name];
}

ptr<ResourceDescriptor> CompiledResourceLoader::reloadResource(const string &name, ptr<ResourceDescriptor> currentValue)
{
    return NULL;
}

void CompiledResourceLoader::addPath(const string &name, const string &path)
{
    paths.insert(make_pair(name, path));
}

void CompiledResourceLoader::addResource(const string &name, ptr<ResourceDescriptor> desc)
{
    resources.insert(make_pair(name, desc));
}

}
