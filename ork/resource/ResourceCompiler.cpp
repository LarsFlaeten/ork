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

#include "ork/resource/ResourceCompiler.h"

namespace ork
{

int resCounter = 0;

int compile(TiXmlElement *e, ostream &out)
{
    int p = resCounter++;
    out << "TiXmlElement *e" << p << " = new TiXmlElement(\"" << e->Value() << "\");" << endl;

    TiXmlAttribute *a = e->FirstAttribute();
    while (a != NULL) {
        out << "e" << p << "->SetAttribute(\"" << a->Name() << "\", \"" << a->Value() << "\");" << endl;
        a = a->Next();
    }

    TiXmlNode *n = e->FirstChild();
    while (n != NULL) {
        TiXmlElement *f = n->ToElement();
        if (f != NULL) {
            int c = compile(f, out);
            out << "e" << p << "->LinkEndChild(e" << c << ");" << endl;
        }
        n = n->NextSibling();
    }

    return p;
}

void compile(unsigned char* data, unsigned int size, ostream &out, unsigned int &offset)
{
    char c = 0;
    out.write((const char*) data, size);
    out.write(&c, 1);
    offset += size + 1;
}

ResourceCompiler::ResourceCompiler(const string &resourceFile, const string &resourceDataFile) :
    XMLResourceLoader(),
    out(resourceFile.c_str(), ios_base::out),
    dout(resourceDataFile.c_str(), ios_base::out | ios_base::binary),
    offset(0)
{
}

ResourceCompiler::~ResourceCompiler()
{
    out.close();
    dout.close();
}

string ResourceCompiler::findResource(const string &name)
{
    string s = XMLResourceLoader::findResource(name);
    out << "addPath(\"" << name << "\", \"" << s << "\");" << endl;
    return s;
}

ptr<ResourceDescriptor> ResourceCompiler::loadResource(const string &name)
{
    ptr<ResourceDescriptor> desc = XMLResourceLoader::loadResource(name);
    int a = compile((TiXmlElement*) desc->descriptor, out);
    if (desc->getData() != NULL) {
        unsigned int o = offset;
        compile(desc->getData(), desc->getSize(), dout, offset);
        out << "addResource(\"" << name << "\", new StaticResourceDescriptor(e" << a << ", data + " << o << ", " << desc->getSize() << "));" << endl;
    } else {
        out << "addResource(\"" << name << "\", new ResourceDescriptor(e" << a << ", NULL, 0));" << endl;
    }
    return desc;
}

}
