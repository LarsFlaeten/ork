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

#include "ork/resource/Resource.h"

#include "ork/resource/ResourceManager.h"

using namespace std;

namespace ork
{

static bool TiXmlGetLocation(const TiXmlNode *parent, const TiXmlNode *e, int &loc)
{
    if (e == parent) {
        return true;
    }
    const TiXmlNode* p = parent->FirstChild();
    if (p == NULL) {
        ++loc;
    } else {
        ++loc;
        while (p != NULL) {
            if (TiXmlGetLocation(p, e, loc)) {
                return true;
            }
            p = p->NextSibling();
        }
        ++loc;
    }
    return false;
}

Resource::Resource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc) :
    manager(manager), name(name), desc(desc)
{
}

Resource::~Resource()
{
    if (manager != NULL) {
        manager->removeResource(this);
    }
}

string Resource::getName()
{
    return name;
}

bool Resource::prepareUpdate()
{
    if (manager == NULL) {
        return false;
    }
    ptr<ResourceLoader> loader = manager->getLoader();
    newDesc = loader->reloadResource(name, desc);
    return newDesc != NULL;
}

bool Resource::changed()
{
    return newDesc != NULL;
}

void Resource::checkParameters(const ptr<ResourceDescriptor> desc,
        const TiXmlElement *e, const string &params)
{
    const TiXmlAttribute *a = e->FirstAttribute();
    while (a != NULL) {
        if (params.find(string(a->Name()) + ",", 0) == string::npos) {
            if (Logger::ERROR_LOGGER != NULL) {
                Resource::log(Logger::ERROR_LOGGER, desc, e, "Unsupported '" + string(a->Name()) + "' attribute");
            }
            throw exception();
        }
        a = a->Next();
    }
}

void Resource::getIntParameter(const ptr<ResourceDescriptor> desc,
        const TiXmlElement *e, const string &name, int *i)
{
    if (e->QueryIntAttribute(name.c_str(), i) != TIXML_SUCCESS) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Missing or bad '" + name + "' attribute");
        }
        throw exception();
    }
}

bool Resource::getFloatParameter(const ptr<ResourceDescriptor> desc,
        const TiXmlElement *e, const string &name, float *value)
{
    int r = e->QueryFloatAttribute(name.c_str(), value);
    if (r == TIXML_WRONG_TYPE) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Bad '" + name + "' attribute");
        }
        throw exception();
    }
    return r == TIXML_SUCCESS;
}

string Resource::getParameter(ptr<ResourceDescriptor> desc,
        const TiXmlElement *e, const char* name)
{
    const char* value = e->Attribute(name);
    if (value == NULL) {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Missing '" + string(name) + "' attribute");
        }
        throw exception();
    }
    return string(value);
}

void Resource::log(ptr<Logger> logger, ptr<ResourceDescriptor> desc,
        const TiXmlElement *e, const string &msg)
{
    log(logger, desc->descriptor, e, msg);
}

void Resource::log(ptr<Logger> logger, const TiXmlElement *desc,
        const TiXmlElement *e, const string &msg)
{
    TiXmlPrinter p;
    desc->Accept(&p);
    if (e == NULL) {
        logger->log("RESOURCE", msg + " in \033" + p.CStr() + "\033");
    } else {
        int line = 1;
        if (!TiXmlGetLocation(desc, e, line)) {
            line = 0;
        }
        ostringstream os;
        os << msg << " at line " << line << " in \033" << p.CStr() << "\033";
        logger->log("RESOURCE", os.str());
    }
}

}
