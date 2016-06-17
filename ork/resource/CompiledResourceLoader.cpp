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

#include "ork/resource/CompiledResourceLoader.h"

#include <fstream>

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
