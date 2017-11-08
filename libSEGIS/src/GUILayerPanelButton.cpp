//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILayerPanelButton: 
//      Implementation
//

#include <SEGIS/GUILayerPanel.h>
#include <SEGIS/GUILayerPanelButton.h>

GUILayerPanelButton::GUILayerPanelButton(UInt32        aID,
                                         Rectangle2D   aBounds,
                                         UInt32        aPadding,
                                         SizeConstraint aSizeConstraint,
                                         FillDirection aFillDirection,
                                         Alignment     aAlignment)
    : GUIButton(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mLayerIndex(-1)
    , mLayerPanel(0)
{
    // default is to not animate GUILayerPanel
    mIsAnimatable = false;
}

GUILayerPanelButton::GUILayerPanelButton(UInt32        aID,
                                         Rectangle2D   aBounds)
    : GUIButton(aID, aBounds, L"")
    , mLayerIndex(-1)
    , mLayerPanel(0)
{
    // default is to not animate GUILayerPanel
    mIsAnimatable = false;
}

GUILayerPanelButton::~GUILayerPanelButton()
{
}

void
GUILayerPanelButton::setLayerPanel(GUILayerPanel* aGUILayerPanel)
{
    mLayerPanel = aGUILayerPanel;
}

void
GUILayerPanelButton::setLayerIndex(SInt32 aIndex)
{
    mLayerIndex = aIndex;
}

Status
GUILayerPanelButton::draw(void)
{
    if (mIsVisible)
    {
        int lBoxPosX = mBounds.x;
        int lBoxPosY = mBounds.y;
        int lBoxW = mBounds.w;
        int lBoxH = mBounds.h;
        int lCorner = 5;

        // draw main area if not root
        glPushMatrix();
        GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);

        glBegin(GL_POLYGON);
        {
            // draw nice looking area
            glColor3ub(mColor.r, mColor.g, mColor.b);
            glVertex2i(0, 0);
            glVertex2i(0, lBoxH);
            glVertex2i(lBoxW - lCorner, lBoxH);
            glVertex2i(lBoxW, lBoxH - lCorner);
            glVertex2i(lBoxW, 0);
        }
        glEnd();

        Color lColor1;
        Color lColor2;
        glBegin(GL_QUADS);
        {
            // draw bevel
            if (mIsPressed)
            {
                // top and left will be shadowed
                // right and bottom will be highlighted
                lColor1 = mColorShadow;
                lColor2 = mColorHighLight;
            }
            else
            {   
                // top and left will be highlighted
                // right and bottom will be shadowed
                lColor1 = mColorHighLight;
                lColor2 = mColorShadow;
            }
            // draw bevel left
            glColor3ub(lColor1.r, lColor1.g, lColor1.b);
            glVertex2i(0, 0);
            glVertex2i(0, lBoxH);
            glVertex2i(mBevel, lBoxH - mBevel);
            glVertex2i(mBevel, mBevel);

            // draw bevel top
            glVertex2i(0, lBoxH);
            glVertex2i(lBoxW - lCorner, lBoxH);
            glVertex2i(lBoxW - lCorner - mBevel, lBoxH - mBevel);
            glVertex2i(mBevel, lBoxH - mBevel);

            // draw bevel right
            glColor3ub(lColor2.r, lColor2.g, lColor2.b);
            glVertex2i(lBoxW, lBoxH - lCorner);
            glVertex2i(lBoxW, 0);
            glVertex2i(lBoxW - mBevel, mBevel);
            glVertex2i(lBoxW - mBevel, lBoxH - lCorner - mBevel);

            if (mLayerIndex == mLayerPanel->getSelectedLayerIndex())
            {
                glColor3ub(mColor.r, mColor.g, mColor.b);
            }

            // draw bevel bottom
            glVertex2i(0, 0);
            glVertex2i(mBevel, mBevel);
            glVertex2i(lBoxW - mBevel, mBevel);
            glVertex2i(lBoxW, 0);
        }
        glEnd();

        // draw mLabelText
        // if (mLabelText)
        {
            UInt32 lPressEffect = 0;
            if (mIsPressed)
            {
                lPressEffect = mBevel;
            }
            // XXX No check whether text fits
            glColor3ub(mLabelColor.r, mLabelColor.g, mLabelColor.b);
            // Util::drawString(mBevel + 5 + lPressEffect, (mBounds.h>>1) - 4 - lPressEffect, mLabelText);
            TextElement::drawText(mBevel + 5 + lPressEffect, (mBounds.h>>1) - 4 - lPressEffect);
        }
        glPopMatrix();

        // a bit of a hack here:
        // if this is the selected button we paint a line over
        // the edge between the button and the layer below
        if (mLayerIndex == mLayerPanel->getSelectedLayerIndex())
        {
            glPushMatrix();
            GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY - 1, lBoxW, lBoxH);
            glBegin(GL_LINES);
            {
                glColor3ub(lColor1.r, lColor1.g, lColor1.b);
                glVertex2i(0, 1);
                glVertex2i(1, 1);
                glColor3ub(mColor.r, mColor.g, mColor.b);
                glVertex2i(1, 0);
                glVertex2i(lBoxW, 0);
            }
            glEnd();
            glPopMatrix();
        }
    }
    return kSucceeded;
}

Status
GUILayerPanelButton::doAction(void)
{
    if (mLayerPanel && mLayerIndex >= 0)
    {
        return mLayerPanel->setSelectedLayer(mLayerIndex);
    }
    return kFailed;
}

bool
GUILayerPanelButton::isInTabOrder(void)
{
    return mIsInTabOrder;
}
