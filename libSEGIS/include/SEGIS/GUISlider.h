//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUISlider: 
//      Subclass to GUIElement which can observe an Option
//
//       -------------------------------------
//      | -----------------[]---------------- |
//       -------------------------------------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "Observer.h"
#include <deque>

using namespace std;

class NumberOption;

class GUISlider : public GUIOptionElement, public Observer
{
    // variables
    protected:
        // num pixels on each side of the slider
        UInt32        mSideSpacing;              

        // width of the slider handle - keep this even
        UInt32        mHandleWidth;

        // height of the slider handle - keep this even
        UInt32        mHandleHeight;             

        // position of slider 0.0 to 1.0
        Float32       mPositionFraction;         

        // defaults to mColor
        Color         mHandleColor;              

        // mHandleColor - mDarkDiff
        Color         mHandleColorShadow;        

        // mHandleColor + mBrightDiff
        Color         mHandleColorHighLight;     

        // do not default to mColor any more
        bool          mHandleColorIsSet;         

        // option attached to the slider
        NumberOption* mNumberOption;             

        bool          mInitialUpdateDone;

        // determines wether this is a horizontal or a vertical slider.
        // defaults to true.
        bool          mIsHorizontal;             

        // for animation
        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;
        
    // functions
    private:
        // transforms between the slider position and attached option's value
        // floating point value is used since it covers all number options
        Float32 fractionToOptionValue(Float32 aFraction);
        Float32 optionValueToFraction(Float32 aOptionValue);
    
    public:
        GUISlider(UInt32      aID,
                  Rectangle2D aBounds,
                  bool        aIsHorizontal);

        GUISlider(UInt32      aID,
                  Rectangle2D aBounds,
                  UInt32      aSideSpacing,
                  bool        aIsHorizontal,
                  UInt32      aHandleWidth,
                  UInt32      aHandleHeight);

        virtual ~GUISlider();

        // get/set
        UInt32 getSideSpacing(void)
        {
            return mSideSpacing;
        }
        void setSideSpacing(UInt32 aSideSpacing)
        {
            mSideSpacing = aSideSpacing;
        }

        UInt32 getHandleWidth(void)
        {
            return mHandleWidth;
        }
        void setHandleWidth(UInt32 aHandleWidth)
        {
            mHandleWidth = aHandleWidth;
        }

        UInt32 getHandleHeight(void)
        {
            return mHandleHeight;
        }
        void setHandleHeight(UInt32 aHandleHeight)
        {
            mHandleHeight = aHandleHeight;
        }

        UInt32 getPadding(void) 
        { 
            return mPadding;
        }
        virtual void setPadding(UInt32 /*aPadding*/ )
        {
            // padding not supported for sliders right now
            // too much coordinate hassle
            mPadding = 0;
        }

        bool getIsHorizontal(void);
        void setIsHorizontal(bool aIsHorizontal);

        Color getColor(void) 
        { 
            return mColor;
        }
        virtual void setColor(Color aColor)
        {
            mColor.r = aColor.r;
            mColor.g = aColor.g;
            mColor.b = aColor.b;
            mColorShadow.r = max(0, int(mColor.r * mDarkDiff));
            mColorShadow.g = max(0, int(mColor.g * mDarkDiff));
            mColorShadow.b = max(0, int(mColor.b * mDarkDiff));
            mColorHighLight.r = min(255, int(mColor.r * mBrightDiff));
            mColorHighLight.g = min(255, int(mColor.g * mBrightDiff));
            mColorHighLight.b = min(255, int(mColor.b * mBrightDiff));

            // set handle color to this color if handle color is not set
            if (!mHandleColorIsSet)
            {
                mHandleColor.r = aColor.r;
                mHandleColor.g = aColor.g;
                mHandleColor.b = aColor.b;
                mHandleColorShadow.r = max(0, int(mHandleColor.r * mDarkDiff));
                mHandleColorShadow.g = max(0, int(mHandleColor.g * mDarkDiff));
                mHandleColorShadow.b = max(0, int(mHandleColor.b * mDarkDiff));
                mHandleColorHighLight.r = min(255, int(mHandleColor.r * mBrightDiff));
                mHandleColorHighLight.g = min(255, int(mHandleColor.g * mBrightDiff));
                mHandleColorHighLight.b = min(255, int(mHandleColor.b * mBrightDiff));
            }
        }

        Color getHandleColor(void) 
        { 
            return mHandleColor;
        }
        virtual void setHandleColor(Color aHandleColor)
        {
            mHandleColorIsSet = true;
            mHandleColor.r = aHandleColor.r;
            mHandleColor.g = aHandleColor.g;
            mHandleColor.b = aHandleColor.b;
            mHandleColorShadow.r = max(0, int(mHandleColor.r * mDarkDiff));
            mHandleColorShadow.g = max(0, int(mHandleColor.g * mDarkDiff));
            mHandleColorShadow.b = max(0, int(mHandleColor.b * mDarkDiff));
            mHandleColorHighLight.r = min(255, int(mHandleColor.r * mBrightDiff));
            mHandleColorHighLight.g = min(255, int(mHandleColor.g * mBrightDiff));
            mHandleColorHighLight.b = min(255, int(mHandleColor.b * mBrightDiff));
        }

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUISlider
        virtual Status draw(void);

        // attach a NumberOption to the slider
        virtual Status setNumberOption(NumberOption* aNumberOption);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a NumberOption
        virtual Status update(const Observable* aOption);

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);
};
