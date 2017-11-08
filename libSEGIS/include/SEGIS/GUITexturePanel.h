//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUITexturePanel: 
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

class Image;

class GUITexturePanel : public GUIElement
{
    // variables
    protected:
        SInt32      mTextureID;
        UInt32      mWidth;
        UInt32      mHeight;
        ColorFormat mColorFormat;
        MipMapMode  mMipMapMode;

    private:
        UInt32      mPropertiesAreSet;
        UInt32      mPropertiesAreApplied;
        UInt32      mIsAutoResize;
        Float32     mRotationAngle;
        UInt32      mAutoRotate;
        Image *     mImage;

    // functions
    public:
        GUITexturePanel(UInt32      aID,
                        Rectangle2D aBounds);

        GUITexturePanel(UInt32        aID,
                         Rectangle2D   aBounds,
                         UInt32        aPadding,
                         SizeConstraint aSizeConstraint,
                         FillDirection aFillDirection,
                         Alignment     aAlignment);

        virtual ~GUITexturePanel();

        virtual Status draw(void);

        // set texture properties
        virtual void setTextureProperties(UInt32      aWidth, 
                                          UInt32      aHeight,
                                          ColorFormat aColorFormat,
                                          MipMapMode  aMipMapMode);

        virtual void setAutoResize(UInt32 aIsAutoResize);

        virtual void setTextureData(UInt08* aImageData);

        virtual Status loadTextureData(Path & aImagePath);
    
        virtual bool isInTabOrder(void)
        {
            return false;   
        }

        virtual bool updateAnimation(void);
};
