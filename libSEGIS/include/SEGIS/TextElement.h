/*
 *  TextElement.h
 *  SEGIS
 *
 *  Created by Nils Andgren on 21/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#pragma once

#include "Types.h"
#include "FontController.h"
#include "TextRenderer.h"

class TextElement
{
    public:
        TextElement();
        virtual ~TextElement();
    
        virtual Status setText(const WString & aTextString);
        virtual const WString & getText() { return mTextString; }
        
        Status drawText(Float32 aX, Float32 aY);
        Status setTextRenderer(int aRendererType);
   
    protected:
        WString mTextString;
        int    mRendererType;
};
