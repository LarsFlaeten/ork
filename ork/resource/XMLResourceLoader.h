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

#ifndef _ORK_XML_RESOURCE_LOADER_H_
#define _ORK_XML_RESOURCE_LOADER_H_

#include <map>
#include <vector>
#include "ork/resource/ResourceLoader.h"

namespace ork
{

/**
 * A ResourceLoader that loads ResourceDescriptor from XML files. This loader
 * can load resources from individual XML files, each file containing a single
 * ResourceDescriptor, and from so called XML archive files, each archive file
 * containing one or more ResourceDescriptor. This loader can search for these
 * files in several directories and archives files configured with #addPath and
 * #addArchive.
 *
 * @ingroup resource
 */
class ORK_API XMLResourceLoader : public ResourceLoader
{
public:
    /**
     * Creates a new XMLResourceLoader.
     */
    XMLResourceLoader();

    /**
     * Deletes this XMLResourceLoader.
     */
    virtual ~XMLResourceLoader();

    /**
     * Adds a search path where ResourceDescriptor can be looked for.
     */
    void addPath(const string &path);

    /**
     * Adds an XML archive file where ResourceDescriptor can be looked for.
     */
    void addArchive(const string &archive);

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

    /**
     * Reloads the ResourceDescriptor of the given name.
     *
     * @param name the name of the ResourceDescriptor to be loaded.
     * @param currentValue the current value of this ResourceDescriptor.
     * @return the new value of this ResourceDescriptor, or NULL if this value
     *      has not changed on disk.
     */
    virtual ptr<ResourceDescriptor> reloadResource(const string &name, ptr<ResourceDescriptor> currentValue);

private:
    /**
     * The directories where individual ResourceDescriptor files can be looked for.
     */
    vector<string> paths;

    /**
     * The archives where other ResourceDescriptor files can be looked for.
     */
    vector<string> archives;

    /**
     * A cache of the archive files. Maps archive file names to archive content
     * and last modification time on disk.
     */
    map<string, pair<TiXmlDocument*, time_t> > cache;

    /**
     * Returns the XML part of the ResourceDescriptor of the given name. This
     * method looks for this descriptor in the archive files and then, if not
     * found, in the directories specified with #addPath.
     *
     * @param name the name of a ResourceDescriptor.
     * @param[in,out] t the last modification time of this %resource descriptor,
     *      or 0 if it has not been loaded yet. This modification time is
     *      updated by this method if it has changed.
     * @param log true to log an error message if the descriptor is not found.
     * @return the XML part of the ResourceDescriptor of the given name, of NULL
     *      if the last modification time is still equal to t or if the %resource
     *      is not found.
     */
    TiXmlElement *findDescriptor(const string &name, time_t &t, bool log = true);

    /**
     * Returns the XML part of the ResourceDescriptor of the given name. This
     * method looks for this descriptor in the given archive file.
     *
     * @param archive the archive file where the descriptor must be looked for.
     * @param name the name of a ResourceDescriptor.
     * @return the XML part of the ResourceDescriptor, or NULL if the archive
     *      file does not contain this %resource descriptor.
     */
    static TiXmlElement *findDescriptor(const TiXmlDocument *archive, const string &name);

    /**
     * Builds the XML part of texture %resource descriptors for the special textures
     * 'renderbuffer-X-Y'. The XML part is generated from the %resource name.
     */
    static TiXmlElement *buildTextureDescriptor(const string &name);

    /**
     * Builds the XML part of program %resource descriptors. The XML part is
     * generated from the %resource name of the form "shader1;shader2;shader3;...".
     */
    static TiXmlElement *buildProgramDescriptor(const string &name);

    /**
     * Loads the archive file of the given name.
     *
     * @param name the name of the archive file to be loaded.
     * @param[out] t returns the last modification time of this file on disk.
     * @return the archive file of the given name, or NULL if this file is not
     *      found.
     */
    TiXmlDocument *loadArchive(const string &name, time_t &t);

    /**
     * Loads the ASCII or binary part of a ResourceDescriptor.
     *
     * @param e the XML part of a ResourceDescriptor.
     * @param[out] size returns the size of ASCII or binary part.
     * @param[in,out] stamps the last modification time of the file(s) that
     *      contain this ASCII or binary part, or an empty vector if this ASCII
     *      or binary part has not been loaded yet. These modification times are
     *      updated by this method if they have changed. Each element of this
     *      vector contains a file name and its last modification time.
     * @return the ASCII or binary part of the given ResourceDescriptor, or NULL
     *      if this %resource has no binary part, if this part is not found, or
     *      if the last modification times are still equal to the given
     *      modification times.
     */
    unsigned char* loadData(TiXmlElement *e, unsigned int &size, vector< pair<string, time_t> > &stamps);
};

}

#endif
