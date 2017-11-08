//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIElement: 
//      Implementation
//

#include <SEGIS/GUIStepButton.h>
#include <SEGIS/DisplayManager.h>

GUIStepButton::GUIStepButton(UInt32        aID,
                             Rectangle2D   aBounds,
                             UInt32        aPadding,
                             SizeConstraint aSizeConstraint,
                             FillDirection aFillDirection,
                             Alignment     aAlignment)
    : GUIButton(aID,
                aBounds,
                aPadding,
                aSizeConstraint,
                aFillDirection,
                aAlignment)
    , mArrowDirection(kDirectionUp)
{
}

GUIStepButton::~GUIStepButton()
{
}

Status 
GUIStepButton::draw(void)
{
    // draw super class
    Status lStatus = GUIButton::draw();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIStepButton::draw - Drawing GUIStepButton %d\n", mID);
    #endif

    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;
    int lHalfW = lBoxW / 2;
    int lHalfH = lBoxH / 2;

    int lPressEffect = mIsPressed * mBevel;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX + lPressEffect,
                        lBoxPosY - lPressEffect,
                        lBoxW, lBoxH);

    //  | | | | | | | | | 
    //  | | | | | | | | | 
    //  | | | |1|1| | | | 
    //  | | |2|2|2|2| | | 
    //  | |3| | | | |4| | 
    //  | | | | | | | | | 
    //  | | | | | | | | | 
    //  | | | | | | | | | 

    glBegin(GL_LINES);
    {
        // draw arrow
        glColor3ub(mLabelColor.r, mLabelColor.g, mLabelColor.b);
        if (kDirectionUp == mArrowDirection)
        {
            // 1
            glVertex2i(lHalfW - 1, lHalfH + 1);
            glVertex2i(lHalfW + 1, lHalfH + 1);
            // 2
            glVertex2i(lHalfW - 2, lHalfH);
            glVertex2i(lHalfW + 2, lHalfH);
            // 3
            glVertex2i(lHalfW - 3, lHalfH - 1);
            glVertex2i(lHalfW - 2, lHalfH - 1);
            // 4
            glVertex2i(lHalfW + 2, lHalfH - 1);
            glVertex2i(lHalfW + 3, lHalfH - 1);
        }
        else if (kDirectionDown == mArrowDirection)
        {
            glVertex2i(lHalfW - 1, lHalfH - 1);
            glVertex2i(lHalfW + 1, lHalfH - 1);
            glVertex2i(lHalfW - 2, lHalfH);
            glVertex2i(lHalfW + 2, lHalfH);
            glVertex2i(lHalfW - 3, lHalfH + 1);
            glVertex2i(lHalfW - 2, lHalfH + 1);
            glVertex2i(lHalfW + 2, lHalfH + 1);
            glVertex2i(lHalfW + 3, lHalfH + 1);
        }
        // TODO GUIStepButton - Only Up/Down arrow implemented
    }
    glEnd();
    glPopMatrix();
    return lStatus;
}

void
GUIStepButton::setDirection(Direction aDirection)
{
    mArrowDirection = aDirection;
}

bool
GUIStepButton::isInTabOrder(void)
{
    return false;
}
