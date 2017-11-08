//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  FreetypeWrapper: 
//      Wrapper class around freetype context
//

#pragma once

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <vector>
#include <string>
#include <stdexcept>

namespace freetype
{
    using std::vector;
    using std::string;

    struct FontData
    {
        float mFontHeight;
        GLuint * mTextures;         // The texture IDs
        GLuint mListBase;           // The first Display List ID
        int * mCharacterWidths;     // The pixel width of each character

        void init(const char * fname, unsigned int aFontHeight);
        void clean();
    };

    void print(const FontData &aFontData, float x, float y, const std::wstring & aText);
}
