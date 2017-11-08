//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  FontController: 
//      Handles the available text renderers.
//

#pragma once

#include "Types.h"
#include "TextRenderer.h"
#include <map>

using namespace std;

// Pre-defined text renderers
enum RendererType
{
    TextRenderer_Default,
    TextRenderer_Small,
    TextRenderer_Medium,
    TextRenderer_Large,
    TextRenderer_Huge,
    TextRenderer_Pretty
};

class FontController
{
    public:
        ~FontController();
    
        static FontController * getFontController();
        
        TextRenderer * getDefaultTextRenderer();
        TextRenderer * getTextRenderer(int aRendererType);
    
        Status addTextRenderer(TextRenderer * aTextRenderer, int aRendererType);
        Status removeTextRenderer(int aRendererType);
    
    private:
        FontController();
    
        static FontController * mTheFontController;
        map<int, TextRenderer*> mTextRenderers;
};
