//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIComboBox: 
//      Implementation
//

#include <SEGIS/BooleanOption.h>
#include <SEGIS/GUIComboBox.h>
#include <SEGIS/GUIComboBoxItem.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/Window.h>

GUIComboBox::GUIComboBox(UInt32      aID,
                         Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
    , mSelectedComboBoxItem(0)
    , mFocusedComboBoxItem(0)
    , mSelectedIndex(0)
    , mFocusedIndex(0)
    , mIsExpanded(false)
{
    mLargestChildSize.w = 0;
    mLargestChildSize.h = 0;
    mLargestChildSize.x = aBounds.x;
    mLargestChildSize.y = aBounds.y;
    mSizeConstraint.w = kLocked;
    mSizeConstraint.h = kLocked;
    mIsAnimatable = true;
    mTickColor.r = mTickColor.g = mTickColor.b = 255;
}

GUIComboBox::GUIComboBox(UInt32         aID,
                         Rectangle2D    aBounds,
                         UInt32         aPadding,
                         SizeConstraint aSizeConstraint,
                         FillDirection  aFillDirection,
                         Alignment      aAlignment)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mSelectedComboBoxItem(0)
    , mFocusedComboBoxItem(0)
    , mSelectedIndex(0)
    , mFocusedIndex(0)
    , mIsExpanded(false)
{
    mSizeConstraint.w = kLocked;
    mSizeConstraint.h = kLocked;
    mIsAnimatable = true;
    mTickColor.r = mTickColor.g = mTickColor.b = 255;
}

GUIComboBox::~GUIComboBox()
{
    mComboBoxItems.clear();
}

Color
GUIComboBox::getTickColor(void)
{
    return mTickColor;
}

void
GUIComboBox::setTickColor(Color aColor)
{
    mTickColor.r = aColor.r;
    mTickColor.g = aColor.g;
    mTickColor.b = aColor.b;
}

void
GUIComboBox::setFocus(bool aIsFocused)
{
    mIsFocused = aIsFocused;
}

Status 
GUIComboBox::draw(void)
{
    if (mIsExpanded)
    {
        auto lMenuItem = mComboBoxItems.begin();
        for (; lMenuItem != mComboBoxItems.end(); ++lMenuItem)
        {
            (*lMenuItem)->draw();
        }
    }
    else
    {
        // draw mSelectedComboBoxItem at this position
        if (mSelectedComboBoxItem)
        {
            mSelectedComboBoxItem->draw();
        }

        int lBoxPosX = mBounds.x;
        int lBoxPosY = mBounds.y;
        int lBoxW = mBounds.w;
        int lBoxH = mBounds.h;

        glPushMatrix();
        GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
        glBegin(GL_LINES);
        {
            // draw tick (arrows)
            glColor3ub(mTickColor.r, mTickColor.g, mTickColor.b);

            glVertex2i(lBoxW - 6, 4);
            glVertex2i(lBoxW - 6, lBoxH - 4);

            glVertex2i(lBoxW - 7, lBoxH - 6);
            glVertex2i(lBoxW - 4, lBoxH - 6);

            glVertex2i(lBoxW - 7, 5);
            glVertex2i(lBoxW - 4, 5);
        }
        glEnd();
        glPopMatrix();
    }
    return kSucceeded;
}

void
GUIComboBox::setBounds(Rectangle2D aBounds)
{
    GUIElement::setBounds(aBounds);
    mLargestChildSize.x = aBounds.x;
    mLargestChildSize.y = aBounds.y;
    if (mIsExpanded)
    {
        updateChildBounds();
    }
    else if (mSelectedComboBoxItem)
    {
        mSelectedComboBoxItem->setBounds(mLargestChildSize);
    }
}

Status
GUIComboBox::addChild(GUIComboBoxItem* aGUIComboBoxItem)
{
    if (!aGUIComboBoxItem)
    {
        return kBadArgument;
    }
    Rectangle2D lBounds = aGUIComboBoxItem->getBounds();
    mComboBoxItems.push_back(aGUIComboBoxItem);
    aGUIComboBoxItem->setParent(this);
    aGUIComboBoxItem->setParentWindow(mParentWindow);
    aGUIComboBoxItem->setVisible(false);
    if (1 == mComboBoxItems.size())
    {
        mSelectedComboBoxItem = aGUIComboBoxItem;
        mSelectedIndex = 0;
        mFocusedIndex = 0;
        mLargestChildSize.w = lBounds.w;
        mLargestChildSize.h = lBounds.h;
    }
    else 
    {
        if (lBounds.w > mLargestChildSize.w)
        {
            mLargestChildSize.w = lBounds.w;
        }
        if (lBounds.h > mLargestChildSize.h)
        {
            mLargestChildSize.h = lBounds.h;
        }
    }
    setBounds(mLargestChildSize);
    return kSucceeded;
}

Status 
GUIComboBox::addChild(Window* /* aParentWindow */, GUIComboBoxItem* aGUIComboBoxItem)
{
    return addChild(aGUIComboBoxItem);
}

Status
GUIComboBox::addChild(GUIElement* /* aGUIElement */)
{
    fprintf(stderr, "GUIComboBox::addChild - ordinary GUIElements cannot be added\n");
    return kFailed;
}

Status
GUIComboBox::addChild(Window* /* aParentWindow */, GUIElement* /* aGUIElement */)
{
    fprintf(stderr, "GUIComboBox::addChild - ordinary GUIElements cannot be added\n");
    return kFailed;
}

Status 
GUIComboBox::removeChild(GUIComboBoxItem* aGUIComboBoxItem)
{
    auto lChildIterator = mComboBoxItems.begin();
    for (; lChildIterator != mComboBoxItems.end(); ++lChildIterator)
    {
        if (*lChildIterator == aGUIComboBoxItem)
        {
            mComboBoxItems.erase(lChildIterator);
            return kSucceeded;
        }
    }
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIComboBox::removeChild - GUIComboBoxItem not found\n");
    #endif
    return kFailed;
}

EventSignal
GUIComboBox::handleKeyEvent(KeyEvent* aKeyEvent)
{
    #ifdef DEBUG_EVENTS
    fprintf(stderr, "GUIComboBox %d got KeyEvent\n", mID);
    #endif
    if (!mIsExpanded)
    {
        if (kKeyPressed == aKeyEvent->mKeyEventType &&
            kKeyTypeNormal == aKeyEvent->mKeyType && 
            ' ' == aKeyEvent->mKey)
        {
            expand();
            aKeyEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        else if (kKeyPressed == aKeyEvent->mKeyEventType &&
                 kKeyTypeSpecial == aKeyEvent->mKeyType &&
                 (Keys::kUp == aKeyEvent->mKey ||
                  Keys::kDown == aKeyEvent->mKey))
        {
            expand();
            aKeyEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
    }
    else 
    {
        if (kKeyPressed == aKeyEvent->mKeyEventType &&
            kKeyTypeNormal == aKeyEvent->mKeyType)
        {
            if (27 == aKeyEvent->mKey) // ESC
            {
                collapse();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (' ' == aKeyEvent->mKey)
            {
                if (mSelectedComboBoxItem)
                {
                    mSelectedComboBoxItem->handleKeyEvent(aKeyEvent);
                }
            }
        }
        else if (kKeyPressed == aKeyEvent->mKeyEventType &&
                 kKeyTypeSpecial == aKeyEvent->mKeyType)
        {
            if (Keys::kUp == aKeyEvent->mKey)
            {
                focusPrevItem();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kDown == aKeyEvent->mKey)
            {
                focusNextItem();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
    }
    return kEventIgnored;
}

EventSignal
GUIComboBox::handleMouseEvent(MouseEvent* aMouseEvent)
{
    if (kMouseUp == aMouseEvent->mMouseEventType)
    {
        if (Util::inBounds(mBounds, aMouseEvent->mMousePosition))
        {
            if (!mIsExpanded)
            {
                // DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                expand();
                // lDisplayManager->requestUpdate(this);
            }
            else
            {
                collapse();
            }
            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        else
        {
            if (mIsExpanded)
            {
                collapse();
            }
        }
    }
    return kEventIgnored;
}

void
GUIComboBox::collapse(void)
{
    auto lChildIterator = mComboBoxItems.begin();
    for (; lChildIterator != mComboBoxItems.end(); ++lChildIterator)
    {
        (*lChildIterator)->setVisible(false);
    }
    mIsExpanded = false;
}

void
GUIComboBox::expand(void)
{
    auto lChildIterator = mComboBoxItems.begin();
    for (; lChildIterator != mComboBoxItems.end(); ++lChildIterator)
    {
        (*lChildIterator)->setVisible(true);
    }
    mIsExpanded = true;
    updateChildBounds();
}

GUIComboBoxItem*
GUIComboBox::getSelectedItem(void)
{
    return mSelectedComboBoxItem;
}

SInt32
GUIComboBox::getSelectedIndex(void)
{
    return mSelectedIndex;
}

Status
GUIComboBox::setSelectedItem(GUIComboBoxItem* aGUIComboBoxItem)
{
    SInt32 lSelectedIndex = -1;
    if (!aGUIComboBoxItem)
    {
        return kBadArgument;
    }

    setFocusedItem(aGUIComboBoxItem);

    if (aGUIComboBoxItem == mSelectedComboBoxItem)
    {
        return kSucceeded;
    }

    for (UInt32 i = 0; i < mComboBoxItems.size(); ++i)
    {
        if (aGUIComboBoxItem == mComboBoxItems[i])
        {
            lSelectedIndex = i;
            mSelectedComboBoxItem = mComboBoxItems[i];
        }
        else
        {
            BooleanOption* lOption = mComboBoxItems[i]->getBooleanOption();
            if (lOption)
            {
                lOption->setValue(aGUIComboBoxItem, false);
            }
            else
            {
                mComboBoxItems[i]->setValue(false);
            }
        }
    }

    if (lSelectedIndex >= 0)
    {
        mSelectedIndex = lSelectedIndex;
        updateChildBounds();
        return kSucceeded;
    }
    else
    {
        return kFailed;
    }
}

Status
GUIComboBox::setFocusedItem(GUIComboBoxItem* aGUIComboBoxItem)
{
    SInt32 lFocused = -1;
    for (UInt32 i = 0; i < mComboBoxItems.size(); ++i)
    {
        if (mComboBoxItems[i] == aGUIComboBoxItem)
        { 
            mFocusedComboBoxItem = mComboBoxItems[i];
            lFocused = i;
            break;
        }
    }
    if (lFocused >= 0)
    {
        mFocusedIndex = lFocused;
        return kSucceeded;
    }
    else
    {
        fprintf(stderr, "GUIComboBox::setFocusedItem - could not find item in list\n");
        return kFailed;
    }
}

void
GUIComboBox::updateChildBounds(void)
{
    Rectangle2D lBounds = mBounds;
    lBounds.w = mLargestChildSize.w;
    lBounds.h = mLargestChildSize.h;
    lBounds.y += mLargestChildSize.h * mSelectedIndex;

    auto lChildIterator = mComboBoxItems.begin();
    for (; lChildIterator != mComboBoxItems.end(); ++lChildIterator)
    {
        (*lChildIterator)->setBounds(lBounds);
        lBounds.y -= mLargestChildSize.h;
    }
}

void
GUIComboBox::focusNextItem(void)
{
    SInt32 lFocused = mFocusedIndex + 1;
    if (lFocused == (SInt32)(mComboBoxItems.size()))
    {
        lFocused = 0;
    }
    mFocusedIndex = lFocused;
    mFocusedComboBoxItem = mComboBoxItems[mFocusedIndex];
    mParentWindow->setFocusedElement(mFocusedComboBoxItem);
}

void
GUIComboBox::focusPrevItem(void)
{
    SInt32 lFocused = int(mFocusedIndex) - 1;
    if (lFocused < 0)
    {
        lFocused = mComboBoxItems.size() - 1;
    }
    mFocusedIndex = lFocused;
    mFocusedComboBoxItem = mComboBoxItems[mFocusedIndex];
    mParentWindow->setFocusedElement(mFocusedComboBoxItem);
}

bool
GUIComboBox::updateAnimation(void)
{
    if (mIsAnimatable && !mIsExpanded && mSelectedComboBoxItem)
    {
        mSelectedComboBoxItem->updateAnimation();
        return true;
    }
    return false;
}
