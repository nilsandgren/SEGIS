//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GraphicsUtil: 
//      Implementation
//

#include <SEGIS/GraphicsUtil.h>
#include <string.h>

void
GraphicsUtil::setColor(Color aColor)
{
    glColor3ub(aColor.r, aColor.g, aColor.b);
}

void 
GraphicsUtil::setViewPort2D(int aPosX, int aPosY, int aWidth, int aHeight)
{
    glViewport(aPosX, aPosY, aWidth, aHeight);
    glScalef(2.0f / aWidth, 2.0f / aHeight, 1.0);
    glTranslatef(-aWidth / 2.0f, -aHeight / 2.0f, 1.0f);
}

void 
GraphicsUtil::setViewPort2D(Rectangle2D aBounds)
{
    UInt32 lWidth = aBounds.w;
    UInt32 lHeight = aBounds.h;
    glViewport(aBounds.x, aBounds.y, lWidth, lHeight);
    glScalef(2.0f / lWidth, 2.0f / lHeight, 1.0);
    glTranslatef(-lWidth / 2.0f, -lHeight / 2.0f, 1.0f);
}

void
GraphicsUtil::drawLine(int aPosAX, int aPosAY, int aPosBX, int aPosBY, Color aColor)
{
    glColor3ub(aColor.r, aColor.g, aColor.b);
    glBegin(GL_LINES); 
    glVertex2i(aPosAX, aPosAY);
    glVertex2i(aPosBX, aPosBY);
    glEnd(); 
}

void
GraphicsUtil::drawLine(int aPosAX, int aPosAY, int aPosBX, int aPosBY)
{
    glBegin(GL_LINES); 
    glVertex2i(aPosAX, aPosAY);
    glVertex2i(aPosBX, aPosBY);
    glEnd(); 
}

void
GraphicsUtil::drawFilledRectangle(Rectangle2D aRectangle2D, Color aColor)
{
    glBegin(GL_QUADS); 
    glColor3ub(aColor.r, aColor.g, aColor.b);
    glVertex2i(aRectangle2D.x, aRectangle2D.y);
    glVertex2i(aRectangle2D.x, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y);
    glEnd();
}

void
GraphicsUtil::drawFilledRectangle(Rectangle2D aRectangle2D)
{
    glBegin(GL_QUADS); 
    glVertex2i(aRectangle2D.x, aRectangle2D.y);
    glVertex2i(aRectangle2D.x, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y);
    glEnd();
}

void
GraphicsUtil::drawRectangle(Rectangle2D aRectangle2D, Color aColor)
{
    glBegin(GL_LINE_LOOP);
    glColor3ub(aColor.r, aColor.g, aColor.b);
    glVertex2i(aRectangle2D.x, aRectangle2D.y);
    glVertex2i(aRectangle2D.x, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y);
    glEnd();
}

void
GraphicsUtil::drawRectangle(Rectangle2D aRectangle2D)
{
    glBegin(GL_LINE_LOOP); 
    glVertex2i(aRectangle2D.x, aRectangle2D.y);
    glVertex2i(aRectangle2D.x, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y + aRectangle2D.h);
    glVertex2i(aRectangle2D.x + aRectangle2D.w, aRectangle2D.y);
    glEnd();
}

void
GraphicsUtil::drawQuad(int aPosAX, int aPosAY,
                       int aPosBX, int aPosBY,
                       int aPosCX, int aPosCY,
                       int aPosDX, int aPosDY,
                       Color aColor)
{
    glBegin(GL_QUADS); 
    glColor3ub(aColor.r, aColor.g, aColor.b);
    glVertex2i(aPosAX, aPosAY);
    glVertex2i(aPosBX, aPosBY);
    glVertex2i(aPosCX, aPosCY);
    glVertex2i(aPosDX, aPosDY);
    glEnd();
}

void
GraphicsUtil::drawQuad(int aPosAX, int aPosAY,
                       int aPosBX, int aPosBY,
                       int aPosCX, int aPosCY,
                       int aPosDX, int aPosDY)
{
    glBegin(GL_QUADS); 
    glVertex2i(aPosAX, aPosAY);
    glVertex2i(aPosBX, aPosBY);
    glVertex2i(aPosCX, aPosCY);
    glVertex2i(aPosDX, aPosDY);
    glEnd();
}
