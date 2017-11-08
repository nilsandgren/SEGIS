//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIButton:
//      Implementation
//

#include <SEGIS/GUIButton.h>
#include <SEGIS/DisplayManager.h>

GUIButton::GUIButton(UInt32          aID,
                     Rectangle2D     aBounds,
                     const WString & aLabelText)
    : GUIElement(aID, aBounds)
    , TextElement()
    , mIsPressed(false)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mPadding = 2;
    mBevel = 1;
    mLabelColor.r = 255;
    mLabelColor.g = 255;
    mLabelColor.b = 255;
    mIsInTabOrder = true;
    mIsAnimatable = true;
    
    setText(aLabelText);
}

GUIButton::GUIButton(UInt32        aID,
                     Rectangle2D   aBounds,
                     UInt32        aPadding,
                     SizeConstraint aSizeConstraint,
                     FillDirection aFillDirection,
                     Alignment     aAlignment)
    : GUIElement(aID,
                 aBounds,
                 aPadding,
                 aSizeConstraint,
                 aFillDirection,
                 aAlignment)
    , TextElement()
    , mIsPressed(false)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    mPadding = aPadding;
    mBevel = 1;
    mLabelColor.r = 255;
    mLabelColor.g = 255;
    mLabelColor.b = 255;
    mIsInTabOrder = true;
    mIsAnimatable = true;
}

GUIButton::GUIButton()
     : GUIButton(0, {0, 0, 80, 20}, L"")
{
    // Rectangle2D lBounds;
    // lBounds.w = 80;
    // lBounds.h = 20;
    // GUIButton::GUIButton(0, lBounds, L"");
}

GUIButton::~GUIButton()
{
}

void
GUIButton::setBevel(SInt32 aBevel)
{
    SInt32 lBevel = abs(aBevel);
    mBevel = min(mPadding, lBevel);
}

void
GUIButton::setFocus(bool aIsFocused)
{
    if (!aIsFocused)
    {
        mIsPressed = false;
    }
    GUIElement::setFocus(aIsFocused);
}

EventSignal
GUIButton::handleKeyEvent(KeyEvent* aKeyEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIButton %d got KeyEvent\n", mID);
    #endif

    // tmp debug
    if(kKeyTypeNormal == aKeyEvent->mKeyType &&
       (' ' == aKeyEvent->mKey || 13 == aKeyEvent->mKey)) // space or enter
    {
        DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
        if (kKeyPressed == aKeyEvent->mKeyEventType)
        {
            mIsPressed = true;
            aKeyEvent->mAcceptor = this;
            lDisplayManager->requestUpdate(this);
            return kEventAbsorbed;
        }
        else if (kKeyReleased == aKeyEvent->mKeyEventType)
        {
            mIsPressed = false;
            aKeyEvent->mAcceptor = this;
            lDisplayManager->requestUpdate(this);
            doAction();
            return kEventAbsorbed;
        }
    }
    return kEventIgnored;
}

EventSignal
GUIButton::handleMouseEvent(MouseEvent* aMouseEvent)
{
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIButton %d got MouseEvent.\n", mID);
    #endif

    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_INFO);
    }

    if ((kMouseDragged != aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMousePosition))
        ||
        (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMouseOriginalPosition)))
    {
        if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            mIsPressed = true;
            lDisplayManager->requestUpdate(this);
        }
        else if (kMouseUp == aMouseEvent->mMouseEventType &&
                 mIsPressed)
        {
            mIsPressed = false;
            //  if mouse position is in bounds of this button when
            //  mouse is released, some kind of action should be taken
            doAction();
            lDisplayManager->requestUpdate(this);
        }
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    return kEventIgnored;
}

Status
GUIButton::draw(void)
{
    Status lStatus = kSucceeded;
    Color lColor1;
    Color lColor2;

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIButton::draw - Drawing GUIButton %d\n", mID);
    #endif

    if (mIsPressed)
    {
        // top and left will be shadowed
        // right and bottom will be highlighted
        lColor1 = mColorShadow;
        lColor2 = mColorHighLight;
    }
    else
    {
        // top and left will be highlighted
        // right and bottom will be shadowed
        lColor1 = mColorHighLight;
        lColor2 = mColorShadow;
    }

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    {
        GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

        glBegin(GL_QUADS);
        {
            // draw main area
            glColor3ub(mColor.r, mColor.g, mColor.b);
            glVertex2i(0, 0);
            glVertex2i(0, lBoxH);
            glVertex2i(lBoxW, lBoxH);
            glVertex2i(lBoxW, 0);

            // TODO The vertex calculations can be optimized
            // draw bevel left
            glColor3ub(lColor1.r, lColor1.g, lColor1.b);
            glVertex2i(0, 0);
            glVertex2i(0, lBoxH);
            glVertex2i(mBevel, lBoxH - mBevel);
            glVertex2i(mBevel, mBevel);

            // draw bevel top
            glVertex2i(0, lBoxH);
            glVertex2i(lBoxW, lBoxH);
            glVertex2i(lBoxW - mBevel, lBoxH - mBevel);
            glVertex2i(mBevel, lBoxH - mBevel);

            // draw bevel right
            glColor3ub(lColor2.r, lColor2.g, lColor2.b);
            glVertex2i(lBoxW, lBoxH);
            glVertex2i(lBoxW, 0);
            glVertex2i(lBoxW - mBevel, mBevel);
            glVertex2i(lBoxW - mBevel, lBoxH - mBevel);

            // draw bevel bottom
            glVertex2i(0, 0);
            glVertex2i(mBevel, mBevel);
            glVertex2i(lBoxW - mBevel, mBevel);
            glVertex2i(lBoxW, 0);
        }
        glEnd();

        // draw mLabelText
        // if (mLabelText)
        {
            UInt32 lPressEffect = 0;
            if (mIsPressed)
            {
                lPressEffect = mBevel;
            }
            glColor3ub(mLabelColor.r, mLabelColor.g, mLabelColor.b);
            TextElement::drawText(mBevel + 5 + lPressEffect,
                                  (mBounds.h >> 1) - 4 - lPressEffect);
        }
    }
    glPopMatrix();

    return lStatus;
}

void
GUIButton::setLabelColor(Color aColor)
{
    mLabelColor.r = aColor.r;
    mLabelColor.g = aColor.g;
    mLabelColor.b = aColor.b;
}

Color
GUIButton::getLabelColor(void)
{
    return mLabelColor;
}

Status
GUIButton::doAction(void)
{
    return kSucceeded;
}

void
GUIButton::setPressed(bool aIsPressed)
{
    mIsPressed = aIsPressed;
}

bool
GUIButton::isInTabOrder(void)
{
    return mIsInTabOrder;
}

bool
GUIButton::updateAnimation(void)
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
