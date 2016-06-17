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

#ifndef _ORK_FONT_H_
#define _ORK_FONT_H_

#include "ork/math/vec2.h"
#include "ork/math/vec4.h"
#include "ork/render/Texture2D.h"
#include "ork/render/Mesh.h"

#include <string>
#include <vector>

namespace ork
{

/**
 * @defgroup util util
 * @ingroup ork
 * Provides utility classes.
 */

/**
 * Helper class to draw text in a given font.
 * A Font allows to easily write a line of text directly in a FrameBuffer
 * at a given position. It has a texture containing ascii chars, and knows
 * which can be displayed. Any character outside its range will be displayed
 * as a blank character defined in the texture (for example a square, or a
 * question mark).
 *
 * @ingroup util
 */
class ORK_API Font : public Object
{
public:
    /**
     * Vertex format for a text mesh.
     */
    struct Vertex
    {
        vec4h pos_uv;

        unsigned char r, g, b, a;

        Vertex();

        Vertex(const vec4h &pos_uv, int color);
    };

    /**
     * Creates a new Font.
     *
     * @param fontTex the Texture2D which contains the images of the font.
     * @param nCols the number of characters columns in the texture.
     * @param nRows the number of characters rows in the texture.
     * @param minChar the first ascii char code to take into account.
     *      Lower chars will be replaced with 'invalidChar'.
     * @param maxChar the last ascii char code to take into account.
     *      Higher chars will be replaced with 'invalidChar'.
     * @param invalidChar the character to be used to display invalid characters.
     * @param fixedWidth whether the font is fixed-width (faster draw call, disallow overlapping characters)
     * @param charWidths an array of (maxChar - minChar + 1) character widths in texel
     *      NOTE: charWidth does NOT give texture coordinates, it gives spaces between characters.
     */
    Font(ptr<Texture2D> fontTex, int nCols, int nRows, int minChar, int maxChar, int invalidChar, bool fixedWidth, std::vector<int> charWidths);

    /**
     * Deletes this Font.
     */
    virtual ~Font();

    /**
     * Returns the texture containing the image of this font.
     */
    ptr<Texture2D> getImage() const;

    /**
     * Returns the width of a character tile.
     */
    float getTileWidth() const;

    /**
     * Returns the height of a character tile.
     */
    float getTileHeight() const;

    /**
     * Returns the aspect ratio of a character tile.
     */
    float getTileAspectRatio() const;

    /**
     * Returns the space between this char and the next one.
     * Approximate the width of the drawed char (it's a bit different for legibility purpose).
     */
    float getCharWidth(char c) const;

    /**
     * Returns the size of a given line of text.
     *
     * @param line the line of text.
     * @param height the height of output text in pixels
     */
    vec2f getSize(const string &line, float height) const;

    /**
     * Add a given line of text in a given Mesh and returns the final
     * position of the line.
     *
     * @param viewport the framebuffer viewport, in pixels.
     * @param xs the x coordinate of the first character to display.
     * @param ys the y coordinate of the first character to display.
     * @param line the line of text to display.
     * @param height height of a char in pixels.
     * @param color color of this line of text (in RGBA8 format).
     * @param textMesh the mesh to write into.
     */
    vec2f addLine(const vec4f &viewport, float xs, float ys, const string &line, float height,
            int color, ptr< Mesh<Vertex, unsigned int> > textMesh);

    /**
     * Add a given line of text in a given Mesh centered at a given
     * position and returns the size of the line.
     *
     * @param viewport the framebuffer viewport, in pixels.
     * @param xs the x coordinate of the center of the line to display.
     * @param ys the y coordinate of the center of the line to display.
     * @param line the line of text to display.
     * @param height height of a char in pixels.
     * @param color color of this line of text (in RGBA8 format).
     * @param textMesh the mesh to write into.
     */
    vec2f addCenteredLine(const vec4f &viewport, float xs, float ys, const string &line, float height,
            int color, ptr< Mesh<Vertex, unsigned int> > textMesh);

protected:
    /**
     * The Texture2D which contains the images of the font.
     * This texture is splitted in rows and colums which form "tiles".
     */
    ptr<Texture2D> fontTex;

    /**
     * The number of character columns in the texture.
     */
    int nCols;

    /**
     * The number of character rows in the texture.
     */
    int nRows;

    /**
     * The first ascii char code to take into account.
     * Lower chars will be replaced with #invalidChar.
     */
    int minChar;

    /**
     * The last ascii char code to take into account.
     * Higher chars will be replaced with #invalidChar.
     */
    int maxChar;

    /**
     * The character to be used to display invalid characters.
     * Must be between #minChar and #maxChar.
     */
    int invalidChar;

    /**
     * If the font has fixed-width characters.
     */
    bool fixedWidth;

    /**
     * The width of a char (in texels).
     * Allows to have good-looking variable sized fonts.
     * The ratio width/height of a given character is logicalWidth[c] / (textureHeight /
     */
    std::vector<int> charWidths;

    /**
     * Creates an uninitialized Font.
     */
    Font();

    /**
     * Initializes the fields of a Font.
     *
     * @param fontTex the Texture2D which contains the images of the font.
     * @param nCols the number of characters columns in the texture.
     * @param nRows the number of characters rows in the texture.
     * @param minChar the first ascii char code to take into account.
     *      Lower chars will be replaced with 'invalidChar'.
     * @param maxChar the last ascii char code to take into account.
     *      Higher chars will be replaced with 'invalidChar'.
     * @param invalidChar the character to be used to display invalid characters.
     * @param fixedWidth whether the font is fixed-width (faster draw call, disallow overlapping characters)
     * @param charWidths an array of (maxChar - minChar + 1) character widths in texel
     *      NOTE: charWidth does NOT give texture coordinates, it gives spaces between characters.
     */
    virtual void init(ptr<Texture2D> fontTex, int nCols, int nRows, int minChar, int maxChar, int invalidChar, bool fixedWidth, std::vector<int> charWidths);

    /**
     * Get the tile index for this character.
     * Force the last character if c is not supported by this Font.
     */
    int charCount(char c) const;

    virtual void swap(ptr<Font> t);
};

}

#endif
