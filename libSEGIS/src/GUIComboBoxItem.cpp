//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIComboBoxItem:
//      Implementation
//


#include <SEGIS/BooleanOption.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/GUIComboBoxItem.h>
#include <SEGIS/Window.h>

GUIComboBoxItem::GUIComboBoxItem(UInt32          aID,
                                 Rectangle2D     aBounds,
                                 const WString & aLabelText)
    : GUIOptionElement(aID, aBounds)
    , mIsSelected(false)
    , mBooleanOption(0)
    , mInitialUpdateDone(false)
    , mParentComboBox(0)
    , mPulseMax(20)
    , mPulseVal(0)
    , mPulseSign(1)
{
    // TODO set size from label text

    mLabelColor.r = 255;
    mLabelColor.g = 255;
    mLabelColor.b = 255;
    mIsAnimatable = true;

    mBevel = 1;
    
    TextElement::setText(aLabelText);
}

GUIComboBoxItem::~GUIComboBoxItem()
{
}

void
GUIComboBoxItem::setParent(GUIComboBox* aGUIComboBox)
{
    mParent = aGUIComboBox;
    mParentComboBox = aGUIComboBox;
}

Status
GUIComboBoxItem::draw(void)
{
    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    // draw main area
    glBegin(GL_QUADS);
    {
        glColor3ub(mColor.r, mColor.g, mColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, lBoxH);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);
    }

    // draw bevel
    Color   lColor1;
    Color   lColor2;
    UInt32  lBevel = abs(mBevel);
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
    glVertex2i(lBevel, lBoxH - lBevel);
    glVertex2i(lBevel, lBevel);

    // draw bevel top
    glVertex2i(0, lBoxH);
    glVertex2i(lBoxW, lBoxH);
    glVertex2i(lBoxW - lBevel, lBoxH - lBevel);
    glVertex2i(lBevel, lBoxH - lBevel);

    // draw bevel right
    glColor3ub(lColor2.r, lColor2.g, lColor2.b);
    glVertex2i(lBoxW, lBoxH);
    glVertex2i(lBoxW, 0);
    glVertex2i(lBoxW - lBevel, lBevel);
    glVertex2i(lBoxW - lBevel, lBoxH - lBevel);

    // draw bevel bottom
    glVertex2i(0, 0);
    glVertex2i(lBevel, lBevel);
    glVertex2i(lBoxW - lBevel, lBevel);
    glVertex2i(lBoxW, 0);

    glEnd();

    // draw label
    glColor3ub(mLabelColor.r, mLabelColor.g, mLabelColor.b);

    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
    lTextRenderer->drawText(4, 4, mTextString);

    glPopMatrix();

#ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIComboBoxItem::draw - Drawing GUIComboBoxItem %d\n", mID);
#endif
    return kSucceeded;
}

void
GUIComboBoxItem::setVisible(bool aIsVisible)
{
    if(mParentWindow)
    {
        if (aIsVisible != mIsVisible)
        {
            if (aIsVisible)
            {
                mParentWindow->addOverlayElement(this);
            }
            else
            {
                mParentWindow->removeOverlayElement(this);
            }
        }
    }
    else
    {
        fprintf(stderr, "GUIComboBoxItem::setVisible - Iiik, I've got no parent window!\n");
    }
    mIsVisible = aIsVisible;
}

void
GUIComboBoxItem::setFocus(bool aIsFocused)
{
    if (aIsFocused != mIsFocused && mParentComboBox)
    {
        if (aIsFocused)
        {
            mParentComboBox->expand();
            mParentComboBox->setFocusedItem(this);
        }
        else
        {
            mParentComboBox->collapse();
        }
        mIsFocused = aIsFocused;
    }
}

void
GUIComboBoxItem::setValue(bool aValue)
{
    mIsSelected = aValue;
}

Status
GUIComboBoxItem::setLabelColor(Color aColor)
{
    mLabelColor.r = aColor.r;
    mLabelColor.g = aColor.g;
    mLabelColor.b = aColor.b;
    return kSucceeded;
}

Color
GUIComboBoxItem::getLabelColor(void)
{
    return mLabelColor;
}

Status
GUIComboBoxItem::setBooleanOption(BooleanOption* aBooleanOption)
{
    if (!aBooleanOption)
    {
        return kFailed;
    }

    // tell old Option we're not observing it
    if (0 != mBooleanOption)
    {
        mBooleanOption->removeObserver(this);
    }
    mBooleanOption = aBooleanOption;

    mBooleanOption->addObserver(this);
    return kSucceeded;
}

BooleanOption*
GUIComboBoxItem::getBooleanOption(void)
{
    return mBooleanOption;
}

void
GUIComboBoxItem::updateFromOption()
{
    if (mBooleanOption)
    {
        setValue(mBooleanOption->getValue());
        mInitialUpdateDone = true;
    }
}

Status
GUIComboBoxItem::update(const Observable* aObservable)
{
    if (0 != mBooleanOption)
    {
        // XXX this is a bit ugly, but we know this is a BooleanOption
        // since thats the only way to add an observable to this class
        BooleanOption* lBooleanOption = (BooleanOption*) aObservable;

        // and if so, that the option to update from is the correct option
        if (mBooleanOption == lBooleanOption)
        {
            // DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            // update our state from the option
            setValue(mBooleanOption->getValue());

            // tell mParentComboBox about the change if true == mBooleanOption->getValue()
            if (mParentComboBox && mBooleanOption->getValue())
            {
                // XXX could this lead to infinite recursion?
                mParentComboBox->setSelectedItem(this);
            }
            return kSucceeded;
        }
        else
        {
            fprintf(stderr,
                    "GUIComboBoxItem::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr,
                "GUIComboBoxItem::update - this combobox item has no option attached to it\n");
        return kFailed;
    }
}

EventSignal
GUIComboBoxItem::handleKeyEvent(KeyEvent* aKeyEvent)
{
    if (mIsFocused)
    {
        if (kKeyTypeNormal == aKeyEvent->mKeyType &&
            kKeyPressed == aKeyEvent->mKeyEventType)
        {
            if (27 == aKeyEvent->mKey) // ESC
            {
                mParentComboBox->collapse();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (' ' == aKeyEvent->mKey)
            {
                if (mBooleanOption && !mIsSelected)
                {
                    mBooleanOption->setValue(this, true);
                    mIsSelected = true;
                }
                mParentComboBox->setSelectedItem(this);
                mParentComboBox->collapse();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else if (kKeyTypeSpecial == aKeyEvent->mKeyType &&
                 kKeyPressed == aKeyEvent->mKeyEventType)
        {
            if (Keys::kUp == aKeyEvent->mKey)
            {
                mParentComboBox->focusPrevItem();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
            else if (Keys::kDown == aKeyEvent->mKey)
            {
                mParentComboBox->focusNextItem();
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
    }
    return kEventIgnored;
}

EventSignal
GUIComboBoxItem::handleMouseEvent(MouseEvent* aMouseEvent)
{
    // we handle mouse event by ourself but mParentComboBox must be told to collapse itself
    if (Util::inBounds(mBounds, aMouseEvent->mMousePosition))
    {
        glutSetCursor(GLUT_CURSOR_INFO);
        if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            if (mBooleanOption && !mIsSelected)
            {
                mBooleanOption->setValue(this, true);
                mIsSelected = true;
            }
            mParentComboBox->setSelectedItem(this);
            mParentComboBox->collapse();

            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        else if (kMouseMoved == aMouseEvent->mMouseEventType ||
                 kMouseDragged == aMouseEvent->mMouseEventType)
        {
            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
    }
    return kEventIgnored;
}

Status
GUIComboBoxItem::addChild(GUIElement* /* aGUIElement */)
{
    fprintf(stderr, "GUIComboBoxItem::addChild - children cannot be added to a GUIComboBoxItem\n");
    return kFailed;
}

Status
GUIComboBoxItem::addChild(Window* /* aParentWindow */, GUIElement* /* aGUIElement */)
{
    fprintf(stderr, "GUIComboBoxItem::addChild - children cannot be added to a GUIComboBoxItem\n");
    return kFailed;
}

bool
GUIComboBoxItem::updateAnimation(void)
{
    if ((mIsAnimatable && mPulseVal) != 0 ||
        (mParentComboBox &&
         !mParentComboBox->isExpanded() &&
         mParentComboBox->isFocused() &&
         mParentComboBox->getSelectedItem() == this) ||
        mIsFocused)
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
