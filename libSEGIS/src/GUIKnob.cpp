//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIKnob: 
//      Implementation
//

#include <SEGIS/GUIElement.h>
#include <SEGIS/NumberOption.h>
#include <SEGIS/GUIKnob.h>
#include <SEGIS/DisplayManager.h>

GUIKnob::GUIKnob(UInt32      aID,
                 Rectangle2D aBounds)
    : GUIOptionElement(aID, aBounds)
    , mNumberOption(0)
    , mInitialUpdateDone(0)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
    , mQuadric(0)
{
    mPadding = 0;
    mBevel = 0;
    mPositionFraction = 0.5;
    mIsAnimatable = true;
    mHandleColorIsSet = false;
    mTickColor.r = mTickColor.g = mTickColor.b = 255;
}

GUIKnob::~GUIKnob()
{
}

Color
GUIKnob::getColor(void) 
{ 
    return mColor;
}

void 
GUIKnob::setColor(Color aColor)
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

Color 
GUIKnob::getHandleColor(void) 
{ 
    return mHandleColor;
}

void 
GUIKnob::setHandleColor(Color aHandleColor)
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

Color 
GUIKnob::getTickColor(void) 
{ 
    return mTickColor;
}
void 
GUIKnob::setTickColor(Color aColor)
{
    mTickColor.r = aColor.r;
    mTickColor.g = aColor.g;
    mTickColor.b = aColor.b;
}

EventSignal 
GUIKnob::handleKeyEvent(KeyEvent* aKeyEvent)
{
    if (kKeyTypeSpecial == aKeyEvent->mKeyType &&
        kKeyPressed == aKeyEvent->mKeyEventType &&
        mNumberOption)
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
    return kEventIgnored;
}

EventSignal 
GUIKnob::handleMouseEvent(MouseEvent* aMouseEvent)
{
    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_CYCLE);
    }

    if (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMouseOriginalPosition))
    {
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();

        const float kPi = 3.14159265f;
        const float k2Pi = 2.0f * kPi;

        // calculate a in what angle from the knob the mouse is now
        // and scale this to a fraction. then get option value for
        // this fraction.

        Float32 lDistX = Float32(aMouseEvent->mMousePosition.x - aMouseEvent->mMouseOriginalPosition.x);
        Float32 lDistY = Float32(aMouseEvent->mMousePosition.y - aMouseEvent->mMouseOriginalPosition.y);
        // only turn knob if mouse has moved a bit
        if (fabsf(lDistX) > 1.0f && fabsf(lDistY) > 1.0)
        {
            Float32 lAngle;
            if (lDistX < 0.0f)
            {
                lAngle = kPi + atan(lDistY / lDistX);
            }
            else
            {
                if (lDistY < 0.0f)
                {
                    lAngle = k2Pi + atan(lDistY / lDistX);
                }
                else
                {
                    lAngle = atan(lDistY / lDistX);
                }
            }

            lAngle += 0.5f * kPi;
            if (lAngle > k2Pi)
            {
                lAngle -= k2Pi;
            }

            // fprintf(stderr, "%.2f\n", lAngle * 360.0f / (k2Pi));
            Float32 lDelta = (k2Pi - lAngle) / k2Pi;

            mPositionFraction = max(0.0f, min(1.0f, lDelta));

            // set new value for option IF we have an option
            if (mNumberOption)
            {
                Float32 lOptionValue = fractionToOptionValue(mPositionFraction);
                mNumberOption->setValue(this, lOptionValue);
            }
        }

        lDisplayManager->requestUpdate(this);
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    else if ((kMouseDown == aMouseEvent->mMouseEventType || 
              kMouseUp == aMouseEvent->mMouseEventType) &&
            Util::inBounds(mBounds, aMouseEvent->mMousePosition))
    {
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    else
    {
        return kEventIgnored;
    }
}

Status 
GUIKnob::draw(void)
{
    Status lStatus = kSucceeded;

    if (!mInitialUpdateDone)
    {
        updateFromOption();
    }

    if (!mQuadric)
    {
        mQuadric = gluNewQuadric();
        if (!mQuadric)
        {
            fprintf(stderr, "gluNewQuadric failed\n");
            return kFailed;
        }
    }

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;
    int lHalfW = lBoxW / 2;

    // draw button
    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
    glTranslated(lHalfW, lHalfW, 0);
    // highlight
    glColor3ub(mHandleColorHighLight.r, mHandleColorHighLight.g, mHandleColorHighLight.b);
    gluPartialDisk(mQuadric, 
                   0, lHalfW,
                   16, 4,
                   -135.0, 180.0);
    // shadow
    glColor3ub(mHandleColorShadow.r, mHandleColorShadow.g, mHandleColorShadow.b);
    gluPartialDisk(mQuadric, 
                   0, lHalfW,
                   16, 4,
                   45.0, 180.0);

    glColor3ub(mHandleColor.r, mHandleColor.g, mHandleColor.b);
    gluDisk(mQuadric, 0, lHalfW-2, 16, 4);

    glTranslated(-lHalfW, -lHalfW, 0);
    glPopMatrix();

    #if 1
    // draw tick
    glPushMatrix();
    glRotatef(180.0f, 0, 0, -1);
    glRotatef(mPositionFraction * 360.0f, 0, 0, -1);
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
    glBegin(GL_LINES);
    {
        glColor3ub(mTickColor.r, mTickColor.b, mTickColor.b);
        glVertex2i(lBoxW/2, lBoxH/2);
        glVertex2i(lBoxW/2, lBoxH-2);
    }
    glEnd();
    glPopMatrix();
    #else
    // pizza slize
    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
    glTranslated(lHalfW, lHalfW, 0);
    // filled fraction of disk
    glColor3ub(mTickColor.r, mTickColor.g, mTickColor.b);
    gluPartialDisk(mQuadric, 
                   0, lHalfW-2,
                   16, 4,
                   -180.0, mPositionFraction * 360.0f);
    glTranslated(-lHalfW, -lHalfW, 0);
    glPopMatrix();
    #endif

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIKnob::draw - Drawing GUIKnob %d\n", mID);
    #endif
    
    return lStatus;
}

Float32 
GUIKnob::fractionToOptionValue(Float32 aFraction)
{
    if (!mNumberOption)
    {
        fprintf(stderr, "GUIKnob::fractionToOptionValue was called, but option is 0\n");
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
GUIKnob::optionValueToFraction(Float32 aOptionValue)
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
GUIKnob::setNumberOption(NumberOption* aNumberOption)
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
GUIKnob::updateFromOption()
{
    if (mNumberOption)
    {
        mPositionFraction = optionValueToFraction(mNumberOption->getValue());
        mInitialUpdateDone = true;
    }
}

// Observer interface
Status 
GUIKnob::update(const Observable* aObservable)
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
                    "GUIKnob::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr, 
                "GUIKnob::update - this slider has no option attached to it\n");
        return kFailed;
    }
}

bool
GUIKnob::isInTabOrder(void)
{
    return true;
}

bool 
GUIKnob::updateAnimation(void)
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
