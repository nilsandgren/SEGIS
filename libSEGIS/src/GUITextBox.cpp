//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUITextBox
//      Implementation
//

#include <SEGIS/GUITextBox.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/StringOption.h>
#include <string.h>
#include <ctype.h>

GUITextBox::GUITextBox(UInt32          aID,
                       Rectangle2D     aBounds,
                       const WString & aText)
    : GUIOptionElement(aID, aBounds)
    , mTextPadding(2)
    , mCursorHPos(mTextPadding)
    , mStringOption(0)
    , mInitialUpdateDone(0)
{
    setPadding(2);
    setBevel(-1);

    mTextColor.r = 255;
    mTextColor.g = 255;
    mTextColor.b = 255;

    TextElement::setText(aText);
    
    mEditPosition = mTextString.length();
}

GUITextBox::~GUITextBox()
{
}

EventSignal
GUITextBox::handleKeyEvent(KeyEvent* aKeyEvent)
{
    if (kKeyPressed == aKeyEvent->mKeyEventType)
    {
        if (kKeyTypeNormal == aKeyEvent->mKeyType)
        {
            bool lUpdate = false;
            char lKey = (char) aKeyEvent->mKey;
            if (127 == lKey)
            {
                deleteChar();
                lUpdate = true;
            }
            else if (isprint(lKey))
            {
                addChar(lKey);
                lUpdate = true;
            }

            if (lUpdate)
            {
                if (mStringOption)
                {
                    mStringOption->setStringValue(this, mTextString);
                }

                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
        else
        {   // special key callback
            char lKey = (char) aKeyEvent->mKey;
            bool lUpdate = false;
            switch (lKey)
            {
                case Keys::kLeft:
                    mEditPosition = max(0, mEditPosition-1);
                    lUpdate = true;
                    break;
                case Keys::kRight:
                    mEditPosition = min((int)mTextString.length(), mEditPosition+1);
                    lUpdate = true;
                    break;
                case Keys::kUp:
                    mEditPosition = 0;
                    lUpdate = true;
                    break;
                case Keys::kDown:
                    mEditPosition = mTextString.length();
                    lUpdate = true;
                    break;
            }
            if (lUpdate)
            {
                DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
                lDisplayManager->requestUpdate(this);
                aKeyEvent->mAcceptor = this;
                return kEventAbsorbed;
            }
        }
    }

    return kEventIgnored;
}

EventSignal
GUITextBox::handleMouseEvent(MouseEvent* aMouseEvent)
{
#   ifdef DEBUG_EVENTS
    fprintf(stderr, "GUITextBox %d got MouseEvent.\n", mID);
#   endif

    if (kMouseUp != aMouseEvent->mMouseEventType)
    {
        glutSetCursor(GLUT_CURSOR_TEXT);
    }

    if ((kMouseDragged != aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMousePosition))
        ||
        (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(mBounds, aMouseEvent->mMouseOriginalPosition)))
    {
        if (kMouseDown == aMouseEvent->mMouseEventType)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            findEditPos(aMouseEvent);
            lDisplayManager->requestUpdate(this);
            aMouseEvent->mAcceptor = this;
            return kEventAbsorbed;
        }
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    return kEventIgnored;
}

Status
GUITextBox::draw(void)
{
    Status lStatus = kSucceeded;

    if (!mInitialUpdateDone)
    {
        updateFromOption();
    }
    
    calcCursorPos();

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    glBegin(GL_QUADS);
    {
        // draw main area
        glColor3ub(mColor.r, mColor.g, mColor.b);
        glVertex2i(0, 0);
        glVertex2i(0, lBoxH);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);

        SInt32 lBevel = abs(mBevel);
        // draw bevel left
        glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
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
        glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);
        glVertex2i(lBoxW - lBevel, lBevel);
        glVertex2i(lBoxW - lBevel, lBoxH - lBevel);

        // draw bevel bottom
        glVertex2i(0, 0);
        glVertex2i(lBevel, lBevel);
        glVertex2i(lBoxW - lBevel, lBevel);
        glVertex2i(lBoxW, 0);
    }
    glEnd();

    // cursor
    if (mIsFocused)
    {
        glBegin(GL_LINES);
        {
            // draw cursor
            glColor3ub(mTextColor.r, mTextColor.g, mTextColor.b);
            glVertex2i(mCursorHPos, mTextPadding);
            glVertex2i(mCursorHPos, lBoxH - mTextPadding);
        }
        glEnd();
    }
    
    
    // draw mLabelText
    if (mTextString.length())
    {
        glColor3ub(mTextColor.r, mTextColor.g, mTextColor.b);
        
        TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
        lTextRenderer->drawText(mPadding + mTextPadding, mPadding + mTextPadding + 1, mTextString);
    }

    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIButton::draw - Drawing GUIButton %d\n", mID);
    #endif

    return lStatus;
}

Status
GUITextBox::setText(const WString & aText)
{
    TextElement::setText(aText);
    mEditPosition = mTextString.length();
    
    return kSucceeded;
}

void
GUITextBox::findEditPos(MouseEvent* aMouseEvent)
{
    UInt32 lClickPos = aMouseEvent->mMousePosition.x - mBounds.x - mPadding;
    UInt32 lPixelSum = 0;
    UInt32 lCharWidth = 0;
    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
    mEditPosition = 0;
    for (UInt32 i = 0; i < mTextString.length(); i++)
    {
        lCharWidth = lTextRenderer->getCharacterWidth((int)(mTextString[i]));
        if (lClickPos < lPixelSum + lCharWidth / 2)
        {
            break;
        }
        lPixelSum += lCharWidth;
        mEditPosition++;
    }
    mCursorHPos = lPixelSum + mTextPadding + mPadding - 1;
}

void
GUITextBox::calcCursorPos()
{
    UInt32 lPixelSum = 0;
    mCursorHPos = 0;
    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
    for (SInt32 i = 0; i < mEditPosition; i++)
    {
        lPixelSum += lTextRenderer->getCharacterWidth((int)(mTextString[i]));
    }
    mCursorHPos = lPixelSum + mTextPadding + 1;
}

void
GUITextBox::deleteChar()
{
    if (mEditPosition)
    {
        mTextString.erase(mEditPosition-1, 1);
        mEditPosition = max(0, mEditPosition-1);
    }
}

void
GUITextBox::addChar(char lKey)
{
    wchar_t lWChar = (wchar_t) lKey;
    mTextString.insert(mEditPosition, 1, lWChar);
    mEditPosition++;
}

void
GUITextBox::setTextColor(Color aColor)
{
    mTextColor.r = aColor.r;
    mTextColor.g = aColor.g;
    mTextColor.b = aColor.b;
}

Color
GUITextBox::getTextColor(void)
{
    return mTextColor;
}

Status
GUITextBox::setStringOption(StringOption* aStringOption)
{
    // don't do anything if aNumberOption is 0
    if (!aStringOption)
    {
        return kFailed;
    }

    // if we already have an Option we must tell it that
    // we're no longer observing it
    if (0 != mStringOption)
    {
        mStringOption->removeObserver(this);
    }
    mStringOption = aStringOption;

    // we're now observing this option
    mStringOption->addObserver(this);
    return kSucceeded;
}

void
GUITextBox::updateFromOption()
{
    if (mStringOption)
    {
        setText(mStringOption->getStringValue());
        mInitialUpdateDone = true;
    }
}


Status
GUITextBox::update(const Observable* aObservable)
{
    // make sure we have an option
    if (0 != mStringOption)
    {
        // XXX this is a bit ugly, but we know this is a StringOption
        // since thats the only way to add an observable to this class
        StringOption* lStringOption = (StringOption*) aObservable;

        // the option to update from is the correct option
        if (mStringOption == lStringOption)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            // update our state from the option and request redisplay
            setText(mStringOption->getStringValue());
            lDisplayManager->requestUpdate(this);
            return kSucceeded;
        }
        else
        {
            fprintf(stderr,
                    "GUITextBox::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr,
                "GUITextBox::update - this text box has no option attached to it\n");
        return kFailed;
    }
}

bool
GUITextBox::isInTabOrder(void)
{
    return true;
}
