//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUINumberBox:
//      Extend GUITextBox
//      A single line text field that accepts numbers as input. The GUINumberBox can have
//      a NumberOption attached to it that is updated when the field is edited.
//
//       -------
//      | 47.11 |
//       -------
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "GUITextBox.h"
#include "Observer.h"
#include "NumberOption.h"

class Observable;
class NumberOption;

class GUINumberBox : public GUITextBox
{
    // variables
    protected:
        NumberType    mNumberType;
        NumberOption* mNumberOption;

        SInt32        mDotIndex;
        bool          mHasMinus;
    // functions
    protected:
    public:
        GUINumberBox(UInt32          aID,
                     Rectangle2D     aBounds,
                     const WString & aText,
                     NumberType      aNumberType);

        virtual ~GUINumberBox();

        // overload focus stuff to verify content
        virtual void setFocus(bool aIsFocused);

        // overload setText from TextElement to verify content
        virtual Status setText(const WString & aText);
    
        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);

        // set/get the label text
        void setValue(Float32 aValue);

        // attach a NumberOption to the box
        virtual Status setNumberOption(NumberOption* aNumberOption);

        virtual void updateFromOption();

        // implement Observer interface
        virtual Status update(const Observable* aObservable);

        virtual bool isInTabOrder(void);
};
