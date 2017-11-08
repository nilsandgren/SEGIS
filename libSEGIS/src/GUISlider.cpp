//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUISlider: 
//      Implementation
//

#include <SEGIS/GUIElement.h>
#include <SEGIS/NumberOption.h>
#include <SEGIS/GUISlider.h>
#include <SEGIS/DisplayManager.h>

GUISlider::GUISlider(UInt32      aID,
                     Rectangle2D aBounds,
                     bool        aIsHorizontal)
    : GUIOptionElement(aID, aBounds)
    , mHandleColorIsSet(false)
    , mNumberOption(0)
    , mInitialUpdateDone(0)
    , mIsHorizontal(aIsHorizontal)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mPadding = 0;
    mBevel = 0;
    mSideSpacing = 4;
    if (mIsHorizontal)
    {
        mHandleWidth = 6;
        mHandleHeight = 12;
    }
    else
    {
        mHandleWidth = 12;
        mHandleHeight = 6;
    }
    mPositionFraction = 0.5;
    mIsAnimatable = true;
}

GUISlider::GUISlider(UInt32        aID,
                     Rectangle2D   aBounds,
                     UInt32        aSideSpacing,
                     bool          aIsHorizontal,
                     UInt32        aHandleWidth,
                     UInt32        aHandleHeight)
    : GUIOptionElement(aID, aBounds)
    , mHandleColorIsSet(false)
    , mNumberOption(0)
    , mInitialUpdateDone(0)
    , mIsHorizontal(aIsHorizontal)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mPadding = 0;
    mBevel = 0;
    mSideSpacing = aSideSpacing;
    mHandleWidth = aHandleWidth;
    mHandleHeight = aHandleHeight;
    mPositionFraction = 0.5f;
    mIsAnimatable = true;
}

GUISlider::~GUISlider()
{
}

bool 
GUISlider::getIsHorizontal(void)
{
    return mIsHorizontal;
}

void 
GUISlider::setIsHorizontal(bool aIsHorizontal)
{
    mIsHorizontal = aIsHorizontal;
}

EventSignal 
GUISlider::handleKeyEvent(KeyEvent* aKeyEvent)
{
    if (kKeyTypeSpecial == aKeyEvent->mKeyType &&
        kKeyPressed == aKeyEvent->mKeyEventType &&
        mNumberOption)
    {
        if (mIsHorizontal)
        {
            if (Keys::kLeft == aKeyEvent->mKey)
            {
                mNumberOption->decreaseValue(this);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kRight == aKeyEvent->mKey)
            {
                mNumberOption->increaseValue(this);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kUp == aKeyEvent->mKey)
            {
                Float32 lMinVal = mNumberOption->getMinValue();
                mNumberOption->setValue(this, lMinVal);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kDown == aKeyEvent->mKey)
            {
                Float32 lMaxVal = mNumberOption->getMaxValue();
                mNumberOption->setValue(this, lMaxVal);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else
        {
            if (Keys::kDown == aKeyEvent->mKey)
            {
                mNumberOption->decreaseValue(this);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kUp == aKeyEvent->mKey)
            {
                mNumberOption->increaseValue(this);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kLeft == aKeyEvent->mKey)
            {
                Float32 lMinVal = mNumberOption->getMinValue();
                mNumberOption->setValue(this, lMinVal);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kRight == aKeyEvent->mKey)
            {
                Float32 lMaxVal = mNumberOption->getMaxValue();
                mNumberOption->setValue(this, lMaxVal);
                updateFromOption();
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
    }
    return kEventIgnored;
}

EventSignal 
GUISlider::handleMouseEvent(MouseEvent* aMouseEvent)
{
    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        if (mIsHorizontal)
        {
            glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
        }
        else
        {
            glutSetCursor(GLUT_CURSOR_UP_DOWN);
        }
    }

    // current position in bounds while not dragging, or
    // original position in bounds while dragging
    if ((kMouseDragged != aMouseEvent->mMouseEventType &&
         Util::inBounds(mBounds, aMouseEvent->mMousePosition)) ||
        (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMouseOriginalPosition)))
    {
        if (kMouseDown == aMouseEvent->mMouseEventType ||
            kMouseDragged == aMouseEvent->mMouseEventType)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();

            if (mIsHorizontal)
            {
                // only care for x position
                SInt32 lDelta = aMouseEvent->mMousePosition.x - mBounds.x - mSideSpacing;
                mPositionFraction = Float32(lDelta) / Float32(mBounds.w - 2 * mSideSpacing);
                mPositionFraction = max(0.0f, min(1.0f, mPositionFraction));
            }
            else
            {
                // only care for y position
                SInt32 lDelta = aMouseEvent->mMousePosition.y - mBounds.y - mSideSpacing;
                mPositionFraction = Float32(lDelta) / Float32(mBounds.h - 2 * mSideSpacing);
                mPositionFraction = max(0.0f, min(1.0f, mPositionFraction));
            }

            // set new value for option IF we have an option
            if (mNumberOption)
            {
                Float32 lOptionValue = fractionToOptionValue(mPositionFraction);
                mNumberOption->setValue(this, lOptionValue);
            }

            lDisplayManager->requestUpdate(this);
        }
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    else
    {
        return kEventIgnored;
    }
}

Status 
GUISlider::draw(void)
{
    Status lStatus = kSucceeded;

    if (!mInitialUpdateDone)
    {
        updateFromOption();
    }

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    glBegin(GL_QUADS);
    {
        // draw main area
        glColor3ub(mColor.r, mColor.g, mColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, lBoxH);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);
    }
    glEnd();

    // slider gap
    if (mIsHorizontal)
    {
        glBegin(GL_LINES);
        {
            // draw slider gap
            UInt32 lGapY = lBoxH / 2;
            glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
            // left shadow
            glVertex2i(mSideSpacing - 1, lGapY - 1);
            glVertex2i(mSideSpacing - 1, lGapY + 1);
            // top shadow
            glVertex2i(mSideSpacing - 1, lGapY + 1);
            glVertex2i(lBoxW - (mSideSpacing - 1), lGapY + 1);
            // right highlight
            glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
            glVertex2i(lBoxW - (mSideSpacing - 1), lGapY + 1);
            glVertex2i(lBoxW - (mSideSpacing - 1), lGapY - 1);
            // bottom highlight
            glVertex2i(lBoxW - (mSideSpacing - 1), lGapY - 1);
            glVertex2i(mSideSpacing - 1, lGapY - 1);
            // black gap
            glColor3ub(0, 0, 0);
            glVertex2i(mSideSpacing, lGapY);
            glVertex2i(lBoxW - mSideSpacing, lGapY);
        }
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        {
            // draw slider gap
            UInt32 lGapX = lBoxW / 2;
            glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
            // left shadow
            glVertex2i(lGapX - 1, mSideSpacing);
            glVertex2i(lGapX - 1, lBoxH - mSideSpacing);
            // top shadow
            glVertex2i(lGapX - 1, lBoxH - mSideSpacing);
            glVertex2i(lGapX + 1, lBoxH - mSideSpacing);
            // right highlight
            glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
            glVertex2i(lGapX + 1, lBoxH - mSideSpacing);
            glVertex2i(lGapX + 1, mSideSpacing);
            // bottom highlight
            glVertex2i(lGapX - 1, mSideSpacing);
            glVertex2i(lGapX + 1, mSideSpacing);
            // black gap
            glColor3ub(0, 0, 0);
            glVertex2i(lGapX, mSideSpacing + 1);
            glVertex2i(lGapX, lBoxH - mSideSpacing);
        }
        glEnd();
    }
    glPopMatrix();

    // handle and bevel
    if (mIsHorizontal)
    {
        const UInt32 lHandlePosX = UInt32(mSideSpacing + 
                                          (lBoxW - 2 * mSideSpacing) * 
                                          mPositionFraction - mHandleWidth/2);
        const UInt32 lHandlePosY = lBoxH/2 - mHandleHeight/2;

        glPushMatrix();
        GraphicsUtil::setViewPort2D(lBoxPosX + lHandlePosX,
                                    lBoxPosY + lHandlePosY,
                                    mHandleWidth, 
                                    mHandleHeight);

        glBegin(GL_QUADS);
        {
            // handle main area and bevel
            glColor3ub(mHandleColor.r, mHandleColor.g, mHandleColor.b);
            glVertex2i(0, 0);
            glVertex2i(0, mHandleHeight);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth, 0);

            UInt32 lHandleBevel = 1;

            // draw handle bevel left
            glColor3ub(mHandleColorHighLight.r, mHandleColorHighLight.g, mHandleColorHighLight.b);
            glVertex2i(0, 0);
            glVertex2i(0, mHandleHeight);
            glVertex2i(lHandleBevel, mHandleHeight - lHandleBevel);
            glVertex2i(lHandleBevel, lHandleBevel);

            // draw handle bevel top
            glVertex2i(0, mHandleHeight);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth - lHandleBevel, mHandleHeight - lHandleBevel);
            glVertex2i(lHandleBevel, mHandleHeight - lHandleBevel);

            // draw handle bevel right
            glColor3ub(mHandleColorShadow.r, mHandleColorShadow.g, mHandleColorShadow.b);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth, 0);
            glVertex2i(mHandleWidth - lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth - lHandleBevel, mHandleHeight - lHandleBevel);

            // draw handle bevel bottom
            glVertex2i(0, 0);
            glVertex2i(lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth - lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth, 0);
        }
        glEnd();
        glPopMatrix();
    }
    else
    {
        const UInt32 lHandlePosY = UInt32(mSideSpacing + 
                                          (lBoxH - 2 * mSideSpacing) * 
                                          mPositionFraction - mHandleHeight/2);
        const UInt32 lHandlePosX = lBoxW/2 - mHandleWidth/2;

        glPushMatrix();
        GraphicsUtil::setViewPort2D(lBoxPosX + lHandlePosX,
                                    lBoxPosY + lHandlePosY,
                                    mHandleWidth, 
                                    mHandleHeight);

        glBegin(GL_QUADS);
        {
            // handle main area and bevel
            glColor3ub(mHandleColor.r, mHandleColor.g, mHandleColor.b);
            glVertex2i(0, 0);
            glVertex2i(0, mHandleHeight);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth, 0);

            UInt32 lHandleBevel = 1;

            // draw handle bevel left
            glColor3ub(mHandleColorHighLight.r, mHandleColorHighLight.g, mHandleColorHighLight.b);
            glVertex2i(0, 0);
            glVertex2i(0, mHandleHeight);
            glVertex2i(lHandleBevel, mHandleHeight - lHandleBevel);
            glVertex2i(lHandleBevel, lHandleBevel);

            // draw handle bevel top
            glVertex2i(0, mHandleHeight);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth - lHandleBevel, mHandleHeight - lHandleBevel);
            glVertex2i(lHandleBevel, mHandleHeight - lHandleBevel);

            // draw handle bevel right
            glColor3ub(mHandleColorShadow.r, mHandleColorShadow.g, mHandleColorShadow.b);
            glVertex2i(mHandleWidth, mHandleHeight);
            glVertex2i(mHandleWidth, 0);
            glVertex2i(mHandleWidth - lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth - lHandleBevel, mHandleHeight - lHandleBevel);

            // draw handle bevel bottom
            glVertex2i(0, 0);
            glVertex2i(lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth - lHandleBevel, lHandleBevel);
            glVertex2i(mHandleWidth, 0);
        }
        glEnd();
        glPopMatrix();
    }

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUISlider::draw - Drawing GUISlider %d\n", mID);
    #endif
    
    return lStatus;
}

Float32 
GUISlider::fractionToOptionValue(Float32 aFraction)
{
    if (!mNumberOption)
    {
        fprintf(stderr, "GUISlider::fractionToOptionValue was called, but option is 0\n");
        return 0.0f;
    }
    // assuming max > min
    Float32 lSpan = mNumberOption->getMaxValue() - mNumberOption->getMinValue();
    Float32 lOptionValue = aFraction * lSpan + mNumberOption->getMinValue();
    lOptionValue = mNumberOption->getValidValue(lOptionValue);
#   ifdef DEBUG_OPTIONS
    fprintf(stderr, "fractionToOptionValue: %f -> %f\n", aFraction, lOptionValue);
#   endif
    return lOptionValue;
}

Float32 
GUISlider::optionValueToFraction(Float32 aOptionValue)
{
    // assuming max > min
    Float32 lSpan = mNumberOption->getMaxValue() - mNumberOption->getMinValue();
    Float32 lFromMin = aOptionValue - mNumberOption->getMinValue();
    Float32 lFraction = lFromMin / lSpan;
#   ifdef DEBUG_OPTIONS
    fprintf(stderr, "optionValueToFraction: %f -> %f\n", aOptionValue, lFraction);
#   endif
    return lFraction;
}

Status 
GUISlider::setNumberOption(NumberOption* aNumberOption)
{
    // don't do anything if aNumberOption is 0
    if (!aNumberOption)
    {
        return kFailed;
    }

    // if we already have an Option we must tell it that
    // we're no longer observing it
    if (0 != mNumberOption)
    {
        mNumberOption->removeObserver(this);
    }
    mNumberOption = aNumberOption;
    // we're now observing this option
    mNumberOption->addObserver(this);
    return kSucceeded;
}

void 
GUISlider::updateFromOption()
{
    if (mNumberOption)
    {
        mPositionFraction = optionValueToFraction(mNumberOption->getValue());
        mInitialUpdateDone = true;
    }
}

// Observer interface
Status 
GUISlider::update(const Observable* aObservable)
{
    // make sure we have an option
    if (0 != mNumberOption)
    {
        // XXX this is a bit ugly, but we know this is a NumberOption 
        // since thats the only way to add an observable to this class
        NumberOption* lNumberOption = (NumberOption*) aObservable;

        // and if so, that the option to update from is the correct option
        if (mNumberOption == lNumberOption)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            // update our state from the option and request redisplay
            mPositionFraction = optionValueToFraction(lNumberOption->getValue());
            lDisplayManager->requestUpdate(this);
            return kSucceeded;
        }
        else
        {
            fprintf(stderr, 
                    "GUISlider::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr, 
                "GUISlider::update - this slider has no option attached to it\n");
        return kFailed;
    }
}

bool
GUISlider::isInTabOrder(void)
{
    return true;
}

bool 
GUISlider::updateAnimation(void)
{
    if (mIsAnimatable &&
        (mIsFocused || mPulseVal != 0))
    {
        mPulseVal += mPulseSign;

        mHandleColor.r += 2 * mPulseSign;
        mHandleColor.g += 2 * mPulseSign;
        mHandleColor.b += 2 * mPulseSign;

        setHandleColor(mHandleColor);

        if (mPulseVal == mPulseMax || mPulseVal == 0)
        {
            mPulseSign = -1 * mPulseSign;
        }
        return true;
    }
    return false;
}
