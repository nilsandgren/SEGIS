//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  LayoutManager: 
//      Implementation
//

#include <SEGIS/LayoutManager.h>

LayoutManager::LayoutManager()
{
}

LayoutManager::~LayoutManager()
{
}

void 
LayoutManager::resize(GUIElement* aRoot, Rectangle2D aNewBounds, bool aForceResize)
{
    // we stop the resizing of this GUIElement and it's children
    // if the size and position is the same as before
    Rectangle2D lOldBounds = aRoot->getBounds();
    if (!aForceResize && 
        aNewBounds.x == lOldBounds.x &&
        aNewBounds.y == lOldBounds.y &&
        aNewBounds.w == lOldBounds.w &&
        aNewBounds.h == lOldBounds.h)
    {
        #ifdef DEBUG_LAYOUTMANAGER
        fprintf(stderr, "LayoutManager::resize - size same as previous. stopping resize.\n");
        #endif
        return;
    }
    
    aRoot->setBounds(aNewBounds);

    #ifdef DEBUG_LAYOUTMANAGER
    fprintf(stderr, 
            "elem %d: (%d,%d)@(%d,%d)\n",
            aRoot->getID(),
            (int)aNewBounds.w,
            (int)aNewBounds.h,
            (int)aNewBounds.x,
            (int)aNewBounds.y);
    #endif

    if (aRoot->getChildren()->size() > 0)
    {
        if (kFillDirectionHorizontal == aRoot->getFillDirection())
        {
            LayoutManager::resizeChildrenFillDirectionHorizontal(aRoot, aNewBounds, aForceResize);
        }
        else if (kFillDirectionVertical == aRoot->getFillDirection())
        {
            LayoutManager::resizeChildrenFillDirectionVertical(aRoot, aNewBounds, aForceResize);
        }
        else if (kFillDirectionNull == aRoot->getFillDirection())
        {
            LayoutManager::resizeChildrenFillDirectionNull(aRoot, aNewBounds, aForceResize);
        }
    }
}

void
LayoutManager::resizeChildrenFillDirectionHorizontal(GUIElement* aRoot, Rectangle2D aNewBounds, bool aForceResize)
{
    UInt32 lReservedWidth = 0;       // width of children with kAuto != SizeConstraint + padding
    UInt32 lNumAutoSizeChildren = 0; // number of children with kAuto == SizeConstraint
    UInt32 lNumChildren = aRoot->getChildren()->size();

    const UInt32 lParentX = aNewBounds.x;
    const UInt32 lParentY = aNewBounds.y;
    const UInt32 lParentH = aNewBounds.h;
    const UInt32 lParentW = aNewBounds.w;
    const UInt32 lPadding = aRoot->getPadding();
    const HorizontalAlignment lHAlign = aRoot->getAlignment().mHAlign;
    const VerticalAlignment lVAlign = aRoot->getAlignment().mVAlign;

    Rectangle2D lChildBounds = {0, 0, 0, 0};

    auto lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        // check how many kAuto size children there are in horizontal direction
        if (kAuto == (*lChild)->getSizeConstraint().w)
        {
            lNumAutoSizeChildren++;
        }
        else
        {
            lReservedWidth += (*lChild)->getBounds().w;
        }
    }

    lReservedWidth = (lNumChildren + 1) * lPadding + lReservedWidth;

    UInt32 lChildPosX = lParentX + lPadding;

    // if there are no children with kAuto SizeConstraint in this FillDirection
    // then we nust calculate were to begin drawing children in horizontal direction. 
    // otherwise we will always begin at padding of parent.
    if (0 == lNumAutoSizeChildren)
    {
        switch (lHAlign)
        {
            case kLeft:
                lChildPosX = lParentX + lPadding;
                break;
            case kHCenter:
                lChildPosX = lParentX + (lParentW - lReservedWidth) / 2;
                break;
            case kRight:
                lChildPosX = lParentX + lParentW - lReservedWidth;
                break;
        }
    }

    UInt32 lAutoWidth = 0;
    UInt32 lAutoRest = 0;
    if (lNumAutoSizeChildren)
    {
        lAutoWidth = (lParentW - lReservedWidth) / lNumAutoSizeChildren;
        lAutoRest = (lParentW - lReservedWidth) - lNumAutoSizeChildren * lAutoWidth;
    }

    // rewind iterator
    lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        lChildBounds = (*lChild)->getBounds();

        lChildBounds.x = lChildPosX;

        // resize child if kAuto
        if (kAuto == (*lChild)->getSizeConstraint().w)
        {
            lChildBounds.w = lAutoWidth + lAutoRest;
            lAutoRest = 0;
        }

        if (kAuto == (*lChild)->getSizeConstraint().h)
        {
            lChildBounds.h = lParentH - (lPadding<<1);
            lChildBounds.y = lParentY + lPadding;
        }
        else
        {
            switch (lVAlign)
            {
                case kTop:
                    lChildBounds.y = lParentY + lParentH - lChildBounds.h - lPadding;
                    break;
                case kVCenter:
                    lChildBounds.y = lParentY + (lParentH - lChildBounds.h) / 2;
                    break;
                case kBottom:
                    lChildBounds.y = lParentY + lPadding;
                    break;
            }
            
        }

        lChildPosX += lChildBounds.w + lPadding;

        resize((*lChild), lChildBounds, aForceResize);
    }
}

void
LayoutManager::resizeChildrenFillDirectionVertical(GUIElement* aRoot, Rectangle2D aNewBounds, bool aForceResize)
{
    UInt32 lReservedHeight = 0;      // height of children with kAuto != SizeConstraint + padding
    UInt32 lNumAutoSizeChildren = 0; // number of children with kAuto == SizeConstraint
    UInt32 lNumChildren = aRoot->getChildren()->size();

    const UInt32 lParentX = aNewBounds.x;
    const UInt32 lParentY = aNewBounds.y;
    const UInt32 lParentH = aNewBounds.h;
    const UInt32 lParentW = aNewBounds.w;
    const UInt32 lPadding = aRoot->getPadding();
    const HorizontalAlignment lHAlign = aRoot->getAlignment().mHAlign;
    const VerticalAlignment lVAlign = aRoot->getAlignment().mVAlign;

    Rectangle2D lChildBounds = {0, 0, 0, 0};

    auto lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        // check how many kAuto size children there are in vertical direction
        if (kAuto == (*lChild)->getSizeConstraint().h)
        {
            lNumAutoSizeChildren++;
        }
        else
        {
            lReservedHeight += (*lChild)->getBounds().h;
        }
    }

    lReservedHeight = (lNumChildren + 1) * lPadding + lReservedHeight;

    UInt32 lChildPosY = lParentY + lPadding;

    // if there are no children with kAuto SizeConstraint in this FillDirection
    // then we nust calculate were to begin drawing children in vertical direction. 
    // otherwise we will always begin at padding of parent.
    if (0 == lNumAutoSizeChildren)
    {
        switch (lVAlign)
        {
            case kBottom:
                lChildPosY = lParentY + lPadding;
                break;
            case kVCenter:
                lChildPosY = lParentY + (lParentH - lReservedHeight) / 2;
                break;
            case kTop:
                lChildPosY = lParentY + lParentH - lReservedHeight;
                break;
        }
    }

    UInt32 lAutoHeight = 0;
    UInt32 lAutoRest = 0;
    if (lNumAutoSizeChildren)
    {
        lAutoHeight = (lParentH - lReservedHeight) / lNumAutoSizeChildren;
        lAutoRest = (lParentH - lReservedHeight) - lNumAutoSizeChildren * lAutoHeight;
    }

    // rewind iterator
    lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        lChildBounds = (*lChild)->getBounds();

        lChildBounds.y = lChildPosY;

        // resize child if kAuto
        if (kAuto == (*lChild)->getSizeConstraint().h)
        {
            lChildBounds.h = lAutoHeight + lAutoRest;
            lAutoRest = 0;
        }

        if (kAuto == (*lChild)->getSizeConstraint().w)
        {
            lChildBounds.w = lParentW - (lPadding<<1);
            lChildBounds.x = lParentX + lPadding;
        }
        else
        {
            switch (lHAlign)
            {
                case kLeft:
                    lChildBounds.x = lParentX + lPadding;
                    break;
                case kHCenter:
                    lChildBounds.x = lParentX + (lParentW - lChildBounds.w) / 2;
                    break;
                case kRight:
                    lChildBounds.x = lParentX + lParentW - lChildBounds.w - lPadding;
                    break;
            }
        }

        lChildPosY += lChildBounds.h + lPadding;

        resize((*lChild), lChildBounds, aForceResize);
    }
}

void
LayoutManager::resizeChildrenFillDirectionNull(GUIElement* aRoot, Rectangle2D aNewBounds, bool aForceResize)
{
    // translate childrens position according to aRoot's translation.
    // children should not be moved/scaled according to parents size
    Rectangle2D lRootBounds = aRoot->getBounds();

    const SInt32 lTranslateX = aNewBounds.x - lRootBounds.x;
    const SInt32 lTranslateY = aNewBounds.y - lRootBounds.y;

    Rectangle2D lChildBounds = {0, 0, 0, 0};

    auto lChild = aRoot->getChildren()->begin();
    for (; lChild != aRoot->getChildren()->end(); ++lChild)
    {
        lChildBounds = (*lChild)->getBounds();
        lChildBounds.x += lTranslateX;
        lChildBounds.y += lTranslateY;
        resize((*lChild), lChildBounds, aForceResize);
    }
}
