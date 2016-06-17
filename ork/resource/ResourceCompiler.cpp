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

#include "ork/resource/ResourceCompiler.h"

using namespace std;

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
