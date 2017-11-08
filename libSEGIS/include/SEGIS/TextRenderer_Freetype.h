/*
 *  FreetypeTextRenderer.h
 *  SEGIS
 *
 *  Created by Nils Andgren on 22/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#pragma once

#include "TextRenderer.h"
#include "FreetypeWrapper.h"

class TextRenderer_Freetype : public TextRenderer
{
    public:
        TextRenderer_Freetype();
        virtual ~TextRenderer_Freetype();

        virtual Status init(const String & aFontPath,
                            UInt32 aFontSize);
        virtual Status drawText(Float32 aX,
                                Float32 aY,
                                const WString & aText);

        virtual UInt32 getStringWidth(const WString & aString);
        virtual UInt32 getCharacterWidth(int aCharacter);
    
    protected:
        freetype::FontData mFontData;
        bool mFreetypeIsInitialized;
};
