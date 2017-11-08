//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIElement: 
//      Implementation
//

#include <SEGIS/GUIElement.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/MainController.h>
#include <SEGIS/Window.h>
#include <SEGIS/Types.h>

Color GUIElement::kDefaultFocusColor = { 255, 0, 0 };

GUIElement::GUIElement()
    : mChildren()
    , mIsFocused(false)
{
    mID = 0;
    mBounds.x = 0;
    mBounds.y = 0;
    mBounds.w = 100;
    mBounds.h = 100;
    mBevel = 0;
    mPadding = 0;

    mBrightDiff = 1.70;
    mDarkDiff = 0.30;
    mColor.r = 127;
    mColor.g = 127;
    mColor.b = 127;
    mColorShadow.r = max(0, int(mColor.r * mDarkDiff));
    mColorShadow.g = max(0, int(mColor.g * mDarkDiff));
    mColorShadow.b = max(0, int(mColor.b * mDarkDiff));
    mColorHighLight.r = min(255, int(mColor.r * mBrightDiff));
    mColorHighLight.g = min(255, int(mColor.g * mBrightDiff));
    mColorHighLight.b = min(255, int(mColor.b * mBrightDiff));

    mFocusColor.r = 120;
    mFocusColor.g = 120;
    mFocusColor.b = 120;

    mSizeConstraint.w = kAuto;
    mSizeConstraint.h = kAuto;
    mFillDirection = kFillDirectionHorizontal;
    mAlignment.mHAlign = kLeft;
    mAlignment.mVAlign = kTop;
    mParent = 0;

    mIsVisible = true;
    mIsAnimatable = false;
    mIsInTabOrder = false;

    mParentWindow = 0;
}

GUIElement::GUIElement(UInt32      aID,
                       Rectangle2D aBounds)
    : mChildren()
    , mIsFocused(false)
{
    mID = aID;
    mBounds.x = aBounds.x;
    mBounds.y = aBounds.y;
    mBounds.w = aBounds.w;
    mBounds.h = aBounds.h;
    mBevel = 0; // flat
    mPadding = 0;

    mBrightDiff = 1.70;
    mDarkDiff = 0.30;

    mColor.r = 127;
    mColor.g = 127;
    mColor.b = 127;
    mColorShadow.r = max(0, int(mColor.r * mDarkDiff));
    mColorShadow.g = max(0, int(mColor.g * mDarkDiff));
    mColorShadow.b = max(0, int(mColor.b * mDarkDiff));
    mColorHighLight.r = min(255, int(mColor.r * mBrightDiff));
    mColorHighLight.g = min(255, int(mColor.g * mBrightDiff));
    mColorHighLight.b = min(255, int(mColor.b * mBrightDiff));

    mFocusColor.r = 120;
    mFocusColor.g = 120;
    mFocusColor.b = 120;

    mSizeConstraint.w = kAuto;
    mSizeConstraint.h = kAuto;
    mFillDirection = kFillDirectionHorizontal;
    mAlignment.mHAlign = kLeft;
    mAlignment.mVAlign = kTop;
    mParent = 0;

    mIsVisible = true;
    mIsAnimatable = false;
    mIsInTabOrder = false;

    mParentWindow = 0;
}

GUIElement::GUIElement(UInt32        aID,
                       Rectangle2D   aBounds,
                       UInt32        aPadding,
                       SizeConstraint aSizeConstraint,
                       FillDirection aFillDirection,
                       Alignment     aAlignment)
    : mChildren()
    , mIsFocused(false)
{
    mID = aID;
    mBounds.x = aBounds.x;
    mBounds.y = aBounds.y;
    mBounds.w = aBounds.w;
    mBounds.h = aBounds.h;
    mBevel = 0;
    mPadding = aPadding;
    mColor.r = 127;
    mColor.g = 127;
    mColor.b = 127;

    mBrightDiff = 1.70;
    mDarkDiff = 0.30;

    mColor.r = 127;
    mColor.g = 127;
    mColor.b = 127;
    mColorShadow.r = max(0, int(mColor.r * mDarkDiff));
    mColorShadow.g = max(0, int(mColor.g * mDarkDiff));
    mColorShadow.b = max(0, int(mColor.b * mDarkDiff));
    mColorHighLight.r = min(255, int(mColor.r * mBrightDiff));
    mColorHighLight.g = min(255, int(mColor.g * mBrightDiff));
    mColorHighLight.b = min(255, int(mColor.b * mBrightDiff));

    mFocusColor.r = 120;
    mFocusColor.g = 120;
    mFocusColor.b = 120;

    mSizeConstraint.w = aSizeConstraint.w;
    mSizeConstraint.h = aSizeConstraint.w;
    mFillDirection = aFillDirection;
    mAlignment.mHAlign = aAlignment.mHAlign;
    mAlignment.mVAlign = aAlignment.mVAlign;
    mParent = 0;

    mIsVisible = true;
    mIsAnimatable = false;
    mIsInTabOrder = false;

    mParentWindow = 0;
}

GUIElement::~GUIElement()
{
    mChildren.clear();
}

void
GUIElement::setColor(Color aColor)
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
}

void 
GUIElement::setFocus(bool aIsFocused)
{
    mIsFocused = aIsFocused;
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    if (aIsFocused)
    {
        lDisplayManager->requestUpdate(this);
    }
    else
    {
        // we request update of the whole window
        MainController* lMainController = MainController::getMainController();
        lDisplayManager->requestUpdate(lMainController->getCurrentWindow()->getRootGUIElement());
    }
}

void
GUIElement::setFocusColor(Color aColor, bool aRecurse)
{ 
    mFocusColor.r = aColor.r;
    mFocusColor.g = aColor.g;
    mFocusColor.b = aColor.b;

    if (aRecurse)
    {
        auto lChild = mChildren.begin();
        for (; lChild != mChildren.end(); ++lChild)
        {
            (*lChild)->setFocusColor(aColor, true);
        }
    }
}


void 
GUIElement::setVisible(bool aIsVisible)
{
    mIsVisible = aIsVisible;
}

void
GUIElement::setBevel(SInt32 aBevel)
{ 
    if (aBevel < 0)
    {
        mBevel = max(-1*(SInt32(mPadding)), aBevel);
    }
    else
    {
        mBevel = min((SInt32)mPadding, aBevel);
    }
}

EventSignal 
GUIElement::handleKeyEvent(KeyEvent*)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIElement %d got KeyEvent\n", mID);
    #endif
    return kEventIgnored;
}
        
EventSignal 
GUIElement::handleMouseEvent(MouseEvent* aMouseEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIElement %d got MouseEvent. Ignored.\n", mID);
    #endif
    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
    return kEventIgnored;
}

Status 
GUIElement::draw(void)
{
    Status lStatus = kSucceeded;

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();

    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    GraphicsUtil::drawQuad(0, 0, 0, lBoxH, lBoxW, lBoxH, lBoxW, 0, mColor);

    Color lColor1;
    Color lColor2;
    if (mBevel <= 0)
    {
        // top and left will be shadowed
        // right and bottom will be highlighted
        lColor1 = mColorShadow;
        lColor2 = mColorHighLight;
    }
    else if (mBevel > 0)
    {
        // top and left will be highlighted
        // right and bottom will be shadowed
        lColor1 = mColorHighLight;
        lColor2 = mColorShadow;
    }

    UInt32 lBevel = abs(mBevel);
    if (mBevel)
    {
        // draw bevel left
        GraphicsUtil::drawQuad(0, 0, 0, lBoxH, lBevel, lBoxH - lBevel, 
                               lBevel, lBevel, lColor1);

        // draw bevel top
        GraphicsUtil::drawQuad(0, lBoxH, lBoxW, lBoxH, lBoxW - lBevel, 
                               lBoxH - lBevel, lBevel, lBoxH - lBevel);

        // draw bevel right
        GraphicsUtil::drawQuad(lBoxW, lBoxH, lBoxW, 0, lBoxW - lBevel, 
                               lBevel, lBoxW - lBevel, lBoxH - lBevel, 
                               lColor2);

        // draw bevel bottom
        GraphicsUtil::drawQuad(0, 0, lBevel, lBevel, lBoxW - lBevel,
                               lBevel, lBoxW, 0);
    }
    
    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement::draw - Drawing GUIElement %d\n", mID);
    #endif
    
    return lStatus;
}

Status 
GUIElement::drawTree(void)
{
    // sanity check
    if (!fitsInParent())
    {
#       ifdef DEBUG_BOUNDS
        fprintf(stderr, "GUIElement::drawTree - element %d does not fit parent\n", mID);
        fprintf(stderr, "GUIElement::drawTree - w, h : %d, %d\n", (int)mBounds.w, (int)mBounds.h);
#       endif
        return kFailed;    
    }

    Status lStatus = kSucceeded;
    if (mIsVisible)
    {
        lStatus = draw();
    }
    if (kSucceeded != lStatus)
    {
        // stop drawing this part of the interface 
        // when a child fails to draw
        return lStatus;
    }

#   ifdef DEBUG_BOUNDS
    drawFocusBorder();
#   else
    if (isFocused())
    {
        drawFocusBorder();
    }
#   endif

    auto lChild = mChildren.begin();
    for (; lChild != mChildren.end(); ++lChild)
    {
        lStatus = (*lChild)->drawTree();
    }
    return lStatus;
}

Status 
GUIElement::drawFocusBorder(void)
{
    Status lStatus = kSucceeded;

    int lBoxPosX = mBounds.x - 2;
    int lBoxPosY = mBounds.y - 2;
    int lBoxW = mBounds.w + 3;
    int lBoxH = mBounds.h + 3;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    glBegin(GL_LINE_LOOP);
    {
        glColor3ub(mFocusColor.r, mFocusColor.g, mFocusColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, lBoxH);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);
    }

    glEnd();
    glPopMatrix();
    
    return lStatus;
}

Status 
GUIElement::addChild(GUIElement* aGUIElement)
{
    if (!aGUIElement)
    {
        return kBadArgument;
    }

    if (!mParentWindow)
    {
        fprintf(stderr, "error: GUIElement %d (%p) has no parent window.\n", aGUIElement->getID(), (void*)aGUIElement);
        fprintf(stderr, "       parent window can be set with with setParentWindow\n");
        fprintf(stderr, "       or\n");
        fprintf(stderr, "       use addChild(Window*, GUIElement*)\n");
        return kBadArgument;
    }
    else
    {
        aGUIElement->setParentWindow(mParentWindow);
        aGUIElement->setFocusColor(mParentWindow->getFocusColor(), 1);
    }
    
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mChildren.push_back(aGUIElement);
    }
    else
    {
        mChildren.push_front(aGUIElement);
    }
    aGUIElement->setParent(this);
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement addChild\n");
    #endif
    return kSucceeded;
}

Status 
GUIElement::addChildBack(GUIElement* aGUIElement)
{
    if (!aGUIElement)
    {
        return kBadArgument;
    }

    if (!mParentWindow)
    {
        fprintf(stderr, "error: GUIElement %d (%p) has no parent window.\n", aGUIElement->getID(), (void*)aGUIElement);
        fprintf(stderr, "       parent window can be set with with setParentWindow\n");
        fprintf(stderr, "       or\n");
        fprintf(stderr, "       use addChild(Window*, GUIElement*)\n");
        return kBadArgument;
    }
    else
    {
        aGUIElement->setParentWindow(mParentWindow);
    }
    
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mChildren.push_front(aGUIElement);
    }
    else
    {
        mChildren.push_back(aGUIElement);
    }
    aGUIElement->setParent(this);
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement addChild\n");
    #endif
    return kSucceeded;
}

Status 
GUIElement::addChild(Window* aParentWindow, GUIElement* aGUIElement)
{
    if (!aGUIElement || !aParentWindow)
    {
        return kBadArgument;
    }

    aGUIElement->setParentWindow(aParentWindow);
    
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mChildren.push_back(aGUIElement);
    }
    else
    {
        mChildren.push_front(aGUIElement);
    }
    aGUIElement->setParent(this);
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement addChild\n");
    #endif
    return kSucceeded;
}

Status 
GUIElement::removeChild(GUIElement* aGUIElement)
{
    auto lChildIterator = mChildren.begin();
    for (; lChildIterator != mChildren.end(); ++lChildIterator)
    {
        if (*lChildIterator == aGUIElement)
        {
            mChildren.erase(lChildIterator);
            return kSucceeded;
        }
    }
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement::removeChild - GUIElement not found\n");
    #endif
    return kFailed;
}

Status
GUIElement::removeTree(GUIElement* aRoot)
{
    auto lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        removeTree(*lChild);
    }
    fprintf(stderr, "deleting GUIElement %d\n", aRoot->getID());
    delete aRoot;
    return kSucceeded;
}

bool
GUIElement::fitsInParent()
{
    if (mParent)
    {
        // position is not taken in account
        SInt32 lParentPadding = (SInt32)(mParent->getPadding());
        if (mParent->getBounds().w < mBounds.w + (lParentPadding<<1) ||
            mParent->getBounds().h < mBounds.h + (lParentPadding<<1))
        {
#           ifdef DEBUG_BOUNDS
            fprintf(stderr, 
                    "GUIElement::fitsInParent - %ld < %ld\n", 
                    mParent->getBounds().w,
                    mBounds.w + (lParentPadding<<1));
            fprintf(stderr, 
                    "GUIElement::fitsInParent - or\n");
            fprintf(stderr, 
                    "GUIElement::fitsInParent - %ld < %ld\n", 
                    mParent->getBounds().h,
                    mBounds.h + (lParentPadding<<1));
#           endif
            return false;
        }
    }
    // if GUIElement has no parent it's considered to fit or else nothing
    // would be drawn
    return true;
}

UInt32
GUIElement::getMinimumSafeWidth(void)
{
    if (kLocked == mSizeConstraint.w)
    {
        return mBounds.w;
    }
    else 
    {
        if (kFillDirectionVertical == mFillDirection)
        {
            // minimum width is width of widest child
            UInt32 lWidth = 0;
            UInt32 lTmp = 0;
            auto lChild = mChildren.begin();
            for (; lChild != mChildren.end(); ++lChild)
            {
                lTmp = (*lChild)->getMinimumSafeWidth();
                if (lTmp > lWidth)
                {
                    lWidth = lTmp;
                }
            }
            return lWidth + (mPadding << 1);
        }
        else
        {
            // minimum width is sum of childrens width
            UInt32 lWidth = 0;
            auto lChild = mChildren.begin();
            for (; lChild != mChildren.end(); ++lChild)
            {
                lWidth += (*lChild)->getMinimumSafeWidth();
            }
            return lWidth + mChildren.size() * (mPadding << 1);
        }
    }
}

UInt32
GUIElement::getMinimumSafeHeight(void)
{
    if (kLocked == mSizeConstraint.h)
    {
        return mBounds.h;
    }
    else
    {
        if (kFillDirectionHorizontal == mFillDirection)
        {
            // minimum height is height of tallest child
            UInt32 lHeight = 0;
            UInt32 lTmp = 0;
            auto lChild = mChildren.begin();
            for (; lChild != mChildren.end(); ++lChild)
            {
                lTmp = (*lChild)->getMinimumSafeHeight();
                if (lTmp > lHeight)
                {
                    lHeight = lTmp;
                }
            }
            return lHeight + (mPadding << 1);
        }
        else
        {
            // minimum height is sum of childrens height
            UInt32 lHeight = 0;
            auto lChild = mChildren.begin();
            for (; lChild != mChildren.end(); ++lChild)
            {
                lHeight += (*lChild)->getMinimumSafeHeight();
            }
            return lHeight + mChildren.size() * (mPadding << 1);
        }
    }
}

bool
GUIElement::isInTabOrder(void)
{
    // base class is not included in tab order
    return false;
}

