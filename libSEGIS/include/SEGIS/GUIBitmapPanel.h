//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIBitmapPanel: 
//      Subclass of GUIElement that displays a texture
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "Option.h"
#include "GUIElement.h"

#include <deque>
using namespace std;

class GUIBitmapPanel : public GUIElement
{
    // variables
    protected:
        SInt32      mWidth;
        SInt32      mHeight;
        ColorFormat mColorFormat;

    private:
        UInt32      mPropertiesAreSet;
        UInt08*     mYUVBuf;
        UInt08*     mRGBBuf;
        Float32     mZoomFactor; // <= 0 -> auto resize

    // functions
    public:
        GUIBitmapPanel(UInt32      aID,
                       Rectangle2D aBounds);

        GUIBitmapPanel(UInt32         aID,
                       Rectangle2D    aBounds,
                       UInt32         aPadding,
                       SizeConstraint aSizeConstraint,
                       FillDirection  aFillDirection,
                       Alignment      aAlignment);

        virtual ~GUIBitmapPanel();

        virtual Status draw(void);

        // set texture properties
        virtual void setBitmapProperties(UInt32      aWidth, 
                                         UInt32      aHeight,
                                         ColorFormat aColorFormat);

        virtual void setBitmapData(UInt08* aImageData);

        virtual void setZoomFactor(Float32 aZoomFactor);

        virtual bool isInTabOrder(void)
        {
            return false;   
        }

        virtual UInt32 getMinimumSafeWidth(void);

        virtual UInt32 getMinimumSafeHeight(void);
};
