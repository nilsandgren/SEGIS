//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  ColorUtil: 
//      Color utility functions
//

#pragma once

#include "Types.h"
#include <math.h>

class ColorUtil
{
    // functions
    public:
        static void convertBGR24toRGB24(UInt08* aDst, UInt08* aSrc, 
                                        UInt32 aWidth, UInt32 aHeight);
        static void convertYUV420toRGB24(UInt08* aDst, UInt08* aSrc, 
                                         UInt32 aWidth, UInt32 aHeight);
        static void convertYUV422toRGB24(UInt08* aDst, UInt08* aSrc, 
                                         UInt32 aWidth, UInt32 aHeight);
        static void convertYUV444toRGB24(UInt08* aDst, UInt08* aSrc, 
                                         UInt32 aWidth, UInt32 aHeight);
        static void convertYV12toRGB24(UInt08* aDst, UInt08* aSrc, 
                                       UInt32 aWidth, UInt32 aHeight);
};
