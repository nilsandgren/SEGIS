//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Util: 
//      Misc utility functions
//

#pragma once

#include "Types.h"
#include <math.h>

class Util
{
    // functions
    public:
        static bool inBounds(const Rectangle2D aRectangle, const Point2D aPoint2D);
        static void drawString(UInt32 aX, UInt32 aY, char* aString);
        static int  stringLength(const unsigned char* aString);
        static int  characterWidth(int aCharacter);
};
