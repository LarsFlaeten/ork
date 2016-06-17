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

#include "ork/resource/Resource.h"

#include "ork/resource/ResourceManager.h"

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
