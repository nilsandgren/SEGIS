/*
 *  TextRenderer_Freetype.cpp
 *  SEGIS
 *
 *  Created by Nils Andgren on 22/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */


#include <SEGIS/TextRenderer_Freetype.h>

// TODO Remove this include
// #include <iostream>

TextRenderer_Freetype::TextRenderer_Freetype()
    : TextRenderer()
    , mFreetypeIsInitialized(0)
{
}

TextRenderer_Freetype::~TextRenderer_Freetype()
{
    mFontData.clean();
}

Status
TextRenderer_Freetype::init(const String & aFontPath, UInt32 aFontSize)
{
    if (!aFontSize)
    {
        fprintf(stderr, "TextRenderer_Freetype::init - Zero given as font size\n");
        return kFailed;
    }
    
    // store font path and font size
    //
    if (aFontPath.size())
    {
        mFontPath = aFontPath;
    }
    else
    {
        fprintf(stderr, "TextRenderer_Freetype::init - Empty string given as font path\n");
        return kFailed;
    }
    
    mFontSize = aFontSize;
    
    return kSucceeded;
}

Status
TextRenderer_Freetype::drawText(Float32 aX,
                                Float32 aY,
                                const WString & aText)
{
    if (!mFreetypeIsInitialized)
	{
		mFontData.init(mFontPath.c_str(), mFontSize);
		mFreetypeIsInitialized = true;
	}
    
	freetype::print(mFontData, aX, aY, aText);
    return kSucceeded;
}

UInt32
TextRenderer_Freetype::getCharacterWidth(int aCharacter)
{
    if (!mFreetypeIsInitialized)
	{
		mFontData.init(mFontPath.c_str(), mFontSize);
		mFreetypeIsInitialized = true;
	}
    
    UInt32 lVal = aCharacter;
    if (lVal > 1023)
    {
        fprintf(stderr, "TextRenderer_Freetype::getCharacterWidth - Warning: Clipping %d to 0-1023 range.\n", lVal);
        lVal = 1023;
    }
    if (lVal < 0)
    {
        fprintf(stderr, "TextRenderer_Freetype::getCharacterWidth - Warning: Clipping %d to 0-1023 range.\n", lVal);
        lVal = 0;
    }
    return mFontData.mCharacterWidths[lVal];
}

UInt32
TextRenderer_Freetype::getStringWidth(const WString & aString)
{
    if (!mFreetypeIsInitialized)
	{
		mFontData.init(mFontPath.c_str(), mFontSize);
		mFreetypeIsInitialized = true;
	}
    
    UInt32 lStringWidth = 0;
    WString::const_iterator lIter = aString.begin();
    for (; lIter != aString.end(); ++lIter)
    {
        lStringWidth += getCharacterWidth(*lIter);
    }
    
    return lStringWidth;
}
