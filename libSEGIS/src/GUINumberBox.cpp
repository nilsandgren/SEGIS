//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUITextBox
//      Implementation
//

#include <SEGIS/GUINumberBox.h>
#include <SEGIS/DisplayManager.h>
#include <string.h>
#include <ctype.h>
#include <sstream>

GUINumberBox::GUINumberBox(UInt32          aID,
                           Rectangle2D     aBounds,
                           const WString & aText,
                           NumberType      aNumberType)
    : GUITextBox(aID, aBounds, aText)
    , mNumberType(aNumberType)
    , mNumberOption(0)
    , mDotIndex(-1)
    , mHasMinus(false)
{
}

GUINumberBox::~GUINumberBox()
{
}

void
GUINumberBox::setFocus(bool aIsFocused)
{
    if (!aIsFocused && mNumberOption)
    {
        setValue(mNumberOption->getValue());
    }
    GUIOptionElement::setFocus(aIsFocused);
}

EventSignal
GUINumberBox::handleKeyEvent(KeyEvent* aKeyEvent)
{
    if (kKeyPressed == aKeyEvent->mKeyEventType)
    {
        if (kKeyTypeNormal == aKeyEvent->mKeyType)
        {
            // TODO handle prosition in text. only append/remove at end now
            char lKey = (char) aKeyEvent->mKey;
            if (127 == lKey) // backspace
            {
                if (mEditPosition == mDotIndex)
                {
                    mDotIndex = -1;
                }
                deleteChar();
                if (mEditPosition == 0)
                {
                    mHasMinus = false;
                }
            }
            else if (isdigit(lKey) &&
                     !(mHasMinus && mEditPosition == 0))
            {
                addChar(lKey);
            }
            else if (kNumberFloat == mNumberType &&
                    '.' == lKey &&
                    -1 == mDotIndex)
            {
                addChar(lKey);
                mDotIndex = mEditPosition - 1;
            }
            else if (!mHasMinus &&
                     mEditPosition == 0 &&
                    '-' == lKey)
            {
                mHasMinus = true;
                addChar(lKey);
            }
            else
            {
                return kEventIgnored;
            }

            Float32 lFloatValue = wcstof(mTextString.c_str(), (wchar_t **) NULL);
            
            if (mNumberOption)
            {
                lFloatValue = mNumberOption->getValidValue(lFloatValue);
                mNumberOption->setValue(this, lFloatValue);
#               if 0
                // TODO make this work for float aswell
                if (kNumberInteger == mNumberType)
                {
                    if (mTextLength &&
                        !(mTextLength == 1 && mText[0] == '-'))
                    {
                        setValue(mNumberOption->getValue());
                    }
                }
#               endif
            }

            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            lDisplayManager->requestUpdate(this);
            aKeyEvent->mAcceptor = this;
            return kEventAbsorbed;
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

Status
GUINumberBox::setText(const WString & aText)
{
    TextElement::setText(aText);
    Float32 lFloatValue = 0.0f;
    if (aText.length())
    {
        lFloatValue = wcstof(mTextString.c_str(), (wchar_t **) NULL);
    }
    if (lFloatValue < 0.0f)
    {
        mHasMinus = true;
    }
    else
    {
        mHasMinus = false;
    }
    if (mNumberOption)
    {
        lFloatValue = mNumberOption->getValidValue(lFloatValue);
        mNumberOption->setValue(this, lFloatValue);
    }
    // DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    // lDisplayManager->requestUpdate(this);
    return kSucceeded;
}

void
GUINumberBox::setValue(Float32 aValue)
{
    if (kNumberFloat == mNumberType)
    {
        std::wstringstream converter;
        converter << aValue;
        converter >> mTextString;
    }
    else
    {
        SInt32 lIntVal = (SInt32) aValue;
        std::wstringstream converter;
        converter << lIntVal;
        converter >> mTextString;
    }

    mEditPosition = mTextString.length();
}

Status 
GUINumberBox::setNumberOption(NumberOption* aNumberOption)
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
GUINumberBox::updateFromOption()
{
    if (mNumberOption)
    {
        setValue(mNumberOption->getValue());
        mInitialUpdateDone = true;
    }
}

// Observer interface
Status 
GUINumberBox::update(const Observable* aObservable)
{
    // make sure we have an option
    if (0 != mNumberOption)
    {
        // XXX this is a bit ugly, but we know this is a NumberOption
        // since thats the only way to add an observable to this class
        NumberOption* lNumberOption = (NumberOption*) aObservable;

        // and if so, that the option to update from is the correct option
        if (mNumberOption == lNumberOption)
        {
            DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
            // update our state from the option and request redisplay
            setValue(mNumberOption->getValue());
            lDisplayManager->requestUpdate(this);
            return kSucceeded;
        }
        else
        {
            fprintf(stderr,
                    "GUINumberBox::update - update was called with other option than the observed one!");
            return kFailed;
        }
    }
    else
    {
        fprintf(stderr,
                "GUINumberBox::update - this slider has no option attached to it\n");
        return kFailed;
    }
}

bool
GUINumberBox::isInTabOrder(void)
{
    return true;
}
