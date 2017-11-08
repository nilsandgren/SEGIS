//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUITextBox
//      A single line text field that accepts text and numbers
//
//       --------
//      | a text |
//       --------

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "GUIOptionElement.h"
#include "Observer.h"
#include "TextElement.h"

class StringOption;

class GUITextBox : public GUIOptionElement, public TextElement, public Observer
{
    // variables
    protected:
        SInt32        mEditPosition;
        Color         mTextColor;
        UInt32        mTextPadding;
        UInt32        mCursorHPos;

        StringOption* mStringOption;
        bool          mInitialUpdateDone;

    // functions
    protected:
        // updates mEditPosition and mCursorHPos from the MouseEvent's position (for MouseEvents)
        virtual void findEditPos(MouseEvent* aMouseEvent);
        // updates mCursorHPos from mEditPosition (for KeyEvents)
        virtual void calcCursorPos();
        // removes a character at mEditPosition and updates state
        virtual void deleteChar();
        // adds a character at mEditPosition and updates state
        virtual void addChar(char lKey);

    public:
        GUITextBox(UInt32          aID,
                   Rectangle2D     aBounds,
                   const WString & aText);

        virtual ~GUITextBox();

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);

        // draws this GUIElement
        virtual Status draw(void);

        // set/get Color
        virtual void setTextColor(Color aColor);
        virtual Color getTextColor(void);
    
        virtual Status setText(const WString & aText);

        // attach a string option
        virtual Status setStringOption(StringOption* aStringOption);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a NumberOption
        virtual Status update(const Observable* aOption);

        virtual bool isInTabOrder(void);
};
