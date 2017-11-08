//  Based on OpenGL font library code that is part of a tutorial
//  for nehe.gamedev.net written by Sven Olsen, 2003.

#include <SEGIS/Types.h>
#include <SEGIS/FreetypeWrapper.h>

#define NUM_TEXTURES 256

namespace freetype
{
    inline UInt32 nextPowerOfTwo(UInt32 a)
    {
        UInt32 rval = 1;
        while (rval < a)
        {
            rval <<= 1;
        }
        return rval;
    }

    // Create a display list entry for a character and store character width
    void makeDisplayList(FT_Face aFontFace,
                         unsigned short aCharacter,
                         GLuint aListBase,
                         GLuint * aTextureBase,
                         FontData * aFontData)
    {
        // Load character Glyph
        FT_UInt lCharIndex = FT_Get_Char_Index(aFontFace, aCharacter);
        if (FT_Load_Glyph(aFontFace, lCharIndex, FT_LOAD_DEFAULT))
        {
            throw std::runtime_error("FT_Load_Glyph failed");
        }

        // Move into Glyph object
        FT_Glyph lGlyph;
        if (FT_Get_Glyph(aFontFace->glyph, &lGlyph))
        {
            throw std::runtime_error("FT_Get_Glyph failed");
        }

        // Convert to bitmap
        FT_Vector origin = {0, 4 << 6};
        if (FT_Glyph_To_Bitmap(&lGlyph, ft_render_mode_normal, &origin, 1))
        {
            throw std::runtime_error("FT_Glyph_To_Bitmap failed");
        }

        FT_BitmapGlyph lBitmapGlyph = (FT_BitmapGlyph) lGlyph;

        FT_Bitmap& lBitmap = lBitmapGlyph->bitmap;
        aFontData->mCharacterWidths[aCharacter] = aFontFace->glyph->advance.x >> 6;
        UInt32 lWidthP2 = nextPowerOfTwo(lBitmap.width);
        UInt32 lHeightP2 = nextPowerOfTwo(lBitmap.rows);
        // One byte luma, one byte alpha
        GLubyte* lPixelBuf = new GLubyte[2 * lWidthP2 * lHeightP2];
        // How much of the bitmap is actual character data, not padding?
        float lX = float(lBitmap.width) / float(lWidthP2);
        float lY = float(lBitmap.rows) / float(lHeightP2);


        // Set pixel values
        //   Luma:  0xff
        //   Alpha: From FreeType Bitmap
        for (UInt32 j = 0; j < lHeightP2; j++)
        {
            for (UInt32 i = 0; i < lWidthP2; i++)
            {
                GLubyte * lBufPtr = lPixelBuf + 2 * (i + j * lWidthP2);
                *lBufPtr++ = 0xff;

                if (i >= lBitmap.width || j >= lBitmap.rows)
                {
                    *lBufPtr = 0;
                }
                else
                {
                    *lBufPtr = lBitmap.buffer[i + lBitmap.width * j];
                }
            }
        }

        // Texture paramaters
        glBindTexture(GL_TEXTURE_2D, aTextureBase[aCharacter]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Create the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     lWidthP2, lHeightP2,
                     0, GL_LUMINANCE_ALPHA,
                     GL_UNSIGNED_BYTE, lPixelBuf);

        // Textures are created - we can deallocate the pixel buffer
        delete [] lPixelBuf;

        glNewList(aListBase + aCharacter, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, aTextureBase[aCharacter]);

        glPushMatrix();
        {
            glTranslatef(lBitmapGlyph->left, 0, 0);
            glTranslatef(0, lBitmapGlyph->top - lBitmap.rows, 0);
            glTranslatef(0, -4, 0);

            glBegin(GL_QUADS);
            {
                glTexCoord2d(0, 0); glVertex2f(0, lBitmap.rows);
                glTexCoord2d(0, lY); glVertex2f(0, 0);
                glTexCoord2d(lX, lY); glVertex2f(lBitmap.width, 0);
                glTexCoord2d(lX, 0); glVertex2f(lBitmap.width, lBitmap.rows);
            }
            glEnd();
        }
        glPopMatrix();
        glTranslatef(aFontFace->glyph->advance.x >> 6, 0, 0);
        glEndList();
        FT_Done_Glyph(lGlyph);
    }

    void FontData::init(const char * aFontPath,
                        unsigned int aFontHeight)
    {
        // Allocate some memory to store the texture ids.
        mTextures = new GLuint[NUM_TEXTURES];
        mCharacterWidths = new int[NUM_TEXTURES];

        this->mFontHeight = aFontHeight;

        // Create and initilize a FreeType font library.
        FT_Library lLibrary;
        if (FT_Init_FreeType(&lLibrary))
        {
            throw std::runtime_error("FT_Init_FreeType failed");
        }

        FT_Face lFontFace;
        if (FT_New_Face(lLibrary, aFontPath, 0, &lFontFace))
        {
            throw std::runtime_error("FT_New_Face failed");
        }

        // FreeType measures in 1/64th of a pixel
        FT_Set_Char_Size(lFontFace, aFontHeight << 6, aFontHeight << 6, 64, 64);

        mListBase = glGenLists(NUM_TEXTURES);
        glGenTextures(NUM_TEXTURES, mTextures);

        // This is where we actually create each of the fonts display lists.
        for(unsigned short i = 0; i < NUM_TEXTURES; i++)
        {
            makeDisplayList(lFontFace, i, mListBase, mTextures, this);
        }

        FT_Done_Face(lFontFace);
        FT_Done_FreeType(lLibrary);
    }

    void FontData::clean()
    {
        glDeleteLists(mListBase, NUM_TEXTURES);
        glDeleteTextures(NUM_TEXTURES,mTextures);
        delete [] mTextures;
        delete [] mCharacterWidths;
    }

    inline void pushScreenCoordinateMatrix()
    {
        glPushAttrib(GL_TRANSFORM_BIT);
        GLint	viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
        glPopAttrib();
    }

    void print(const FontData & aFontData,
               float aX,
               float aY,
               const std::wstring & aText)
    {
        GLuint lDisplayListBase = aFontData.mListBase;

        std::string lLine;
        for (UInt32 i = 0; i < aText.length(); i++)
        {
            lLine.append(1, aText[i]);
        }

        glPushMatrix();
        {
            glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
            glMatrixMode(GL_MODELVIEW);
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

            glListBase(lDisplayListBase);

            glTranslatef(aX, aY, 0);
            glCallLists(aText.length(), GL_UNSIGNED_BYTE, lLine.c_str());

            glPopAttrib();
        }
        glPopMatrix();
    }
}
