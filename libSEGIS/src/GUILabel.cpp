//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILabel:
//      Implementation
//

#include <SEGIS/GUILabel.h>
#include <string.h>

GUILabel::GUILabel(UInt32          aID,
                   Rectangle2D     aBounds,
                   const WString & aString)
    : GUIElement(aID, aBounds)
    , TextElement()
{
    // we default to white for text - this is a dark GUI :)
    mLabelColor.r = 255;
    mLabelColor.g = 255;
    mLabelColor.b = 255;
    
    TextElement::setText(aString);
}

GUILabel::~GUILabel()
{
}

EventSignal GUILabel::handleKeyEvent(KeyEvent*)
{
    return kEventIgnored;
}

EventSignal GUILabel::handleMouseEvent(MouseEvent*)
{
    return kEventIgnored;
}

// draws the GUILabel string
Status 
GUILabel::draw(void)
{
    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;

    glPushMatrix();
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH + 4);

    glColor3ub(mLabelColor.r, mLabelColor.g, mLabelColor.b);
    TextElement::drawText(mPadding, (mBounds.h>>1) - 4);
    
    glPopMatrix();

    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUILabel::draw - Drawing GUILabel %d\n", mID);
    #endif
    
    return kSucceeded;
}

Status 
GUILabel::setLabelColor(Color aColor)
{
    mLabelColor.r = aColor.r;
    mLabelColor.g = aColor.g;
    mLabelColor.b = aColor.b;
    return kSucceeded;
}

Color 
GUILabel::getLabelColor(void)
{
    return mLabelColor;
}

UInt32
GUILabel::getMinimumSafeWidth(void)
{
    TextRenderer * lTextRenderer = FontController::getFontController()->getTextRenderer(mRendererType);
    return lTextRenderer->getStringWidth(mTextString);
}

UInt32
GUILabel::getMinimumSafeHeight(void)
{
    return mBounds.h;
}

