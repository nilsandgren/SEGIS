//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIButton:
//      Subclass to GUIElement
//      A button with a label text. The button responds to mouse and keyboard events.
//      When clicked the GUIButton's doAction-function is executed.
//
//       ---------------
//      |\ ----------- /|
//      | |           | |
//      | | L a b e l | |
//      | |           | |
//      |/ ----------- \|
//       ---------------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIElement.h"
#include "TextElement.h"
#include <deque>

using namespace std;

class GUIButton : public GUIElement, public TextElement
{
    // variables
    private:

    protected:
        bool   mIsPressed;
        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

        Color  mLabelColor;

    // functions
    public:
        GUIButton(UInt32           aID,
                  Rectangle2D      aBounds,
                  const WString &  aLabelText);

        GUIButton(UInt32        aID,
                  Rectangle2D   aBounds,
                  UInt32        aPadding,
                  SizeConstraint aSizeConstraint,
                  FillDirection aFillDirection,
                  Alignment     aAlignment);

        virtual ~GUIButton();

        virtual void setFocus(bool aIsFocused);

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUIButton
        virtual Status draw(void);

        // the bevel of a button will always be positive
        virtual void setBevel(SInt32 aBevel);

        // set/get label color
        virtual void setLabelColor(Color aColor);
        Color getLabelColor(void);

        // do what the button is supposed to do when clicked
        virtual Status doAction();

        // force button press
        // triggers button press animation without affecting attahed option
        virtual void setPressed(bool aIsPressed);

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);
    
    protected:
        GUIButton();
};
