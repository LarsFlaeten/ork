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

#include "ork/resource/XMLResourceLoader.h"

#include <string.h>
#include <fstream>
#include <ctime>

#ifdef _MSC_VER
#include <time.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#endif

#include "stbi/stb_image.h"

#include "ork/resource/ResourceManager.h"

using namespace std;

namespace ork
{

/**
 * Returns true if the given file name extension corresponds to an image file.
 *
 * @param a file name.
 * @return true if the file name extension corresponds to an image file.
 */
static bool isTextureFile(const string &name)
{
    int n = name.size() - 4;
    if (n > 0) {
        const char *ext = name.substr(n).c_str();
        if (strcmp(".jpg", ext) == 0 ||
            strcmp(".png", ext) == 0 ||
            strcmp(".bmp", ext) == 0 ||
            strcmp(".tga", ext) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * A ResourceDescriptor that also stores a set of last modification times.
 */
class XMLResourceDescriptor : public ResourceDescriptor
{
public:
    typedef vector< pair<string, time_t> > Stamps;

    /**
     * Creates a new XMLResourceDescriptor.
     *
     * @param descriptor the XML part of this descriptor.
     * @param data the ASCII or binary part of this descriptor.
     * @param size the size in bytes of the ASCII or binary part.
     * @param stamp the last modification time of the file that contain the XML
     *      part.
     * @param stamps the last modification time(s) of the file(s) that contain
     *      the ASCII or binary part.
     */
    XMLResourceDescriptor(const TiXmlElement *descriptor, unsigned char *data, unsigned int size,
            time_t stamp, const Stamps &dataStamps) :
        ResourceDescriptor(descriptor, data, size), stamp(stamp), dataStamps(dataStamps)
    {
    }

    /**
     * Deletes this XMLResourceDescriptor.
     */
    virtual ~XMLResourceDescriptor()
    {
    }

    /**
     * Returns true if this descriptor is equal to the given one.
     *
     * @param desc the XML part of another XMLResourceDescriptor.
     * @param stamp the stamp of another XMLResourceDescriptor.
     * @param dataStamps the dataStamps of another XMLResourceDescriptor.
     * @return true if the XML parts of both XMLResourceDescriptor are equal and
     *      if the modification times of the binary parts are equal.
     */
    bool equal(const TiXmlElement *desc, time_t stamp, const Stamps &dataStamps)
    {
        if (dataStamps.size() != this->dataStamps.size()) {
            return false;
        }
        for (unsigned int i = 0; i < dataStamps.size(); ++i) {
            if (difftime(dataStamps[i].second, this->dataStamps[i].second) != 0) {
                return false;
            }
        }
        if (difftime(stamp, this->stamp) == 0) {
            return true;
        }
        return equal(desc, this->descriptor);
    }

    /**
     * Returns true if the given XML elements are equal. This means that they
     * have the same attibutes and the same sub elements.
     */
    static bool equal(const TiXmlNode *n1, const TiXmlNode *n2)
    {
        if (strcmp(n1->Value(), n2->Value()) != 0) {
            return false;
        }
        const TiXmlElement *e1 = n1->ToElement();
        const TiXmlElement *e2 = n2->ToElement();
        if ((e1 == NULL) != (e2 == NULL)) {
            return false;
        }
        if (e1 != NULL) {
            const TiXmlAttribute *a1 = e1->FirstAttribute();
            const TiXmlAttribute *a2 = e2->FirstAttribute();
            while (a1 != NULL) {
                if (a2 == NULL) {
                    return false;
                }
                if (strcmp(a1->Name(), a2->Name()) != 0) {
                    return false;
                }
                if (strcmp(a1->Value(), a2->Value()) != 0) {
                    return false;
                }
                a1 = a1->Next();
                a2 = a2->Next();
            }
            if (a2 != NULL) {
                return false;
            }
        }
        const TiXmlNode *c1 = n1->FirstChild();
        const TiXmlNode *c2 = n2->FirstChild();
        while (c1 != NULL) {
            if (c2 == NULL) {
                return false;
            }
            if (!equal(c1, c2)) {
                return false;
            }
            c1 = c1->NextSibling();
            c2 = c2->NextSibling();
        }
        return c2 == NULL;
    }

private:
    /**
     * The last modification time of the file that contains the XML part of this
     * %resource descriptor.
     */
    time_t stamp;

    /**
     * The last modification time(s) of the file(s) that contain the ASCII or
     * binary part of this %resource descriptor. Each element of this vector
     * contains a file name and its last modification time.
     */
    Stamps dataStamps;

    friend class XMLResourceLoader;
};

XMLResourceLoader::XMLResourceLoader() : ResourceLoader()
{
}

XMLResourceLoader::~XMLResourceLoader()
{
    for (map<string, pair<TiXmlDocument*, time_t> >::iterator i = cache.begin(); i != cache.end(); ++i) {
        delete i->second.first;
    }
    cache.clear();
}

void XMLResourceLoader::addPath(const string &path)
{
    paths.push_back(path);
}

void XMLResourceLoader::addArchive(const string &archive)
{
    archives.push_back(archive);
}

string XMLResourceLoader::findResource(const string &name)
{
    TiXmlElement desc(name);
    return findFile(&desc, paths, name);
}

ptr<ResourceDescriptor> XMLResourceLoader::loadResource(const string &name)
{
    time_t stamp = 0;
    TiXmlElement *desc = NULL;
    if (strncmp(name.c_str(), "renderbuffer", 12) == 0) {
        // resource names of the form "renderbuffer-X-Y" describe texture
        // resources that are not described by any file, either for the XML part
        // or for the binary part. The XML part is generated from the resource
        // name, and the binary part is NULL
        desc = buildTextureDescriptor(name);
    } else if (isTextureFile(name)) {
        // 2D texture resources can be loaded directly from an image file; the
        // texture parameters (internal format, filters, etc) then get default values
        desc = new TiXmlElement("texture2D");
        desc->SetAttribute("name", name);
        desc->SetAttribute("source", name);
        desc->SetAttribute("internalformat", "RGBA8");
        desc->SetAttribute("min", "LINEAR_MIPMAP_LINEAR");
        desc->SetAttribute("mag", "LINEAR");
        desc->SetAttribute("wraps", "REPEAT");
        desc->SetAttribute("wrapt", "REPEAT");
    } else if (name.find(';', 0) != string::npos) {
        // resource names of the form "module1;module;module3;..." describe
        // program resources that may not be described by any file, either for the
        // XML part or for the binary part. The XML part is generated from the
        // resource name, and the binary part is NULL (unless a compiled program
        // exists for this program)
        desc = findDescriptor(name, stamp, false);
        if (desc == NULL) {
            desc = buildProgramDescriptor(name);
        }
    } else if (name.size() >= 5 && strcmp(".mesh", name.substr(name.size() - 5).c_str()) == 0) {
        // mesh resources do not have any file to describe the XML part, which is
        // trivial and hence generated on the fly here:
        desc = new TiXmlElement("mesh");
        desc->SetAttribute("source", name);
    } else {
        // for all other resource types, the XML part is described in a file,
        // which must be loaded
        desc = findDescriptor(name, stamp);
    }
    if (desc != NULL) {
        // when we have the XML part we can load the binary part, if any
        XMLResourceDescriptor::Stamps dataStamps;
        try {
            unsigned int size = 0;
            unsigned char *data = loadData(desc, size, dataStamps);
            return new XMLResourceDescriptor(desc, data, size, stamp, dataStamps);
        } catch (...) {
            delete desc;
        }
    }
    return NULL;
}

ptr<ResourceDescriptor> XMLResourceLoader::reloadResource(const string &name, ptr<ResourceDescriptor> currentValue)
{
    ptr<XMLResourceDescriptor> cur = currentValue.cast<XMLResourceDescriptor>();
    time_t stamp = cur->stamp;
    TiXmlElement *desc = NULL;
    if (strncmp(name.c_str(), "renderbuffer", 12) != 0 &&
        !isTextureFile(name) &&
        name.find(';', 0) == string::npos &&
        strcmp("mesh", cur->descriptor->Value()) != 0)
    {
        // for resources whose XML part is described in a file (see loadResource)
        // we first test if the XML part has changed or not. If it has changed
        // desc contains the new value, and stamp the new last modification time
        desc = findDescriptor(name, stamp);
    }
    if (desc == NULL) {
        // if the XML part has not changed we clone the current value
        desc = cur->descriptor->Clone()->ToElement();
    }
    XMLResourceDescriptor::Stamps dataStamps = cur->dataStamps;
    if (difftime(cur->stamp, stamp) != 0) {
        // if the XML part has changed the files describing the binary part may
        // no longer be the same, so we clear the corresponding modification
        // time vector to force a reloading of the binary part.
        dataStamps.clear();
    }
    try {
        unsigned int size = 0;
        // we now test if the ASCII or binary part has changed
        unsigned char* data = loadData(desc, size, dataStamps);
        if (!cur->equal(desc, stamp, dataStamps)) {
            // if the XML part and/or the binary part has changed
            return new XMLResourceDescriptor(desc, data, size, stamp, dataStamps);
        }
    } catch (...) {
        delete desc;
    }
    return NULL;
}

string XMLResourceLoader::findFile(const TiXmlElement *desc, const vector<string> paths, const string &file)
{
    for (unsigned int i = 0; i < paths.size(); ++i) {
        string path = paths[i] + '/' + file;
        FILE *f;
        fopen(&f, path.c_str(), "rb");
        if (f != NULL) {
            fclose(f);
            return path;
        }
    }
    if (Logger::ERROR_LOGGER != NULL && desc != NULL) {
        Resource::log(Logger::ERROR_LOGGER, desc, desc, "Cannot find '" + string(file) + "' file");
    }
    throw exception();
}

unsigned char *XMLResourceLoader::loadFile(const string &file, unsigned int &size)
{
    ifstream fs(file.c_str(), ios::binary);
    if(!fs.is_open())
        throw runtime_error(file + " was not found!");
    fs.seekg(0, ios::end);
    size = fs.tellg();
    unsigned char *data = new unsigned char[size + 1];
    fs.seekg(0);
    fs.read((char*) data, size);
    fs.close();
    data[size] = 0;
    if (Logger::INFO_LOGGER != NULL) {
        Logger::INFO_LOGGER->log("RESOURCE", "Loaded file '" + file + "'");
    }
    return data;
}

void XMLResourceLoader::getTimeStamp(const string &name, time_t &t)
{
#ifdef _MSC_VER
    FILETIME ftWrite;
    HANDLE hFile = CreateFile((LPCTSTR )name.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE){
        if (GetFileTime(hFile, NULL, NULL, &ftWrite)) {
            CloseHandle(hFile);
            LARGE_INTEGER li;
            li.LowPart =  ftWrite.dwLowDateTime;
            li.HighPart = ftWrite.dwHighDateTime;
            t = li.QuadPart / __int64(10000000);
        }
    }
#else
    int fd = open(name.c_str(), O_RDONLY);
    if (fd != -1) {
        struct stat stats;
        fstat(fd, &stats);
        t = stats.st_mtime;
        close(fd);
    }
#endif
}

TiXmlElement *XMLResourceLoader::findDescriptor(const string &name, time_t &t, bool log)
{
    // we first look in the archive files
    for (unsigned int i = 0; i < archives.size(); ++i) {
        time_t u = t;
        TiXmlDocument *archive = loadArchive(archives[i], u);
        if (archive != NULL) {
            TiXmlElement *desc = findDescriptor(archive, name);
            if (desc != NULL) {
                if (u == t) {
                    // if the last modification time is equal to the last known
                    // modification time, return NULL
                    return NULL;
                } else {
                    t = u;
                    return desc;
                }
            }
        }
    }
    // then in the directories specified with #addPath
    for (unsigned int i = 0; i < paths.size(); ++i) {
        string n = paths[i] + "/" + name + ".xml";
        time_t u = 0;
        getTimeStamp(n, u);
        if (u != 0) {
            if (u == t) {
                // if the last modification time is equal to the last known
                // modification time, return NULL
                return NULL;
            } else {
                t = u;
            }
            unsigned int size = 0;
            unsigned char *data = loadFile(n, size);
            TiXmlDocument doc(n);
            if (doc.Parse((const char*) data)) {
                if (Logger::INFO_LOGGER != NULL) {
                    Logger::INFO_LOGGER->log("RESOURCE", "Loaded file '" + n + "'");
                }
                delete[] data;
                return doc.RootElement()->Clone()->ToElement();
            } else {
                if (data != NULL) {
                    delete[] data;
                }
                if (Logger::ERROR_LOGGER != NULL) {
                    Logger::ERROR_LOGGER->log("RESOURCE", "Syntax error in '" + n + "'");
                }
            }
        }
    }
    if (log && Logger::ERROR_LOGGER != NULL) {
        Logger::ERROR_LOGGER->log("RESOURCE", "Cannot find resource '" + name + "'");
    }
    // resource not found, return NULL
    return NULL;
}

TiXmlElement *XMLResourceLoader::findDescriptor(const TiXmlDocument *archive, const string &name)
{
    // we first load the archive ...
    const TiXmlElement *root = archive->RootElement();
    if (root != NULL) {
        // ... then we look for our descriptor in the archive content
        for (const TiXmlNode *child = root->FirstChild(); child != NULL; child = child->NextSibling()) {
            const TiXmlElement *desc = child->ToElement();
            if (desc != NULL) {
                const char *n = desc->Attribute("name");
                if (n != NULL && strcmp(n, name.c_str()) == 0) {
                    return desc->Clone()->ToElement();
                }
            }
        }
    }
    return NULL;
}

TiXmlElement *XMLResourceLoader::buildTextureDescriptor(const string &name)
{
    string::size_type index1 = name.find('-', 0);
    string::size_type index2 = name.find('-', index1 + 1);
    string size = name.substr(index1 + 1, index2 - index1 - 1);
    string::size_type index3 = name.find('-', index2 + 1);
    string internalformat = name.substr(index2 + 1, index3 == string::npos ? index3 : index3 - index2 - 1);

    TiXmlElement *p = new TiXmlElement("texture2D");
    p->SetAttribute("name", name);
    p->SetAttribute("internalformat", internalformat.c_str());
    p->SetAttribute("width", size.c_str());
    p->SetAttribute("height", size.c_str());
    p->SetAttribute("format", "RED");
    p->SetAttribute("type", "FLOAT");
    p->SetAttribute("min", "NEAREST");
    p->SetAttribute("mag", "NEAREST");
    return p;
}

TiXmlElement *XMLResourceLoader::buildProgramDescriptor(const string &name)
{
    TiXmlElement *p = new TiXmlElement("program");
    p->SetAttribute("name", name);
    string::size_type start = 0;
    string::size_type index;
    while ((index = name.find(';', start)) != string::npos) {
        string module = name.substr(start, index - start);
        TiXmlElement *s = new TiXmlElement("module");
        s->SetAttribute("name", module);
        p->InsertEndChild(*s);
        delete s; // call to TiXmlElement.Clone inside TiXmlElement#InsertEndChild. s wasn't deleted
        start = index + 1;
    }
    return p;
}

TiXmlDocument *XMLResourceLoader::loadArchive(const string &name, time_t &t)
{
    // we first look in the cache
    map<string, pair<TiXmlDocument*, time_t> >::iterator i = cache.find(name);
    if (i != cache.end()) {
        t = i->second.second;
        // if the last modification time of the file is equal to the last
        // modification time of the file in cache ...
        getTimeStamp(name, t);
        if (difftime(i->second.second, t) == 0) {
            // ... then we just return the cached file content
            return i->second.first;
        }
    }
    unsigned int size = 0;
    unsigned char *data = loadFile(name, size);
    // then we try to load the archive file
    TiXmlDocument *doc = new TiXmlDocument(name);
    if (doc->Parse((const char*) data)) {
        delete[] data;
        if (Logger::INFO_LOGGER != NULL) {
            Logger::INFO_LOGGER->log("RESOURCE", "Loaded file '" + name + "'");
        }
        if (i != cache.end()) {
            // if the cache already contains a value for this name, delete it
            delete i->second.first;
        } else {
            getTimeStamp(name, t);
        }
        // put the new value and its last modification time in cache
        cache[name] = make_pair(doc, t);
        return doc;
    } else {
        if (data != NULL) {
            delete[] data;
        }
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("RESOURCE", "File not found or syntax error in '" + name + "'");
        }
        delete doc;
        return NULL;
    }
}

unsigned char* XMLResourceLoader::loadData(TiXmlElement *desc, unsigned int &size, vector< pair<string, time_t> > &stamps)
{
    // if the resource has an ASCII or binary part ...
    if (strcmp(desc->Value(), "texture1D") == 0 ||
        strcmp(desc->Value(), "texture1DArray") == 0 ||
        strcmp(desc->Value(), "texture2D") == 0 ||
        strcmp(desc->Value(), "texture2DArray") == 0 ||
        strcmp(desc->Value(), "texture3D") == 0 ||
        strcmp(desc->Value(), "textureCube") == 0 ||
        strcmp(desc->Value(), "textureCubeArray") == 0 ||
        strcmp(desc->Value(), "textureRectangle") == 0 ||
        strcmp(desc->Value(), "module") == 0 ||
        strcmp(desc->Value(), "mesh") == 0 ||
        strcmp(desc->Value(), "program") == 0)
    {
        // we first get the name of the file containing this ASCII or binary part
        string str;
        const char* file = desc->Attribute("source");
        if (file == NULL && strcmp(desc->Value(), "program") == 0) {
            str = string(desc->Attribute("name")) + ".bin";
            try {
                findFile(NULL, paths, str);
                file = str.c_str();
            } catch (...) {
            }
            if (file == NULL) {
                return NULL;
            }
        } else if (file == NULL) {
            if (strcmp(desc->Value(), "module") != 0 &&
                strcmp(desc->Value(), "mesh") != 0 &&
                desc->Attribute("width") != NULL)
            {
                // a texture resource can have no binary part, provided its
                // dimensions are specified in the XML part. In this case we
                // return a NULL binary part.
                return NULL;
            }
            if (!(strcmp(desc->Value(), "module") == 0 &&
                (desc->Attribute("vertex") != NULL ||
                desc->Attribute("tessControl") != NULL ||
                desc->Attribute("tessEvaluation") != NULL ||
                desc->Attribute("geometry") != NULL ||
                desc->Attribute("fragment") != NULL)))
            {
                // a module may not have a 'source' attribute if it has one
                // of the vertex, tessControl, ... or fragment attribute
                if (Logger::ERROR_LOGGER != NULL) {
                    Resource::log(Logger::ERROR_LOGGER, desc, desc, "Missing 'source' attribute");
                }
                // otherwise it is an error if the binary part file name is not specified
                throw exception();
            }
        }

        // then we test if the modification times have changed or not
        // if not then do not load the ASCII or binary part again
        bool doLoad = false;
        if (stamps.size() == 0) { // if the binary part has not been loaded yet
            doLoad = true; // we load it
        } else {
            // otherwise we load it only if the last modification times have changed
            for (unsigned int i = 0; i < stamps.size(); ++i) {
                time_t newt = 0;
                getTimeStamp(stamps[i].first, newt);
                if (difftime(newt, stamps[i].second) != 0) {
                    doLoad = true;
                    break;
                }
            }
        }
        if (!doLoad) {
            return NULL;
        }

        // special case for modules made of separate files
        if (strcmp(desc->Value(), "module") == 0 && file == NULL) {
            stamps.clear();
            unsigned char *vertexData = NULL;
            unsigned int vertexSize = 0;
            if (desc->Attribute("vertex") != NULL) {
                string path = findFile(desc, paths, desc->Attribute("vertex"));
                unsigned char *data = loadFile(path, vertexSize);
                vertexData = loadShaderData(desc, paths, path, data, vertexSize, stamps);
            }
            unsigned char *tessControlData = NULL;
            unsigned int tessControlSize = 0;
            if (desc->Attribute("tessControl") != NULL) {
                string path = findFile(desc, paths, desc->Attribute("tessControl"));
                unsigned char *data = loadFile(path, tessControlSize);
                tessControlData = loadShaderData(desc, paths, path, data, tessControlSize, stamps);
            }
            unsigned char *tessEvalData = NULL;
            unsigned int tessEvalSize = 0;
            if (desc->Attribute("tessEvaluation") != NULL) {
                string path = findFile(desc, paths, desc->Attribute("tessEvaluation"));
                unsigned char *data = loadFile(path, tessEvalSize);
                tessEvalData = loadShaderData(desc, paths, path, data, tessEvalSize, stamps);
            }
            unsigned char *geometryData = NULL;
            unsigned int geometrySize = 0;
            if (desc->Attribute("geometry") != NULL) {
                string path = findFile(desc, paths, desc->Attribute("geometry"));
                unsigned char *data = loadFile(path, geometrySize);
                geometryData = loadShaderData(desc, paths, path, data, geometrySize, stamps);
            }
            unsigned char *fragmentData = NULL;
            unsigned int fragmentSize = 0;
            if (desc->Attribute("fragment") != NULL) {
                string path = findFile(desc, paths, desc->Attribute("fragment"));
                unsigned char *data = loadFile(path, fragmentSize);
                fragmentData = loadShaderData(desc, paths, path, data, fragmentSize, stamps);
            }
            size = vertexSize + tessControlSize + tessEvalSize + geometrySize + fragmentSize + 5;
            unsigned char *data = new unsigned char[size];
            unsigned int offset = 0;
            if (vertexData != NULL) {
                memcpy(data + offset, vertexData, vertexSize);
                offset += vertexSize;
                delete[] vertexData;
            }
            data[offset++] = 0;
            if (tessControlData != NULL) {
                memcpy(data + offset, tessControlData, tessControlSize);
                offset += tessControlSize;
                delete[] tessControlData;
            }
            data[offset++] = 0;
            if (tessEvalData != NULL) {
                memcpy(data + offset, tessEvalData, tessEvalSize);
                offset += tessEvalSize;
                delete[] tessEvalData;
            }
            data[offset++] = 0;
            if (geometryData != NULL) {
                memcpy(data + offset, geometryData, geometrySize);
                offset += geometrySize;
                delete[] geometryData;
            }
            data[offset++] = 0;
            if (fragmentData != NULL) {
                memcpy(data + offset, fragmentData, fragmentSize);
                offset += fragmentSize;
                delete[] fragmentData;
            }
            data[offset++] = 0;
            return data;
        }

        // then we load the raw ASCII or binary part
        string path = stamps.size() == 0 ? findFile(desc, paths, file) : stamps[0].first;
        unsigned char *data = loadFile(path, size);

        stamps.clear();

        // and then we process it depending on the type of the resource
        if (strcmp(desc->Value(), "module") == 0) {
            // for a shader resource the ASCII part can reference other files
            // via #include directives; we need to load them and to substitute
            // their content
            return loadShaderData(desc, paths, path, data, size, stamps);
        } else if (strcmp(desc->Value(), "mesh") == 0 ||
                   strcmp(desc->Value(), "program") == 0)
        {
            // for a mesh or compiled program resource, no processing is needed
            time_t t;
            getTimeStamp(path, t);
            stamps.push_back(make_pair(path, t));
            return data;
        } else {
            // for a texture we need to decompress the file (PNG, JPG, etc)
            return loadTextureData(desc, path, data, size, stamps);
        }
    }
    return NULL;
}

unsigned char* XMLResourceLoader::loadShaderData(TiXmlElement *desc, const vector<string> &paths,
        const string &path, unsigned char *data, unsigned int &size, vector< pair<string, time_t> > &stamps)
{
    time_t t = 0;
    getTimeStamp(path, t);
    stamps.push_back(make_pair(path, t));
    if (strstr((char*) data, "#include") == NULL) {
        // if there is no #include directive in 'data' then we can directly return
        return data;
    }
    // otherwise we must load the referenced files and substitute their content;
    // the result will be placed in the 'result' string
    string result;
    bool comment = false;
    bool lineComment = false;
    unsigned int i = 0;
    // we parse the file to find the #include that are not inside comments
    while (i < size) {
        if (!comment) { // if we are not inside a comment
            if (i + 1 < size) {
                if (data[i] == '/' && data[i + 1] == '*') {
                    // if we find a '/*' then we now are in a comment
                    result.append((char*) data, i, 2);
                    comment = true;
                    lineComment = false;
                    i += 2;
                    continue;
                } else if (data[i] == '/' && data[i + 1] == '/') {
                    // likewise if we find a '//'
                    result.append((char*) data, i, 2);
                    comment = true;
                    lineComment = true;
                    i += 2;
                    continue;
                }
            }
            if (i + 8 <= size && strncmp((char*) data + i, "#include", 8) == 0) {
                // if we find a #include
                char* s = strchr((char*) data + i, '\"');
                if (s != NULL) {
                    char *e = strchr(s + 1, '\"');
                    if (e != NULL) {
                        // we first extract the referenced file name
                        string inc = string(s + 1, e - s - 1);
                        string incFile;
                        try {
                            // then we find the absolute name of this file
                            incFile = findFile(desc, paths, inc);
                        } catch (...) {
                            delete[] data;
                            throw exception();
                        }
                        unsigned int incSize;
                        // we can then load the content of the referenced file
                        unsigned char* incData = loadFile(incFile, incSize);
                        // and then analyze its content with a recursive call
                        // to process the #include directives that this file may
                        // in turn contain
                        unsigned char* incShader = loadShaderData(desc, paths, incFile, incData, incSize, stamps);
                        // finally we append this processed content to the
                        // result data, instead of the #include directive itself
                        result.append((char*) incShader);
                        delete[] incShader;

                        i = (e - (char*) data) + 1;
                        continue;
                    }
                }
            }
        } else {
            if (lineComment) { // if we are in a line comment
                if (data[i] == '\n') { // and find a newline we exit the comment
                    result.append((char*) data, i++, 1);
                    comment = false;
                    continue;
                }
            } else if (i + 1 < size && data[i] == '*' && data[i + 1] == '/') {
                // likewise, if we find a '*/' in a block comment, we exit it
                result.append((char*) data, i, 2);
                comment = false;
                i += 2;
                continue;
            }
        }
        result.append((char*) data, i++, 1);
    }
    delete[] data;
    size = result.size();
    data = new unsigned char[size + 1];
    memcpy(data, result.c_str(), size + 1);
    return data;
}

unsigned char* XMLResourceLoader::loadTextureData(TiXmlElement *desc, const string &path,
        unsigned char *data, unsigned int &size, vector< pair<string, time_t> > &stamps)
{
    unsigned char* trailer = data + size - 5 * sizeof(int);
    unsigned char *result = NULL;
    int w;
    int h = 1;
    int d = 0;
    int channels;
    bool raw = ((unsigned int*) trailer)[0] == 0xCAFEBABE;
    bool hdr = stbi_is_hdr_from_memory(data, size) > 0;
    if (raw) { // if the data ends with '0xCAFEBABE' w h d c
        // then it is a raw file containing w*h*c floats (h multiple of d)
        w = ((int*) trailer)[1]; // texture width
        h = ((int*) trailer)[2]; // texture height (ignored for 1D textures)
        d = ((int*) trailer)[3]; // texture depth (0 for 2D textures)
        channels = ((int*) trailer)[4]; // number of channels per pixel (1..4)
        result = data;
        if (d > 0) {
            desc->SetAttribute("depth", d);
        }
    } else if (hdr) { // file in radiance HDR file format
        result = (unsigned char*) stbi_loadf_from_memory(data, size, &w, &h, &channels, 0);
        delete[] data;
    } else { // file in PNG, JPG or other LDR formats supported by stbi library
        result = stbi_load_from_memory(data, size, &w, &h, &channels, 0);
        delete[] data;
    }
    if (result == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("RESOURCE", "Cannot load texture file '" + path + "'");
        }
        throw exception();
    }

    desc->SetAttribute("width", w);
    if (desc->Attribute("height") == NULL) {
        desc->SetAttribute("height", h);
    }
    if (channels == 1) {
        if (desc->Attribute("format") == NULL) {
            desc->SetAttribute("format", "RED");
        }
    } else if (channels == 2) {
        if (desc->Attribute("format") == NULL) {
            desc->SetAttribute("format", "RG");
        }
    } else if (channels == 3) {
        if (desc->Attribute("format") == NULL) {
            desc->SetAttribute("format", "RGB");
        }
    } else if (channels == 4) {
        if (desc->Attribute("format") == NULL) {
            desc->SetAttribute("format", "RGBA");
        }
    } else {
        if (raw) {
            delete[] data;
        } else {
            stbi_image_free(result);
        }
        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->log("RESOURCE", "Unsupported texture format '" + path + "'");
        }
        throw exception();
    }
    if (raw || hdr) {
        desc->SetAttribute("type", "FLOAT");
    } else {
        desc->SetAttribute("type", "UNSIGNED_BYTE");
    }

    int lineSize = w * channels * (raw || hdr ? sizeof(float) : 1);
    size = lineSize * h;

    unsigned char* flippedResult;

    if (raw) {
        flippedResult = result;
    } else {
        // all formats except 'raw' store the image from top to bottom
        // while OpenGL requires a bottom to top layout; so we revert the
        // order of lines here to get a good orientation in OpenGL
        flippedResult = new unsigned char[lineSize * h];
        int srcOffset = 0;
        int dstOffset = lineSize * (h - 1);
        for (int i = 0; i < h; ++i) {
            memcpy(flippedResult + dstOffset, result + srcOffset, lineSize);
            srcOffset += lineSize;
            dstOffset -= lineSize;
        }
        stbi_image_free(result);
    }

    time_t t = 0;
    getTimeStamp(path, t);
    stamps.push_back(make_pair(path, t));

    return flippedResult;
}

}
