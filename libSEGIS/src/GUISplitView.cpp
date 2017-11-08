//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUISplitView: 
//      Implementation
//

#include <SEGIS/GUISplitView.h>
#include <SEGIS/LayoutManager.h>

#define HANDLE_BIG_SIZE 4
#define HANDLE_SMALL_SIZE 1

GUISplitView::GUISplitView(UInt32      aID,
                           Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
{
}

GUISplitView::GUISplitView(UInt32         aID,
                           Rectangle2D    aBounds,
                           UInt32         aPadding,
                           SizeConstraint aSizeConstraint,
                           FillDirection  aFillDirection,
                           Alignment      aAlignment)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
{
    if (kFillDirectionNull == aFillDirection)
    {
        fprintf(stderr, "GUISplitView::GUISplitView - Null FillDirection not allowed. Changing to Vertical.\n");
        GUIElement::setFillDirection(kFillDirectionVertical);
    }
}

GUISplitView::~GUISplitView()
{
}

EventSignal
GUISplitView::handleMouseEvent(MouseEvent* aMouseEvent)
{
    if (mChildren.size() == 2)
    {
        Rectangle2D lGenerousHandleBounds = mHandleBounds;
        lGenerousHandleBounds.x -= 2;
        lGenerousHandleBounds.y -= 2;
        lGenerousHandleBounds.w += 4;
        lGenerousHandleBounds.h += 4;
        Rectangle2D lGenerousHandleBackupBounds = mHandleBoundsBackup;
        lGenerousHandleBackupBounds.x -= 2;
        lGenerousHandleBackupBounds.y -= 2;
        lGenerousHandleBackupBounds.w += 4;
        lGenerousHandleBackupBounds.h += 4;

        // set mouse pointer type if pointer is in bounds of current handle position
        if (Util::inBounds(lGenerousHandleBounds, aMouseEvent->mMousePosition))
        {
            if (kFillDirectionHorizontal == mFillDirection)
            {
                glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
            }
            else if (kFillDirectionVertical == mFillDirection)
            {
                glutSetCursor(GLUT_CURSOR_UP_DOWN);
            }
        }

        // backup current handle position and child bounds on mouse down
        if (kMouseDown == aMouseEvent->mMouseEventType &&
            Util::inBounds(lGenerousHandleBounds, aMouseEvent->mMousePosition))
        {
            setBounds(mBounds);
            backupChildBounds();
            backupHandleBounds();
        }
        // if the mouse dragging started from the handle we should move the handle
        else if (kMouseDragged == aMouseEvent->mMouseEventType &&
                Util::inBounds(lGenerousHandleBackupBounds, aMouseEvent->mMouseOriginalPosition))
        {
            if (kFillDirectionVertical == mFillDirection)
            {
                SInt32 lDeltaDrag = aMouseEvent->mMousePosition.y - 
                                    aMouseEvent->mMouseOriginalPosition.y;

                Rectangle2D lChildBounds = mChildBoundsBackup[0];
                lChildBounds.h = mChildBoundsBackup[0].h + lDeltaDrag;
                LayoutManager::resize(mChildren[0], lChildBounds);

                lChildBounds = mChildBoundsBackup[1];
                lChildBounds.h = mChildBoundsBackup[1].h - lDeltaDrag;
                lChildBounds.y = mChildBoundsBackup[1].y + lDeltaDrag;
                LayoutManager::resize(mChildren[1], lChildBounds);
            }
            else if (kFillDirectionHorizontal == mFillDirection)
            {
                SInt32 lDeltaDrag = aMouseEvent->mMousePosition.x - 
                                    aMouseEvent->mMouseOriginalPosition.x;

                Rectangle2D lChildBounds = mChildBoundsBackup[0];
                lChildBounds.w = mChildBoundsBackup[0].w + lDeltaDrag;
                LayoutManager::resize(mChildren[0], lChildBounds);

                lChildBounds = mChildBoundsBackup[1];
                lChildBounds.w = mChildBoundsBackup[1].w - lDeltaDrag;
                lChildBounds.x = mChildBoundsBackup[1].x + lDeltaDrag;
                LayoutManager::resize(mChildren[1], lChildBounds);
            }
            updateHandleBounds();
        }
    }
    return kEventIgnored;
}

Status
GUISplitView::draw(void)
{
    GUIElement::draw();

    if (2 == mChildren.size())
    {
        UInt32 lMarkX = mHandleBounds.x - mBounds.x; // to local coords
        UInt32 lMarkY = mHandleBounds.y - mBounds.y; // to local coords
        UInt32 lMarkW = mHandleBounds.w;
        UInt32 lMarkH = mHandleBounds.h;

        glPushMatrix();
        GraphicsUtil::setViewPort2D(mBounds.x, mBounds.y, mBounds.w, mBounds.h);

        glBegin(GL_LINES);
        {
            if (kFillDirectionVertical == mFillDirection)
            {
                glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
                glVertex2i(lMarkX         , lMarkY);
                glVertex2i(lMarkX + lMarkW, lMarkY);

                glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
                glVertex2i(lMarkX         , lMarkY + lMarkH);
                glVertex2i(lMarkX + lMarkW, lMarkY + lMarkH);
            }
            else if (kFillDirectionHorizontal == mFillDirection)
            {
                glColor3ub(mColorShadow.r, mColorShadow.g, mColorShadow.b);
                glVertex2i(lMarkX, lMarkY         );
                glVertex2i(lMarkX, lMarkY + lMarkH);

                glColor3ub(mColorHighLight.r, mColorHighLight.g, mColorHighLight.b);
                glVertex2i(lMarkX + lMarkW, lMarkY);
                glVertex2i(lMarkX + lMarkW, lMarkY + lMarkH);
            }
        }
        glEnd(); 
        glPopMatrix();
    }

    return kSucceeded;
}

// makes sure at most two children are added
// children cannot have kLocked as SizeConstraint in FillDirection
Status
GUISplitView::addChild(GUIElement* aGUIElement)
{
    if (mChildren.size() > 1)
    {
        fprintf(stderr, "GUISplitView::addChild - Split view already contains two or more GUIElements\n");
        return kFailed;
    }
    if (kFillDirectionVertical == mFillDirection && 
        kLocked == aGUIElement->getSizeConstraint().h)
    {
        fprintf(stderr, "GUISplitView::addChildBack - A vertical split view cannot have locked SizeConstraint vertically.\n");
        return kFailed;
    }
    if (kFillDirectionHorizontal == mFillDirection && 
        kLocked == aGUIElement->getSizeConstraint().w)
    {
        fprintf(stderr, "GUISplitView::addChildBack - A horizontal split view cannot have locked SizeConstraint horizontally.\n");
        return kFailed;
    }
    Status lStatus = GUIElement::addChild(aGUIElement);
    if (mChildren.size() == 2)
    {
        updateHandleBounds();
    }
    return lStatus;
}

Status
GUISplitView::addChildBack(GUIElement* aGUIElement)
{
    if (mChildren.size() > 1)
    {
        fprintf(stderr, "GUISplitView::addChildBack - Split view already contains two or more GUIElements\n");
        return kFailed;
    }
    if (kFillDirectionVertical == mFillDirection && 
        kLocked == aGUIElement->getSizeConstraint().h)
    {
        fprintf(stderr, "GUISplitView::addChildBack - A vertical split view cannot have locked SizeConstraint vertically.\n");
        return kFailed;
    }
    if (kFillDirectionHorizontal == mFillDirection && 
        kLocked == aGUIElement->getSizeConstraint().w)
    {
        fprintf(stderr, "GUISplitView::addChildBack - A horizontal split view cannot have locked SizeConstraint horizontally.\n");
        return kFailed;
    }

    return GUIElement::addChildBack(aGUIElement);
}

void
GUISplitView::setBounds(Rectangle2D aBounds)
{
    GUIElement::setBounds(aBounds);
    updateHandleBounds();
}

void
GUISplitView::setFillDirection(FillDirection aFillDirection)
{
    if (kFillDirectionNull != aFillDirection)
    {
        GUIElement::setFillDirection(aFillDirection);
    }
}

void
GUISplitView::updateHandleBounds(void)
{
    UInt32 lHandleW1 = HANDLE_BIG_SIZE;
    UInt32 lHandleW2 = HANDLE_SMALL_SIZE;
    if (mChildren.size() == 2)
    {
        if (kFillDirectionHorizontal == mFillDirection)
        {
            mHandleBounds.x = mBounds.x +
                              SInt32(mChildren[0]->getBounds().w + 1.5 * mPadding - lHandleW2);
            // mHandleBounds.y = mChildren[0]->getBounds().y + mBounds.h/2 - lHandleW1;
            mHandleBounds.y = mBounds.y + SInt32((mBounds.h + 0.5)/2.0) - lHandleW1;
            mHandleBounds.w = lHandleW2 << 1;
            mHandleBounds.h = lHandleW1 << 1;
        }
        else if (kFillDirectionVertical == mFillDirection)
        {
            mHandleBounds.x = mBounds.x + SInt32((mBounds.w + 0.5)/2.0) - lHandleW1;
            mHandleBounds.y = mBounds.y + mBounds.h -
                              (mChildren[1]->getBounds().h + 1.5 * mPadding + lHandleW2 + 0.5);
            mHandleBounds.w = lHandleW1 << 1;
            mHandleBounds.h = lHandleW2 << 1;
        }
    }
    else
    // atleast do something
    {
        if (kFillDirectionVertical == mFillDirection)
        {
            mHandleBounds.x = mBounds.x + mBounds.w/2 - lHandleW1;
        }
        else if (kFillDirectionHorizontal == mFillDirection)
        {
            mHandleBounds.y = mBounds.y + mBounds.h/2 - lHandleW1;
        }
    }
}

void
GUISplitView::backupHandleBounds(void)
{
    mHandleBoundsBackup = mHandleBounds;
}

void
GUISplitView::backupChildBounds(void)
{
    if (mChildren.size() == 2)
    {
        mChildBoundsBackup[0].x = mChildren[0]->getBounds().x;
        mChildBoundsBackup[0].y = mChildren[0]->getBounds().y;
        mChildBoundsBackup[0].w = mChildren[0]->getBounds().w;
        mChildBoundsBackup[0].h = mChildren[0]->getBounds().h;
        mChildBoundsBackup[1].x = mChildren[1]->getBounds().x;
        mChildBoundsBackup[1].y = mChildren[1]->getBounds().y;
        mChildBoundsBackup[1].w = mChildren[1]->getBounds().w;
        mChildBoundsBackup[1].h = mChildren[1]->getBounds().h;
    }
}
