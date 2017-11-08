/*
 *  TextElement.cpp
 *  SEGIS
 *
 *  Created by Nils Andgren on 21/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#include <SEGIS/TextElement.h>
#include <SEGIS/Util.h>

TextElement::TextElement()
    : mTextString(L"")
    , mRendererType(TextRenderer_Default)
{
}

TextElement::~TextElement()
{
}

Status
TextElement::setText(const WString & aTextString)
{
    mTextString = aTextString;
    return kSucceeded;
}

Status
TextElement::setTextRenderer(int aRendererType)
{
    switch (aRendererType)
    {
        case TextRenderer_Default:
        case TextRenderer_Small:
        case TextRenderer_Medium:
        case TextRenderer_Large:
        case TextRenderer_Huge:
        case TextRenderer_Pretty:
        {
            mRendererType = aRendererType;
            return kSucceeded;
        }
        default:
            return kFailed;
    }
}

Status
TextElement::drawText(Float32 aX, Float32 aY)
{
    FontController * lFontController = FontController::getFontController();
    TextRenderer* lTextRenderer = lFontController->getTextRenderer(mRendererType);
    
    if (lTextRenderer)
    {
        lTextRenderer->drawText(aX, aY, mTextString);
        return kSucceeded;
    }
    else
    {
        fprintf(stderr, "TextElement::drawText - Could not get default TextRenderer\n");
        return kFailed;
    }
}
