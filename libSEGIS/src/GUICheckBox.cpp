//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUICheckBox: 
//      Implementation
//

#include <SEGIS/GUIOptionElement.h>
#include <SEGIS/GUICheckBox.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/BooleanOption.h>

GUICheckBox::GUICheckBox(UInt32      aID,
                         Rectangle2D aBounds)
    : GUIOptionElement(aID, aBounds)
    , mIsChecked(false)
    , mBooleanOption(0)
    , mBoxSize(12)
    , mInitialUpdateDone(false)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mPadding = 2;
    mBevel = 1;
    mTickColor.r = 255;
    mTickColor.g = 255;
    mTickColor.b = 255;
    mIsAnimatable = true;
}

GUICheckBox::~GUICheckBox()
{
}

EventSignal 
GUICheckBox::handleKeyEvent(KeyEvent* aKeyEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUICheckBox %d got KeyEvent\n", mID);
    #endif
    if (kKeyPressed == aKeyEvent->mKeyEventType && 
       (' ' == aKeyEvent->mKey || 13 == aKeyEvent->mKey)) // space or enter
    {
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
        mIsChecked = !mIsChecked;
        if (mBooleanOption)
        {
            mBooleanOption->setValue(this, mIsChecked);
        }
        lDisplayManager->requestUpdate(this);
        aKeyEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    
    return kEventIgnored;
}
        
EventSignal 
GUICheckBox::handleMouseEvent(MouseEvent* aMouseEvent)
{
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUICheckBox %d got MouseEvent.\n", mID);
    #endif
    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_INFO);
    }
    if (Util::inBounds(mBoxBounds, aMouseEvent->mMousePosition))
    {
        if (kMouseUp == aMouseEvent->mMouseEventType)
        {
            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        else if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            // checked / unchecked state is changed on mouse down now
            // if click is within bounds of the checkbox
            mIsChecked = !mIsChecked;
            if (mBooleanOption)
            {
                mBooleanOption->setValue(this, mIsChecked);
            }
            aMouseEvent->mAcceptor = this;
            lDisplayManager->requestUpdate(this);
            return kEventAbsorbed;
        }
    }
    return kEventIgnored;
}

void
GUICheckBox::setBounds(Rectangle2D aBounds)
{
    mBounds.x = aBounds.x;
    mBounds.y = aBounds.y;
    mBounds.w = aBounds.w;
    mBounds.h = aBounds.h;
    mBoxBounds.x = mBounds.x + mBounds.w / 2 - mBoxSize / 2;
    mBoxBounds.y = mBounds.y + mBounds.h / 2 - mBoxSize / 2;
    mBoxBounds.w = mBoxSize;
    mBoxBounds.h = mBoxSize;
}

void
GUICheckBox::setBoxSize(UInt32 aSize)
{
    mBoxSize = aSize;
    setBounds(mBounds); // update all coords
}

UInt32
GUICheckBox::getBoxSize(void)
{
    return mBoxSize;
}

void
GUICheckBox::setValue(bool aValue)
{
    mIsChecked = aValue;
}

void 
GUICheckBox::setTickColor(Color aColor)
{
    mTickColor.r = aColor.r;
    mTickColor.g = aColor.g;
    mTickColor.b = aColor.b;
}

Color 
GUICheckBox::getTickColor(void)
{
    return mTickColor;
}

Status 
GUICheckBox::draw(void)
{
    Status lStatus = kSucceeded;

    if (!mInitialUpdateDone)
    {
        updateFromOption();
    }

    int lBoundsPosX = mBounds.x;
    int lBoundsPosY = mBounds.y;
    int lBoundsW = mBounds.w;
    int lBoundsH = mBounds.h;

    // TODO perhaps care about alignment, but the box is not really a child
    // center box in bounds
    int lBoxX = lBoundsW / 2 - mBoxSize / 2;
    int lBoxY = lBoundsH / 2 - mBoxSize / 2;

    // set viewport/ortho for the checkbox only
    glPushMatrix();

    GraphicsUtil::setViewPort2D(lBoundsPosX + lBoxX,
                                lBoundsPosY + lBoxY,
                                mBoxSize, mBoxSize);

    UInt32 lBoxBevel = 1;
    glBegin(GL_QUADS);
    {
        // draw main area
        glColor3ub(mColor.r, mColor.g, mColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, mBoxSize);
        glVertex2i(mBoxSize, mBoxSize);
        glVertex2i(mBoxSize, 0);

        // draw outer bevel left
        glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
        glVertex2i(0, 0);
        glVertex2i(0, mBoxSize);
        glVertex2i(lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(lBoxBevel, lBoxBevel);

        // draw outer bevel top
        glVertex2i(0, mBoxSize);
        glVertex2i(mBoxSize, mBoxSize);
        glVertex2i(mBoxSize - lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(lBoxBevel, mBoxSize - lBoxBevel);

        // draw outer bevel right
        glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
        glVertex2i(mBoxSize, mBoxSize);
        glVertex2i(mBoxSize, 0);
        glVertex2i(mBoxSize - lBoxBevel, lBoxBevel);
        glVertex2i(mBoxSize - lBoxBevel, mBoxSize - lBoxBevel);

        // draw outer bevel bottom
        glVertex2i(0, 0);
        glVertex2i(lBoxBevel, lBoxBevel);
        glVertex2i(mBoxSize - lBoxBevel, lBoxBevel);
        glVertex2i(mBoxSize, 0);

        // draw inner bevel left
        glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
        glVertex2i(lBoxBevel, lBoxBevel);
        glVertex2i(lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(2 * lBoxBevel, mBoxSize - 2 * lBoxBevel);
        glVertex2i(2 * lBoxBevel, 2 * lBoxBevel);

        // draw inner bevel top
        glVertex2i(lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(mBoxSize + lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(mBoxSize + 2 * lBoxBevel, mBoxSize - 2 * lBoxBevel);
        glVertex2i(2 * lBoxBevel, mBoxSize - 2 * lBoxBevel);

        // draw inner bevel right
        glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
        glVertex2i(mBoxSize - lBoxBevel, mBoxSize - lBoxBevel);
        glVertex2i(mBoxSize - lBoxBevel, lBoxBevel);
        glVertex2i(mBoxSize - 2 * lBoxBevel, 2 * lBoxBevel);
        glVertex2i(mBoxSize - 2 * lBoxBevel, mBoxSize - 2 * lBoxBevel);

        // draw inner bevel bottom
        glVertex2i(lBoxBevel, lBoxBevel);
        glVertex2i(2 * lBoxBevel, 2 * lBoxBevel);
        glVertex2i(mBoxSize - 2 * lBoxBevel, 2 * lBoxBevel);
        glVertex2i(mBoxSize - lBoxBevel, lBoxBevel);
    }
    glEnd();

    // tick mark
    if (mIsChecked)
    {
        glBegin(GL_LINES);
        {
            // draw the tick
            glColor3ub(mTickColor.r, mTickColor.g, mTickColor.b);

            glVertex2i(3 * lBoxBevel, mBoxSize - 3 * lBoxBevel);
            glVertex2i(mBoxSize - 3 * lBoxBevel, 3 * lBoxBevel);

            glVertex2i(3 * lBoxBevel, 3 * lBoxBevel);
            glVertex2i(mBoxSize - 3 * lBoxBevel, mBoxSize - 3 * lBoxBevel);
        }
        glEnd();
    }

    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUICheckBox::draw - Drawing GUICheckBox %d\n", mID);
    #endif
    
    return lStatus;
}

Status
GUICheckBox::setBooleanOption(BooleanOption* aBooleanOption)
{
    // don't do anything if aBooleanOption is 0
    if (!aBooleanOption)
    {
        return kFailed;
    }

    // if we already have an Option we must tell it that
    // we're no longer observing it
    if (0 != mBooleanOption)
    {
        mBooleanOption->removeObserver(this);
    }
    mBooleanOption = aBooleanOption;

    // we're now observing this option
    mBooleanOption->addObserver(this);
    return kSucceeded;
}

void
GUICheckBox::updateFromOption()
{
    if (mBooleanOption)
    {
        setValue(mBooleanOption->getValue());
        mInitialUpdateDone = true;
    }
}

// Observer interface
Status 
GUICheckBox::update(const Observable* aObservable)
{
    // make sure we have an option
    if (0 != mBooleanOption)
    {
        // XXX this is a bit ugly, but we know this is a BooleanOption 
        // since thats the only way to add an observable to this class
        BooleanOption* lBooleanOption = (BooleanOption*) aObservable;

        // and if so, that the option to update from is the correct option
        if (mBooleanOption == lBooleanOption)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            // update our state from the option and request redisplay
            setValue(mBooleanOption->getValue());
            lDisplayManager->requestUpdate(this);
            return kSucceeded;
        }
        else
        {
            fprintf(stderr, 
                    "GUICheckBox::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr, 
                "GUICheckBox::update - this checkbox has no option attached to it\n");
        return kFailed;
    }
}

bool
GUICheckBox::isInTabOrder(void)
{
    return true;
}

bool 
GUICheckBox::updateAnimation(void)
{
    if (mIsAnimatable &&
        (mIsFocused || mPulseVal != 0))
    {
        mPulseVal += mPulseSign;

        mColor.r += 2 * mPulseSign;
        mColor.g += 2 * mPulseSign;
        mColor.b += 2 * mPulseSign;

        setColor(mColor);

        if (mPulseVal == mPulseMax || mPulseVal == 0)
        {
            mPulseSign = -1 * mPulseSign;
        }
        return true;
    }
    return false;
}
