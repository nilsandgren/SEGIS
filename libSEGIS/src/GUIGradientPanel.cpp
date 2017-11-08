//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIGradientPanel:
//      Implementation
//


#include <SEGIS/GUIGradientPanel.h>

GUIGradientPanel::GUIGradientPanel(UInt32      aID,
                                   Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
{
    mColor0.r = mColor1.r = mColor2.r = mColor3.r = 0;
    mColor0.g = mColor1.g = mColor2.g = mColor3.g = 0;
    mColor0.b = mColor1.b = mColor2.b = mColor3.b = 0;
}

GUIGradientPanel::GUIGradientPanel(UInt32        aID,
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
    mColor0.r = mColor1.r = mColor2.r = mColor3.r = 0;
    mColor0.g = mColor1.g = mColor2.g = mColor3.g = 0;
    mColor0.b = mColor1.b = mColor2.b = mColor3.b = 0;
}

GUIGradientPanel::~GUIGradientPanel()
{
}

Status 
GUIGradientPanel::draw(void)
{
    Status lStatus = kSucceeded;

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

    glBegin(GL_QUADS);
    {
        glColor3ub(mColor0.r, mColor0.g, mColor0.b);
        glVertex2i(0, 0);
        glColor3ub(mColor1.r, mColor1.g, mColor1.b);
        glVertex2i(0, lBoxH);
        glColor3ub(mColor2.r, mColor2.g, mColor2.b);
        glVertex2i(lBoxW, lBoxH);
        glColor3ub(mColor3.r, mColor3.g, mColor3.b);
        glVertex2i(lBoxW, 0);
    }
    
    Color lColor1;
    Color lColor2;
    if (mBevel <= 0)
    {
        // top and left will be shadowed
        // right and bottom will be highlighted
        lColor1 = mColorShadow;
        lColor2 = mColorHighLight;
    }
    else if (mBevel > 0)
    {
        // top and left will be highlighted
        // right and bottom will be shadowed
        lColor1 = mColorHighLight;
        lColor2 = mColorShadow;
    }

    UInt32 lBevel = abs(mBevel);
    if (mBevel)
    {
        // TODO The vertex calculations can be optimized
        // draw bevel left
        glColor3ub(lColor1.r, lColor1.g, lColor1.b);
        glVertex2i(0, 0);
        glVertex2i(0, lBoxH);
        glVertex2i(lBevel, lBoxH - lBevel);
        glVertex2i(lBevel, lBevel);

        // draw bevel top
        glVertex2i(0, lBoxH);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW - lBevel, lBoxH - lBevel);
        glVertex2i(lBevel, lBoxH - lBevel);

        // draw bevel right
        glColor3ub(lColor2.r, lColor2.g, lColor2.b);
        glVertex2i(lBoxW, lBoxH);
        glVertex2i(lBoxW, 0);
        glVertex2i(lBoxW - lBevel, lBevel);
        glVertex2i(lBoxW - lBevel, lBoxH - lBevel);

        // draw bevel bottom
        glVertex2i(0, 0);
        glVertex2i(lBevel, lBevel);
        glVertex2i(lBoxW - lBevel, lBevel);
        glVertex2i(lBoxW, 0);
    }
    
    glEnd();
    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIElement::draw - Drawing GUIElement %d\n", mID);
    #endif
    
    return lStatus;
    
}

void 
GUIGradientPanel::setColor(Color aColor)
{
    GUIElement::setColor(aColor);
    mColor0.r = mColor1.r = mColor2.r = mColor3.r = aColor.r;
    mColor0.g = mColor1.g = mColor2.g = mColor3.g = aColor.g;
    mColor0.b = mColor1.b = mColor2.b = mColor3.b = aColor.b;
}

// sets the color for each vertex in the panel
void 
GUIGradientPanel::setColor(Color aColor0,
                           Color aColor1,
                           Color aColor2,
                           Color aColor3)
{
    mColor0.r = aColor0.r;
    mColor0.g = aColor0.g;
    mColor0.b = aColor0.b;

    mColor1.r = aColor1.r;
    mColor1.g = aColor1.g;
    mColor1.b = aColor1.b;

    mColor2.r = aColor2.r;
    mColor2.g = aColor2.g;
    mColor2.b = aColor2.b;

    mColor3.r = aColor3.r;
    mColor3.g = aColor3.g;
    mColor3.b = aColor3.b;
}

