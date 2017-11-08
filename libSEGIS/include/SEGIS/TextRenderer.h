/*
 *  TextRenderer.h
 *  SEGIS
 *
 *  Created by Nils Andgren on 22/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#pragma once

#include "Types.h"

class TextRenderer
{
    public:
        TextRenderer()
            : mFontSize(0)
        {
        }

        virtual ~TextRenderer()
        {
        }

        virtual Status drawText(Float32 aX,
                                Float32 aY,
                                const WString & aText) = 0;
        virtual Status init(const String & aFontPath, UInt32 aFontSize) = 0;
        virtual UInt32 getStringWidth(const WString & aString) = 0;
        virtual UInt32 getCharacterWidth(int aCharacter) = 0;
        virtual UInt32 getFontHeight() { return mFontSize; }

    protected:
        String mFontPath;
        UInt32 mFontSize;
};
