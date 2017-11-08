//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIRadioButton: 
//      Implementation
//

#include <SEGIS/GUIOptionElement.h>
#include <SEGIS/GUIRadioButton.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/BooleanOption.h>

GUIRadioButton::GUIRadioButton(UInt32      aID,
                               Rectangle2D aBounds)
    : GUIOptionElement(aID, aBounds)
    , mIsChecked(false)
    , mBooleanOption(0)
    , mBoxSize(12)
    , mSynchedWithOption(false)
    , mRadioButtonGroup(0)
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

GUIRadioButton::~GUIRadioButton()
{
}

EventSignal 
GUIRadioButton::handleKeyEvent(KeyEvent* aKeyEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIRadioButton %d got KeyEvent\n", mID);
    #endif
    if (' ' == aKeyEvent->mKey || 13 == aKeyEvent->mKey) // space or enter
    {
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
        // NOTE only activation of a GUIRadioButton is done by event
        // the deactivation is handled by the RadioButtonGroup
        if (!mIsChecked)
        {
            mIsChecked = true;
            if (mBooleanOption)
            {
                mBooleanOption->setValue(this, mIsChecked);
            }
            if (mRadioButtonGroup)
            {
                mRadioButtonGroup->deactivateOthers(this);
            }
            lDisplayManager->requestUpdate(this);
        }
        aKeyEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    
    return kEventIgnored;
}
        
EventSignal 
GUIRadioButton::handleMouseEvent(MouseEvent* aMouseEvent)
{
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
	#ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIRadioButton %d got MouseEvent.\n", mID);
	#endif
    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_INFO);
    }

    // checked / unchecked state is changed on mouse down now
    // if click is within bounds of the checkbox
    // NOTE only activation of a GUIRadioButton is done by event
    // the deactivation is handled by the RadioButtonGroup
    if (Util::inBounds(mBoxBounds, aMouseEvent->mMousePosition))
    {
        if(kMouseDown == aMouseEvent->mMouseEventType)
        {
            if (!mIsChecked)
            {
                mIsChecked = true;
                if (mBooleanOption)
                {
                    mBooleanOption->setValue(this, mIsChecked);
                }
                if (mIsChecked && mRadioButtonGroup)
                {
                    mRadioButtonGroup->deactivateOthers(this);
                }
                lDisplayManager->requestUpdate(this);
                aMouseEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else if (kMouseUp == aMouseEvent->mMouseEventType)
        {
            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
    }
    return kEventIgnored;
}

void
GUIRadioButton::setBounds(Rectangle2D aBounds)
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
GUIRadioButton::setBoxSize(UInt32 aSize)
{
    mBoxSize = aSize;
    setBounds(mBounds); // update all coords
}

UInt32
GUIRadioButton::getBoxSize(void)
{
    return mBoxSize;
}

void
GUIRadioButton::setValue(bool aValue)
{
    mIsChecked = aValue;
}

void 
GUIRadioButton::setTickColor(Color aColor)
{
    mTickColor.r = aColor.r;
    mTickColor.g = aColor.g;
    mTickColor.b = aColor.b;
}

Color 
GUIRadioButton::getTickColor(void)
{
    return mTickColor;
}

Status 
GUIRadioButton::draw(void)
{
    Status lStatus = kSucceeded;

    if (!mSynchedWithOption)
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
                                mBoxSize,
                                mBoxSize);

    glBegin(GL_QUADS);
    {
        // draw main area
        glColor3ub(mColor.r, mColor.g, mColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, mBoxSize);
        glVertex2i(mBoxSize, mBoxSize);
        glVertex2i(mBoxSize, 0);

        UInt32 lBoxBevel = 1;

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

    // draw the tick
    if (mIsChecked)
    {
        glBegin(GL_LINES);
        {
            glColor3ub(mTickColor.r, mTickColor.g, mTickColor.b);

            glVertex2i(mBoxSize / 2 - 1, mBoxSize / 2 + 1);
            glVertex2i(mBoxSize / 2 + 1, mBoxSize / 2 + 1);

            glVertex2i(mBoxSize / 2 - 2, mBoxSize / 2 + 0);
            glVertex2i(mBoxSize / 2 + 2, mBoxSize / 2 + 0);

            glVertex2i(mBoxSize / 2 - 2, mBoxSize / 2 - 1);
            glVertex2i(mBoxSize / 2 + 2, mBoxSize / 2 - 1);

            glVertex2i(mBoxSize / 2 - 1, mBoxSize / 2 - 2);
            glVertex2i(mBoxSize / 2 + 1, mBoxSize / 2 - 2);
        }
        glEnd();
    }

    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIRadioButton::draw - Drawing GUIRadioButton %d\n", mID);
    #endif
    
    return lStatus;
}

Status
GUIRadioButton::setBooleanOption(BooleanOption* aBooleanOption)
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

BooleanOption*
GUIRadioButton::getBooleanOption(void)
{
    return mBooleanOption;
}

void
GUIRadioButton::updateFromOption()
{
    if (mBooleanOption)
    {
        setValue(mBooleanOption->getValue());
        mSynchedWithOption = true;
    }
}

// Observer interface
Status 
GUIRadioButton::update(const Observable* aObservable)
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
                    "GUIRadioButton::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr, 
                "GUIRadioButton::update - this radio button has no option attached to it\n");
        return kFailed;
    }
}

Status
GUIRadioButton::setGroup(RadioButtonGroup* aGroup)
{
    mRadioButtonGroup = aGroup;
    return kSucceeded;
}

bool
GUIRadioButton::isInTabOrder(void)
{
    return true;
}

bool 
GUIRadioButton::updateAnimation(void)
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

RadioButtonGroup::RadioButtonGroup()
{
}

RadioButtonGroup::~RadioButtonGroup()
{
}

Status 
RadioButtonGroup::addRadioButton(GUIRadioButton* aRadioButton)
{
    if (!aRadioButton)
    {
        return kFailed;
    }

    mRadioButtons.push_back(aRadioButton);
    aRadioButton->setGroup(this);
    return kSucceeded;
}

Status 
RadioButtonGroup::removeRadioButton(GUIRadioButton* aRadioButton)
{
    auto lChildIterator = mRadioButtons.begin();
    for (; lChildIterator != mRadioButtons.end(); ++lChildIterator)
    {
        if (*lChildIterator == aRadioButton)
        {
            mRadioButtons.erase(lChildIterator);
            return kSucceeded;
        }
    }
    return kFailed;
}

Status 
RadioButtonGroup::deactivateOthers(GUIRadioButton* aRadioButton)
{
    auto lChildIterator = mRadioButtons.begin();
    for (; lChildIterator != mRadioButtons.end(); ++lChildIterator)
    {
        if (*lChildIterator != aRadioButton)
        {
            BooleanOption* lOption = (*lChildIterator)->getBooleanOption();
            if (lOption)
            {
                lOption->setValue(aRadioButton, false);
            }
            else
            {
                (*lChildIterator)->setValue(false);
            }
        }
    }
    return kSucceeded;
}
