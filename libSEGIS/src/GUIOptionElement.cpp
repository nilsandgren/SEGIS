//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIOptionElement: 
//      Implementation
//


#include <SEGIS/GUIOptionElement.h>

GUIOptionElement::GUIOptionElement()
    : GUIElement()
{
	mBounds.x = 0;
	mBounds.y = 0;
	mBounds.w = 0;
	mBounds.h = 0;
}


GUIOptionElement::GUIOptionElement(UInt32      aID,
                                   Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
{
}

GUIOptionElement::GUIOptionElement(UInt32        aID,
                                   Rectangle2D   aBounds,
                                   UInt32        aPadding,
                                   SizeConstraint aSizeConstraint,
                                   FillDirection aFillDirection,
                                   Alignment     aAlignment)
    : GUIElement(aID, 
                 aBounds, 
                 aPadding, 
                 aSizeConstraint, 
                 aFillDirection, 
                 aAlignment)
{
}

GUIOptionElement::~GUIOptionElement()
{
}
