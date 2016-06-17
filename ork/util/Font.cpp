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

#include "ork/util/Font.h"
#include "ork/resource/ResourceTemplate.h"

using namespace std;

namespace ork
{

Font::Vertex::Vertex()
{
}

Font::Vertex::Vertex(const vec4h &pos_uv, int color) :
    pos_uv(pos_uv), r(color >> 24), g(color >> 16), b(color >> 8), a(color)
{
}

Font::Font(ptr<Texture2D> fontTex, int nCols, int nRows,
     int minChar, int maxChar, int invalidChar, bool fixedWidth, std::vector<int> charWidths)
    : Object("Font")
{
    init(fontTex, nCols, nRows, minChar, maxChar, invalidChar, fixedWidth, charWidths);
}

Font::Font() : Object("Font")
{
}

Font::~Font()
{
}

void Font::init(ptr<Texture2D> fontTex, int nCols, int nRows,
                int minChar, int maxChar, int invalidChar, bool fixedWidth, std::vector<int> charWidths)
{
    this->fontTex = fontTex;
    this->nCols = nCols;
    this->nRows = nRows;
    this->minChar = minChar;
    this->maxChar = maxChar;
    this->invalidChar = invalidChar;
    this->charWidths = charWidths;
    this->fixedWidth = fixedWidth;
    assert(static_cast<int>(charWidths.size()) == 1 + maxChar - minChar);
}

ptr<Texture2D> Font::getImage() const
{
    return fontTex;
}

float Font::getTileWidth() const
{
    return static_cast<float>(fontTex->getWidth()) / nCols;
}

float Font::getTileHeight() const
{
    return static_cast<float>(fontTex->getHeight()) / nRows;
}

float Font::getTileAspectRatio() const
{
    return getTileWidth() / getTileHeight();
}

int Font::charCount(char c) const
{
    int i = c;
    if (i < minChar) {
        return invalidChar - minChar;
    }
    if (i > maxChar) {
        return invalidChar - minChar;
    }
    return i - minChar;
}

float Font::getCharWidth(char c) const
{
    return charWidths[charCount(c)];
}

vec2f Font::getSize(const string &line, float height) const
{
    float res = 0.f;
    int lineSize = static_cast<int>(line.size());

    for (int i = 0; i < lineSize; ++i) {
        int index = charCount(line[i]);
        int width = charWidths[index];
        res += (height * width) / getTileWidth();
    }
    return vec2f(res, height);
}

vec2f Font::addLine(const vec4f &viewport, float xs, float ys, const string &line, float height,
    int color, ptr< Mesh<Vertex, unsigned int> > textMesh)
{
    for (unsigned int i = 0; i < line.size(); ++i) {

        int index = charCount(line[i]);
        int width = charWidths[index];

        // use isFixedSize to determine
        // If isFixedSize == true, draw smaller non-overlapping quads
        //                == false, draw overlapping quads to allow overlapping characters
        float charRatio = fixedWidth ? (width / (float)getTileWidth()) : 1.f;

        int x = index % nCols;
        int y = index / nCols;
        y = (nRows - 1) - y;

        float u0 = x / (float)nCols;
        float u1 = (x + charRatio) / (float)nCols;
        float v0 = y / (float)nRows;
        float v1 = (y + 1.f) / (float)nRows;

        float tileAspectRatio = getTileAspectRatio();

        float xs0 = xs / viewport.z;
        float xs1 = (xs + charRatio * tileAspectRatio * height) / viewport.z;
        float ys0 = ys / viewport.w;
        float ys1 = (ys + height) / viewport.w;

        vec4h pos_uv0 = vec4f(xs0 * 2.0f - 1.0f, 1.0f - ys1 * 2.0f, u0, v0).cast<half>();
        vec4h pos_uv1 = vec4f(xs1 * 2.0f - 1.0f, 1.0f - ys1 * 2.0f, u1, v0).cast<half>();
        vec4h pos_uv2 = vec4f(xs1 * 2.0f - 1.0f, 1.0f - ys0 * 2.0f, u1, v1).cast<half>();
        vec4h pos_uv3 = vec4f(xs0 * 2.0f - 1.0f, 1.0f - ys0 * 2.0f, u0, v1).cast<half>();

        textMesh->addVertex(Vertex(pos_uv0, color));
        textMesh->addVertex(Vertex(pos_uv1, color));
        textMesh->addVertex(Vertex(pos_uv2, color));
        textMesh->addVertex(Vertex(pos_uv2, color));
        textMesh->addVertex(Vertex(pos_uv3, color));
        textMesh->addVertex(Vertex(pos_uv0, color));

        xs += (height * width) / getTileWidth();
    }

    return vec2f(xs, ys);
}

vec2f Font::addCenteredLine(const vec4f &viewport, float xs, float ys, const string &line, float height,
    int color, ptr< Mesh<Vertex, unsigned int> > textMesh)
{
    vec2f size = getSize(line, height);
    xs -= size.x * 0.5f;
    addLine(viewport, xs, ys, line, height, color, textMesh);
    return size;
}

void Font::swap(ptr<Font> t)
{
    std::swap(fontTex, t->fontTex);
    std::swap(nCols, t->nCols);
    std::swap(nRows, t->nRows);
    std::swap(minChar, t->minChar);
    std::swap(maxChar, t->maxChar);
    std::swap(invalidChar, t->invalidChar);
    std::swap(fixedWidth, t->fixedWidth);
    std::swap(charWidths, t->charWidths);
}

class FontResource : public ResourceTemplate<40, Font>
{
public:
    FontResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<40, Font>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,tex,nCols,nRows,minChar,maxChar,invalidChar,charWidths,");

        string tex = getParameter(desc, e, "tex");
        ptr<Texture2D> fontTex = manager->loadResource(tex).cast<Texture2D>();

        vec2f fontSize = vec2f(19.0f / 32.0f, 24.0f);
        int nCols, nRows;
        int minChar, maxChar, invalidChar;

        getIntParameter(desc, e, "nCols", &nCols);
        getIntParameter(desc, e, "nRows", &nRows);
        getIntParameter(desc, e, "minChar", &minChar);
        getIntParameter(desc, e, "maxChar", &maxChar);
        getIntParameter(desc, e, "invalidChar", &invalidChar);

        // some checks
        assert(nCols >= 0);
        assert(nRows >= 0);
        assert(minChar >= 0);
        assert(minChar <= 255);
        assert(maxChar > minChar); // at least 1 character
        assert(maxChar <= 255);
        assert(invalidChar >= minChar);
        assert(invalidChar <= maxChar);

        std::vector<int> charWidths;
        int count = 1 + maxChar - minChar;

        charWidths.resize(count);

        bool fixedWidth;

        // parse character widths
        // if only one width is specified, the font is fixed-width and all character have the same width
        //                           else, the font is variable-width and character can overlap
        {
            int n = 0;
            string charWidthS = std::string(e->Attribute("charWidths")) + ",";
            string::size_type start = 0;
            string::size_type index;
            int width;

            while ((index = charWidthS.find(',', start)) != string::npos) {

                string widthStr = charWidthS.substr(start, index - start);
                start = index + 1;
                istringstream tmpStream(widthStr);

                if (tmpStream >> width) {
                    if (n >= count) assert(false); // too much widths
                    charWidths[n] = width;
                } else {
                    assert(false); // not an integer number, check your XML
                }
                n++;
            }
            assert(n > 0);

            fixedWidth = (n == 1);

            // no more items, fill with last one (allow to specify only one width for all characters)
            for (int i = n; i < count; ++i) {
                charWidths[i] = charWidths[i - 1];
            }
        }

        init(fontTex, nCols, nRows, minChar, maxChar, invalidChar, fixedWidth, charWidths);
    }
};

extern const char font[] = "font";

static ResourceFactory::Type<font, FontResource> FontType;

}
