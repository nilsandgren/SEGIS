//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILabel:
//      A text string in the GUI
//
//      L a b e l
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIElement.h"
#include "TextElement.h"
#include <deque>

class GUILabel : public GUIElement, public TextElement
{
    // variables
    protected:
        Color mLabelColor;

    // functions
    protected:
    public:
        GUILabel(UInt32          aID,
                 Rectangle2D     aBounds,
                 const WString & aString);

        virtual ~GUILabel();

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUIElement
        virtual Status draw(void);

        // set/get the label text
        // virtual Status setLabelText(char* aText);
        // virtual const char* getLabelText(void);

        // set/get label color
        virtual Status setLabelColor(Color aColor);
        virtual Color getLabelColor(void);

        virtual UInt32 getMinimumSafeWidth(void);
        virtual UInt32 getMinimumSafeHeight(void);
};
