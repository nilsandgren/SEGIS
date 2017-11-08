//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILayerPanel: 
//      Implementation
//

#include <SEGIS/GUILayerPanel.h>
#include <SEGIS/GUILayerPanelButton.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/LayoutManager.h>
#include <SEGIS/Window.h>

GUILayerPanel::GUILayerPanel(UInt32      aID,
                             Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
    , mButtonArea(0)
    , mLayerArea(0)
    , mSelectedLayer(0)
    , mSelectedLayerIndex(-1)
    , mLayerButtonHeight(20)
{
    mTextColor.r = 255;
    mTextColor.g = 255;
    mTextColor.b = 255;

    mIsVisible = true;

    setFillDirection(kFillDirectionHorizontal);
    
    // create a private GUIElement for storing buttons
    Rectangle2D lBounds;
    lBounds.x = mBounds.x;
    lBounds.y = mBounds.y;
    lBounds.w = mBounds.w;
    lBounds.h = mLayerButtonHeight;

    SizeConstraint lSizeConstraint;
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;

    Alignment lAlignment;
    lAlignment.mHAlign = kLeft;
    lAlignment.mVAlign = kBottom;

    mButtonArea = new GUIElement(0, lBounds);
    mButtonArea->setSizeConstraint(lSizeConstraint);
    mButtonArea->setFillDirection(kFillDirectionHorizontal);
    mButtonArea->setAlignment(lAlignment);
    mButtonArea->setVisible(false);

    lBounds.h = mBounds.h;
    lAlignment.mVAlign = kTop;
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    mLayerArea = new GUIElement(0, lBounds);
    mLayerArea->setSizeConstraint(lSizeConstraint);
    mLayerArea->setFillDirection(kFillDirectionHorizontal);
    mLayerArea->setAlignment(lAlignment);
    mLayerArea->setVisible(true);
    mLayerArea->setPadding(1);
    mLayerArea->setBevel(1);

    // we haven't got any parent window yet so we give 
    // the mButtonArea and mLayerArea a parent window later

    mChildren.push_front(mButtonArea);
    mChildren.push_front(mLayerArea);
}

GUILayerPanel::~GUILayerPanel()
{
    mLayers.clear();
    mLayerButtons.clear();
}

// contained GUIElements are stored in mLayers. only one of these are stored in
// mChildren at a time so we need to overload some GUIElement functions
Status
GUILayerPanel::addChild(GUIElement* aGUIElement)
{
    if (!aGUIElement)
    {
        return kBadArgument;
    }

    // only needs to be done once, but who cares?
    mButtonArea->setParentWindow(mParentWindow);
    mLayerArea->setParentWindow(mParentWindow);

    mLayers.push_back(aGUIElement);
    aGUIElement->setParent(this);
    aGUIElement->setParentWindow(mParentWindow);

    if (1 == mLayers.size())
    {
        mLayerArea->addChild(aGUIElement);
        mSelectedLayer = aGUIElement;
        mSelectedLayerIndex = 0;
    }

    // create a button for this layer and add to mButtonArea
    Rectangle2D lDefaultButtonBounds;
    // we let LayoutManager handle position
    lDefaultButtonBounds.x = 0;
    lDefaultButtonBounds.y = 0;
    lDefaultButtonBounds.w = 64;
    lDefaultButtonBounds.h = mLayerButtonHeight;

    // keep LayoutManager from resizing the buttons
    SizeConstraint lButtonSizeConstraint;
    lButtonSizeConstraint.w = kLocked;
    lButtonSizeConstraint.h = kLocked;

    GUILayerPanelButton* lLayerButton = new GUILayerPanelButton(0, lDefaultButtonBounds);
    lLayerButton->setParentWindow(mParentWindow);
    lLayerButton->setSizeConstraint(lButtonSizeConstraint);
    lLayerButton->setLabelColor(mTextColor);
    lLayerButton->setLayerIndex(mLayers.size() - 1); 
    lLayerButton->setLayerPanel(this);
    lLayerButton->setParent(this);
    lLayerButton->setColor(mColor);
    mButtonArea->addChild(lLayerButton);
    mLayerButtons.push_back(lLayerButton);

    updateButtonIndices();

    return kSucceeded;
}

Status
GUILayerPanel::addChild(Window* aParentWindow, GUIElement* aGUIElement)
{
    aGUIElement->setParentWindow(aParentWindow);
    return addChild(aGUIElement);
}

Status
GUILayerPanel::removeChild(GUIElement* aGUIElement)
{
    auto lLayerIterator = mLayers.begin();
    auto lButtonIterator = mLayerButtons.begin();
    GUIElement* lDeletedElement = 0;
    for (; lLayerIterator != mLayers.end(); ++lLayerIterator, ++lButtonIterator)
    {
        if (*lLayerIterator == aGUIElement)
        {
            lDeletedElement = *lLayerIterator;
            mLayers.erase(lLayerIterator);
            mLayerButtons.erase(lButtonIterator);
            break;
        }
    }
    if (lDeletedElement)
    {
        // is the removed layer the same as the visible one?
        if (lDeletedElement == mSelectedLayer)
        {
            if (mLayers.size())
            {
                // if we have more layers left we set the first one as visible
                mLayerArea->removeChild(lDeletedElement);
                mSelectedLayerIndex = 0;
                mSelectedLayer = mLayers[0];
                mLayerArea->addChild(mSelectedLayer);
                // update the layer index for all buttons
                for (UInt32 lLayer = 0; lLayer < mLayerButtons.size(); ++lLayer)
                {
                    mLayerButtons[lLayer]->setLayerIndex(lLayer);
                }
            }
            else
            {
                // no layers left
                mSelectedLayerIndex = -1;
                mLayerArea->removeChild(lDeletedElement);
                mSelectedLayer = 0;
            }
        }
        updateButtonIndices();
        return kSucceeded;
    }
    else
    {
        fprintf(stderr, "GUILayerPanel::removeChild - child not found\n");
        return kFailed;
    }
}

void
GUILayerPanel::setAlignment(Alignment aAlignment)
{
    if (mLayerArea)
    {
        mLayerArea->setAlignment(aAlignment);
    }
    if (mButtonArea)
    {
        mButtonArea->setAlignment(aAlignment);
    }
}

Status
GUILayerPanel::setSelectedLayer(UInt32 aLayerIndex)
{
    if (aLayerIndex >= mLayers.size())
    {
        fprintf(stderr, "GUILayerPanel::setSelectedLayer - layer index higher than max index\n");
        return kFailed;
    }

    mLayerArea->removeChild(mSelectedLayer);
    mSelectedLayer = mLayers[aLayerIndex];
    mSelectedLayerIndex = aLayerIndex;
    mLayerArea->addChild(mSelectedLayer);

    if (mParentWindow)
    {
        mParentWindow->updateTabOrder();
    }

    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    // LayoutManager::resize(mSelectedLayer, mLayerArea->getBounds());
    LayoutManager::resize(this, mBounds, 1);
    lDisplayManager->requestUpdate(mSelectedLayer);

    return kSucceeded;
}

Status
GUILayerPanel::selectNextLayer(void)
{
    SInt32 lIndex = mSelectedLayerIndex++;
    if (lIndex >= (SInt32)(mLayers.size()))
    {
        lIndex = 0;
    }
    setSelectedLayer(lIndex);
    return kSucceeded;
}

Status
GUILayerPanel::selectPrevLayer(void)
{
    SInt32 lIndex = mSelectedLayerIndex--;

    if (lIndex < 0)
    {
        lIndex = mLayers.size() - 1;
    }
    setSelectedLayer(lIndex);
    return kSucceeded;
}

GUIElement*
GUILayerPanel::getSelectedLayer(void)
{
    if (mSelectedLayerIndex >= 0)
    {
        return mLayers[mSelectedLayerIndex];
    }
    else
    {
        return 0;
    }
}

SInt32
GUILayerPanel::getSelectedLayerIndex(void)
{
    return mSelectedLayerIndex;
}

Status
GUILayerPanel::setLayerLabelText(UInt32 aLayerIndex,
                                 const WString & aLabelText)
{
    if (mLayerButtons[aLayerIndex])
    {
        mLayerButtons[aLayerIndex]->setText(aLabelText);
        return kSucceeded;
    }
    else
    {
        fprintf(stderr, 
                "GUILayerPanel::setLabelText - there's no layer wwith index %d\n",
                aLayerIndex);
        return kFailed;
    }
}

void
GUILayerPanel::setLabelColor(Color aColor)
{
    mTextColor = aColor;
    for (UInt32 i = 0; i < mLayerButtons.size(); ++i)
    {
        mLayerButtons[i]->setLabelColor(mTextColor);
    }
}

Color
GUILayerPanel::getLabelColor(void)
{
    return mTextColor;
}

void
GUILayerPanel::setColor(Color aColor)
{
    mColor.r = aColor.r;
    mColor.g = aColor.g;
    mColor.b = aColor.b;

    for (UInt32 i = 0; i < mLayerButtons.size(); ++i)
    {
        mLayerButtons[i]->setColor(aColor);
    }
    mLayerArea->setColor(aColor);
}

void
GUILayerPanel::setFocusColor(Color aColor, bool aRecurse)
{
    mFocusColor.r = aColor.r;
    mFocusColor.g = aColor.g;
    mFocusColor.b = aColor.b;

    // set color for button row
    auto lButtonIterator = mLayerButtons.begin();
    for (; lButtonIterator != mLayerButtons.end(); ++lButtonIterator)
    {
        (*lButtonIterator)->setFocusColor(aColor, false);
    }
    
    // propagate to elements contained in the layers
    if (aRecurse)
    {   
        auto lLayerIterator = mLayers.begin();
        for (; lLayerIterator != mLayers.end(); ++lLayerIterator)
        {
            (*lLayerIterator)->setFocusColor(aColor, true);
        }
    }
}

Status 
GUILayerPanel::draw(void)
{
    resizeLayerPanelButtons();
    // elements in mChildren (buttons + visible layer) will draw them self
    GUIElement::draw();
    return kSucceeded;
}

void
GUILayerPanel::updateButtonIndices(void)
{
    /*
    SInt32 lButtons = mLayerButtons.size()-1;
    for (UInt32 i = 0; i < mLayerButtons.size(); ++i, lButtons--)
    {
        mLayerButtons[i]->setLayerIndex(i);
    }
    */
}

UInt32
GUILayerPanel::getMinimumSafeWidth(void)
{
    if (mLayerButtons.size())
    {
        // calc width of layer panel buttons and all the layers. return the largest
        // of the two.
        UInt32 lWidth = 0;
        UInt32 lTmp = 0;
        // button row
        auto lButtonIterator = mLayerButtons.begin();
        for (; lButtonIterator != mLayerButtons.end(); ++lButtonIterator)
        {
            lWidth += (*lButtonIterator)->getBounds().w;
        }

        // layers
        auto lLayerIterator = mLayers.begin();
        for (; lLayerIterator != mLayers.end(); ++lLayerIterator)
        {
            lTmp = (*lLayerIterator)->getMinimumSafeWidth();
            if (lTmp > lWidth)
            {
                lWidth = lTmp;
            }
        }
        return lWidth;
    }
    else
    {
        if (kLocked == mSizeConstraint.w)
        {
            return mBounds.w;
        }
        else
        {
            return 0;
        }
    }
}

UInt32
GUILayerPanel::getMinimumSafeHeight(void)
{
    if (mLayerButtons.size())
    {
        // add height of button row to height of tallest layer
        UInt32 lTmp = 0;
        UInt32 lLayerHeight = 0;
        // check height of first button, assuming they are the same
        UInt32 lButtonHeight = mLayerButtons[0]->getBounds().h;

        // layers
        auto lLayerIterator = mLayers.begin();
        for (; lLayerIterator != mLayers.end(); ++lLayerIterator)
        {
            lTmp = (*lLayerIterator)->getMinimumSafeHeight();
            if (lTmp > lLayerHeight)
            {
                lLayerHeight = lTmp;
            }
        }
        return lLayerHeight + lButtonHeight;
    }
    else
    {
        if (kLocked == mSizeConstraint.h)
        {
            return mBounds.h;
        }
        else
        {
            return 0;
        }
    }
}

void
GUILayerPanel::resizeLayerPanelButtons()
{
    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
    UInt32 lPosX = mBounds.x;
    Rectangle2D lBounds;
    auto lButtonIterator = mLayerButtons.begin();
    for ( ; lButtonIterator != mLayerButtons.end(); ++lButtonIterator)
    {
        lBounds = (*lButtonIterator)->getBounds();
        lBounds.x = lPosX;
        lBounds.w = lTextRenderer->getStringWidth((*lButtonIterator)->getText()) + 12;
        lPosX += lBounds.w;
        (*lButtonIterator)->setBounds(lBounds);
    }
}

