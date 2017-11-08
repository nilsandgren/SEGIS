//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIStepButtonPair
//      Implementation
//

#include <SEGIS/GUIOptionElement.h>
#include <SEGIS/GUIStepButtonPair.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/BooleanOption.h>

GUIStepButtonPair::GUIStepButtonPair(UInt32        aID,
                                     FillDirection aFillDirection)
	: mNumberOption(0)
    , mButtonSize(8)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mID = aID;
    mFillDirection = aFillDirection;
    mIsAnimatable = true;
    
    Rectangle2D lButtonBounds;
    lButtonBounds.x = 0;
    lButtonBounds.y = 0;
    lButtonBounds.w = mButtonSize;
    lButtonBounds.h = mButtonSize;

    SizeConstraint lConstraint;
    lConstraint.w = kLocked;
    lConstraint.h = kLocked;
    setSizeConstraint(lConstraint); // self is not resizable either

    Alignment lAlignment;
    lAlignment.mHAlign = kLeft;
    lAlignment.mVAlign = kTop;
    setAlignment(lAlignment);

    mPadding = 0;
    mBevel = 0;

    mBounds.x = 0;
    mBounds.y = 0;

    mButtons[0] = new GUIStepButton(0, lButtonBounds, 1, lConstraint, aFillDirection, lAlignment);
    mButtons[1] = new GUIStepButton(1, lButtonBounds, 1, lConstraint, aFillDirection, lAlignment);
    mButtons[0]->setBevel(1);
    mButtons[1]->setBevel(1);

    if (kFillDirectionHorizontal == aFillDirection)
    {
        mBounds.w = 2 * mButtonSize;
        mBounds.h = mButtonSize;
        addChild(mButtons[0]);
        addChild(mButtons[1]);
        mButtons[0]->setDirection(kDirectionLeft);
        mButtons[1]->setDirection(kDirectionRight);
    }
    else
    {
        mBounds.h = 2 * mButtonSize;
        mBounds.w = mButtonSize;
        addChild(mButtons[1]); // intentional!
        addChild(mButtons[0]);
        mButtons[1]->setDirection(kDirectionUp);
        mButtons[0]->setDirection(kDirectionDown);
    }

    mArrowColor.r = 255;
    mArrowColor.g = 255;
    mArrowColor.b = 255;

    mButtons[0]->setLabelColor(mArrowColor);
    mButtons[1]->setLabelColor(mArrowColor);
}

GUIStepButtonPair::~GUIStepButtonPair()
{
    if (mButtons[0])
    {
        delete mButtons[0];
        mButtons[0] = 0;
    }
    if (mButtons[1])
    {
        delete mButtons[1];
        mButtons[1] = 0;
    }
}

EventSignal 
GUIStepButtonPair::handleKeyEvent(KeyEvent* aKeyEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIStepButtonPair %d got KeyEvent\n", mID);
    #endif
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();

    if (kKeyTypeSpecial == aKeyEvent->mKeyType &&
        kKeyPressed == aKeyEvent->mKeyEventType &&
        mNumberOption)
    {
        if (Keys::kLeft == aKeyEvent->mKey || 
            Keys::kDown == aKeyEvent->mKey)
        {
            mNumberOption->decreaseValue(this);
            mButtons[0]->setPressed(true);
            updateFromOption();
            lDisplayManager->requestUpdate(this);
            aKeyEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        else if (Keys::kRight == aKeyEvent->mKey ||
                 Keys::kUp == aKeyEvent->mKey)
        {
            mNumberOption->increaseValue(this);
            mButtons[1]->setPressed(true);
            updateFromOption();
            lDisplayManager->requestUpdate(this);
            aKeyEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
    }
    mButtons[0]->setPressed(false);
    mButtons[1]->setPressed(false);
    lDisplayManager->requestUpdate(this);
    return kEventIgnored;
}
        
EventSignal 
GUIStepButtonPair::handleMouseEvent(MouseEvent* aMouseEvent)
{
#   ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIStepButtonPair %d got MouseEvent.\n", mID);
#   endif

    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_INFO);
    }

    // test if any of the buttons absorbs the event
    if (kMouseDragged != aMouseEvent->mMouseEventType &&
        Util::inBounds(mButtons[0]->getBounds(), aMouseEvent->mMousePosition))
    {
        if (mNumberOption && 
            kMouseDown == aMouseEvent->mMouseEventType)
        {
            // first button -> decrease value
            mNumberOption->decreaseValue(this);
        }
        if (kMouseUp == aMouseEvent->mMouseEventType)
        {
            mButtons[0]->setPressed(false);
        }
        else if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            mButtons[0]->setPressed(true);
        }
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
        lDisplayManager->requestUpdate(this);

        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    else if (kMouseDragged != aMouseEvent->mMouseEventType &&
             Util::inBounds(mButtons[1]->getBounds(), aMouseEvent->mMousePosition))
    {
        if (mNumberOption && 
            kMouseDown == aMouseEvent->mMouseEventType)
        {
            // first button -> decrease value
            mNumberOption->increaseValue(this);
        }
        if (kMouseUp == aMouseEvent->mMouseEventType)
        {
            mButtons[1]->setPressed(false);
        }
        else if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            mButtons[1]->setPressed(true);
        }
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
        lDisplayManager->requestUpdate(this);

        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    return kEventIgnored;
}

Status 
GUIStepButtonPair::draw(void)
{
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIStepButtonPair::draw - Drawing GUIStepButtonPair %d\n", mID);
    #endif
    // DisplayManager will make sure the 2 children are drawn alright
    return kSucceeded;
}

void
GUIStepButtonPair::setBounds(Rectangle2D aBounds)
{
    mBounds.x = aBounds.x;
    mBounds.y = aBounds.y;
    mBounds.w = aBounds.w;
    mBounds.h = aBounds.h;
}

void
GUIStepButtonPair::setBevel(SInt32 aBevel)
{
    SInt32 lBevel = abs(aBevel);
    mBevel = min(mPadding, lBevel);
}

Color
GUIStepButtonPair::getColor(void)
{
    return mColor;
}

void
GUIStepButtonPair::setColor(Color aColor)
{
    GUIOptionElement::setColor(aColor);

    if (mButtons[0])
    {
        mButtons[0]->setColor(aColor);
    }
    if (mButtons[1])
    {
        mButtons[1]->setColor(aColor);
    }
}

Status
GUIStepButtonPair::setNumberOption(NumberOption* aNumberOption)
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
GUIStepButtonPair::setArrowColor(Color aColor)
{
    mArrowColor.r = aColor.r;
    mArrowColor.g = aColor.g;
    mArrowColor.b = aColor.b;
    if (mButtons[0])
    {
        mButtons[0]->setLabelColor(mArrowColor);
    }
    if (mButtons[1])
    {
        mButtons[1]->setLabelColor(mArrowColor);
    }
}

Color
GUIStepButtonPair::getArrowColor(void)
{
    return mArrowColor;
}

bool
GUIStepButtonPair::isInTabOrder(void)
{
    return true;
}

bool
GUIStepButtonPair::updateAnimation(void)
{
    if (mIsAnimatable &&
        (mIsFocused || mPulseVal != 0))
    {
        mPulseVal += mPulseSign;

        mColor.r += 2 * mPulseSign;
        mColor.g += 2 * mPulseSign;
        mColor.b += 2 * mPulseSign;

        mButtons[0]->setColor(mColor);
        mButtons[1]->setColor(mColor);

        if (mPulseVal == mPulseMax || mPulseVal == 0)
        {
            mPulseSign = -1 * mPulseSign;
        }
        return true;
    }
    return false;
}

Status
GUIStepButtonPair::addChild(GUIStepButton* aStepButton)
{
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mChildren.push_back(aStepButton);
    }
    else
    {
        mChildren.push_front(aStepButton);
    }
    aStepButton->setParent(this);
#   ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIStepButtonPair addChild\n");
#   endif
    return kSucceeded;
}
