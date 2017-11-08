//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Util: 
//      Implementation
//

#include <SEGIS/Util.h>
#include <SEGIS/GraphicsUtil.h>
#include <string.h>

bool 
Util::inBounds(const Rectangle2D aRectangle, const Point2D aPoint2D)
{
    return (aPoint2D.x <= aRectangle.x + aRectangle.w &&
            aPoint2D.x >= aRectangle.x &&
            aPoint2D.y <= aRectangle.y + aRectangle.h &&
            aPoint2D.y >= aRectangle.y);
}

void 
Util::drawString(UInt32 aX, UInt32 aY, char* aString)
{
    glRasterPos2i(aX, aY);
    char* lStringPtr;
    for (lStringPtr = aString; *lStringPtr != '\0'; lStringPtr++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *lStringPtr);
        // glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *lStringPtr);
    }
}

int
Util::stringLength(const unsigned char* aString)
{
    return glutBitmapLength(GLUT_BITMAP_HELVETICA_10, aString);
    // return glutBitmapLength(GLUT_BITMAP_8_BY_13, aString);
}

int 
Util::characterWidth(int aCharacter)
{
    return glutBitmapWidth(GLUT_BITMAP_HELVETICA_10, aCharacter);
}
