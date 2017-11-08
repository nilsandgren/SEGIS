//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIMenuItem:
//      Implementation
//

#include <SEGIS/Util.h>
#include <SEGIS/GraphicsUtil.h>
#include <SEGIS/GUIMenuItem.h>
#include <SEGIS/Window.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/MainController.h>
#include <SEGIS/MenuController.h>

#define H_PAD 6
#define V_PAD 12

GUIMenuItem::GUIMenuItem(UInt32           aID,
                         Rectangle2D      aBounds,
                         const WString &  aString)
    : GUILabel(aID, aBounds, aString)
    , mIsExpanded(false)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
    , mRootMenuItem(0)
    , mParentMenuItem(0)
{
    mBevel = 1;
    mIsInTabOrder = false;
    mIsAnimatable = true;
    mChildPosition.x = 0;
    mChildPosition.y = 0;
    mLargestChildSize.x = 0;
    mLargestChildSize.y = 0;
    mSizeConstraint.w = kChangable;
    mSizeConstraint.h = kChangable;

    // change width to fit text
    mBounds.w = aBounds.w;
    mBounds.h = aBounds.h;

    mPadding = 4;
    
    TextElement::setText(aString);
}

GUIMenuItem::~GUIMenuItem()
{
    mSubMenuItems.clear();
}

void
GUIMenuItem::setFocus(bool aIsFocused)
{
    if (aIsFocused)
    {
        MenuController* lMenuController = MenuController::getMenuController();
        lMenuController->setFocusedMenuItem(this);
    }
    mIsFocused = aIsFocused;
}

void
GUIMenuItem::positionSubMenuItems()
{
    Rectangle2D lChildBounds = mBounds;
    if (mParentMenuItem)
    {
        lChildBounds.x += mBounds.w;
    }
    else
    {
        lChildBounds.y -= mBounds.h;
    }
    auto lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        (*lMenuItem)->setBounds(lChildBounds);
        lChildBounds.y -= mBounds.h;
    }
}

void
GUIMenuItem::resizeSubMenuItems()
{
    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);

    // resize root menu
    if (!mParentMenuItem)
    {
        mBounds.w = lTextRenderer->getStringWidth(mTextString) + H_PAD;
        mBounds.h = lTextRenderer->getFontHeight() + V_PAD;
    }

    UInt32 lMaxWidth = 0;
    UInt32 lTmpWidth = 0;
    auto lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        lTmpWidth = lTextRenderer->getStringWidth((*lMenuItem)->getText());
        if (lTmpWidth > lMaxWidth)
        {
            lMaxWidth = lTmpWidth;
        }
    }
    
    lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        (*lMenuItem)->mBounds.w = lMaxWidth + H_PAD;
        (*lMenuItem)->mBounds.h = lTextRenderer->getFontHeight() + V_PAD;
    }
    
#if 0 
    // recursive version
    lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        (*lMenuItem)->resizeSubMenuItems();
    }
#endif
}

void
GUIMenuItem::expandSubMenuItems()
{
    // size and position of sub-menu items should already have been set
    if (!mSubMenuItems.size())
    {
        return;
    }
    auto lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        (*lMenuItem)->setVisible(true);
    }
    mIsExpanded = true; // next level is expanded
}

void
GUIMenuItem::collapseSubMenuItems()
{
    auto lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        (*lMenuItem)->setVisible(false);
        (*lMenuItem)->collapseSubMenuItems();
    }
    mIsExpanded = false; // next level not expanded
}

deque< GUIMenuItem* > *
GUIMenuItem::getSubMenuItems(void)
{
    return &mSubMenuItems;
}


bool
GUIMenuItem::isExpanded(void)
{
    return mIsExpanded;
}

Status
GUIMenuItem::addChild(GUIMenuItem* aMenuItem)
{
    if (!aMenuItem)
    {
        return kBadArgument;
    }
    mSubMenuItems.push_back(aMenuItem);
    aMenuItem->setParentMenuItem(this);
    aMenuItem->setVisible(false);
    aMenuItem->setParentWindow(mParentWindow);
    if (!mParentMenuItem) // this is the root
    {
        aMenuItem->setRootMenuItem(this);
    }
    else
    {
        aMenuItem->setRootMenuItem(mRootMenuItem);
    }
    return kSucceeded;
}

Status
GUIMenuItem::addSubMenuItem(GUIMenuItem* aMenuItem)
{
    return addChild(aMenuItem);
}

Status
GUIMenuItem::removeSubMenuItem(GUIMenuItem* aMenuItem)
{
    auto lMenuItem = mSubMenuItems.begin();
    for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
    {
        if (*lMenuItem == aMenuItem)
        {
            mSubMenuItems.erase(lMenuItem);
            return kSucceeded;
        }
    }
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIMenuItem::removeSubMenuItem - GUIMenuItem not found\n");
    #endif
    return kFailed;
}

Status
GUIMenuItem::doAction(void)
{
    return kSucceeded;
}

Status
GUIMenuItem::draw(void)
{
    if (mIsVisible)
    {
        resizeSubMenuItems();
        positionSubMenuItems();
        
        int lBoxPosX = mBounds.x;
        int lBoxPosY = mBounds.y;
        int lBoxW = mBounds.w;
        int lBoxH = mBounds.h;
        int lCorner = 5;

        if (mIsExpanded)
        {
            lCorner = 0;
        }

        // draw main area if not root
        if (mRootMenuItem)
        {
            glPushMatrix();
            GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

            glBegin(GL_POLYGON);
            {
                glColor3ub(mColor.r, mColor.g, mColor.b);
                glVertex2i(0, 0);
                glVertex2i(0, lBoxH);
                glVertex2i(lBoxW - lCorner, lBoxH);
                glVertex2i(lBoxW, lBoxH - lCorner);
                glVertex2i(lBoxW, 0);
            }
            glEnd();

            glBegin(GL_QUADS);
            {
                // draw bevel
                Color lColor1;
                Color lColor2;
                if (mBevel < 0)
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
                // draw bevel left
                glColor3ub(lColor1.r, lColor1.g, lColor1.b);
                glVertex2i(0, 0);
                glVertex2i(0, lBoxH);
                glVertex2i(mBevel, lBoxH - mBevel);
                glVertex2i(mBevel, mBevel);

                // draw bevel top
                glVertex2i(0, lBoxH);
                glVertex2i(lBoxW - lCorner, lBoxH);
                glVertex2i(lBoxW - lCorner - mBevel, lBoxH - mBevel);
                glVertex2i(mBevel, lBoxH - mBevel);

                // draw bevel right
                glColor3ub(lColor2.r, lColor2.g, lColor2.b);
                glVertex2i(lBoxW, lBoxH - lCorner);
                glVertex2i(lBoxW, 0);
                glVertex2i(lBoxW - mBevel, mBevel);
                glVertex2i(lBoxW - mBevel, lBoxH - lCorner - mBevel);

                // draw bevel bottom
                glVertex2i(0, 0);
                glVertex2i(mBevel, mBevel);
                glVertex2i(lBoxW - mBevel, mBevel);
                glVertex2i(lBoxW, 0);
            }
            glEnd();
            glPopMatrix();
        }

        GUILabel::draw();

        auto lMenuItem = mSubMenuItems.begin();
        for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
        {
            (*lMenuItem)->draw();
        }
    }
    return kSucceeded;
}

void
GUIMenuItem::setBounds(Rectangle2D aBounds)
{
    // we don't change size for menu items, only position
    mBounds.x = aBounds.x;
    mBounds.y = aBounds.y;
    positionSubMenuItems();
}

bool
GUIMenuItem::updateAnimation(void)
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

EventSignal
GUIMenuItem::handleKeyEvent(KeyEvent* /* aKeyEvent */)
{
#   if 0
    if (kKeyTypeSpecial == aKeyEvent->mKeyType &&
        kKeyPressed == aKeyEvent->mKeyEventType)
    {
        if (Keys::kLeft == aKeyEvent->mKey)
        {
            if (mParentMenuItem)
            {
                mParentMenuItem->collapseSubMenuItems();
                mParentMenuItem->setFocus(true);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else if (Keys::kRight == aKeyEvent->mKey)
        {
            if (mSubMenuItems.size())
            {
                expandSubMenuItems();
                mSubMenuItems[0]->setFocus(true);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        /*
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
        */
    }
#   endif
    return kEventIgnored;
}

void
GUIMenuItem::setVisible(bool aIsVisible)
{
    if(mParentWindow && mRootMenuItem)
    {
        if (aIsVisible && aIsVisible != mIsVisible)
        {
            mParentWindow->addOverlayElement(this);
        }
        else if (aIsVisible != mIsVisible)
        {
            mParentWindow->removeOverlayElement(this);
        }
    }
    mIsVisible = aIsVisible;
}

EventSignal
GUIMenuItem::handleMouseEvent(MouseEvent* aMouseEvent)
{
    if (aMouseEvent->mAcceptor)
    {
        return kEventAbsorbed;
    }
    if (mIsVisible)
    {
        if (Util::inBounds(mBounds, aMouseEvent->mMousePosition))
        {
            glutSetCursor(GLUT_CURSOR_INFO);
            if (kMouseUp == aMouseEvent->mMouseEventType)
            {
                // this is a leaf item
                if (0 == mSubMenuItems.size())
                {
                    mRootMenuItem->collapseSubMenuItems();
                    doAction();
                    aMouseEvent->mAcceptor = this;
                    return kEventAbsorbed;
                }
                // this is the menu root
                else if (!mRootMenuItem)
                {
                    if (mIsExpanded)
                    {
                        collapseSubMenuItems();
                    }
                    else
                    {
                        expandSubMenuItems();
                    }
                    aMouseEvent->mAcceptor = this;
                    return kEventAbsorbed;
                }
            }
            // we have a root
            else if (mRootMenuItem)
            {
                aMouseEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else if (mIsExpanded)
        {
            // see if sub-menus wants event
            auto lMenuItem = mSubMenuItems.begin();
            for (; lMenuItem != mSubMenuItems.end(); ++lMenuItem)
            {
                (*lMenuItem)->handleMouseEvent(aMouseEvent);
                if (aMouseEvent->mAcceptor)
                {
                    return kEventAbsorbed;
                }
            }
        }
    }
    return kEventIgnored;
}
