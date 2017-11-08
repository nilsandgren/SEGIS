//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GraphicsUtil: 
//      Graphics utility functions
//

#pragma once

#include "Types.h"
#include <math.h>

class GraphicsUtil
{
    // functions
    public:
        static void setColor(Color aColor);
        static void setViewPort2D(int aPosX, int aPosY, int aWidth, int aHeight);
        static void setViewPort2D(Rectangle2D aBounds);

        static void drawLine(int aPosAX, int aPosAY, int aPosBX, int aPosBY, Color aColor);
        static void drawLine(int aPosAX, int aPosAY, int aPosBX, int aPosBY);

        static void drawFilledRectangle(Rectangle2D aRectangle2D, Color aColor);
        static void drawFilledRectangle(Rectangle2D aRectangle2D);

        static void drawRectangle(Rectangle2D aRectangle2D, Color aColor);
        static void drawRectangle(Rectangle2D aRectangle2D);
    
        static void drawQuad(int aPosAX, int aPosAY, 
                             int aPosBX, int aPosBY,
                             int aPosCX, int aPosCY,
                             int aPosDX, int aPosDY,
                             Color aColor);
        static void drawQuad(int aPosAX, int aPosAY, 
                             int aPosBX, int aPosBY,
                             int aPosCX, int aPosCY,
                             int aPosDX, int aPosDY);
};
