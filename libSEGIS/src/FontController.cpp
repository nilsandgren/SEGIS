/*
 *  FontController.cpp
 *  SEGIS
 *
 *  Created by Nils Andgren on 22/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */

#include <SEGIS/FontController.h>
#include <SEGIS/TextRenderer_Freetype.h>

FontController *
FontController::mTheFontController = 0;

FontController::FontController()
{
    String lFontPath("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
    // Set up text renderers and store in mTextRenderers
    TextRenderer_Freetype * lTextRenderer_Default = new TextRenderer_Freetype();
    lTextRenderer_Default->init(lFontPath, 12);

    TextRenderer_Freetype * lTextRenderer_Small = new TextRenderer_Freetype();
    lTextRenderer_Small->init(lFontPath, 12);

    TextRenderer_Freetype * lTextRenderer_Medium = new TextRenderer_Freetype();
    lTextRenderer_Medium->init(lFontPath, 14);

    TextRenderer_Freetype * lTextRenderer_Large = new TextRenderer_Freetype();
    lTextRenderer_Large->init(lFontPath, 16);

    TextRenderer_Freetype * lTextRenderer_Huge = new TextRenderer_Freetype();
    lTextRenderer_Huge->init(lFontPath, 24);

    TextRenderer_Freetype * lTextRenderer_Pretty = new TextRenderer_Freetype();
    lTextRenderer_Pretty->init(lFontPath, 48);

    mTextRenderers[TextRenderer_Default] = lTextRenderer_Default;
    mTextRenderers[TextRenderer_Small] = lTextRenderer_Small;
    mTextRenderers[TextRenderer_Medium] = lTextRenderer_Medium;
    mTextRenderers[TextRenderer_Large] = lTextRenderer_Large;
    mTextRenderers[TextRenderer_Huge] = lTextRenderer_Huge;
    mTextRenderers[TextRenderer_Pretty] = lTextRenderer_Pretty;
}

FontController::~FontController()
{
    auto lTextRendererIterator = mTextRenderers.begin();
    for (; lTextRendererIterator != mTextRenderers.end(); ++lTextRendererIterator)
    {
        delete lTextRendererIterator->second;
    }
    mTextRenderers.clear();
}

FontController *
FontController::getFontController()
{
    if (!mTheFontController)
    {
        mTheFontController = new FontController();
    }
    return mTheFontController;
    
}
    
TextRenderer *
FontController::getDefaultTextRenderer()
{
    return getTextRenderer(TextRenderer_Default);
}

TextRenderer *
FontController::getTextRenderer(int aRendererType)
{
    return mTextRenderers[aRendererType];
}
    
Status
FontController::addTextRenderer(TextRenderer * aTextRenderer, int aRendererType)
{
    if (!aTextRenderer)
    {
        fprintf(stderr, "FontController::addTextRenderer - Null pointer given as TextRenderer");
        return kFailed;
    }
    
    // make sure RendererType is available
    if (mTextRenderers.find(aRendererType) != mTextRenderers.end())
    {
        // RendererType value already taken
        fprintf(stderr, "FontController::addTextRenderer - RendererType %d already taken\n", aRendererType);
        return kFailed;
    }
    
    mTextRenderers[aRendererType] = aTextRenderer;
    return kSucceeded;
}

Status
FontController::removeTextRenderer(int aRendererType)
{
    mTextRenderers.erase(aRendererType);
    return kSucceeded;
}
