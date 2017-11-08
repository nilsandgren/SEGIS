//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIElement:
//      Base class for GUI elements
//      The GUIElement is a rectangular element in the GUI that can contain other (more
//      advanced) elements - the children.
//
//      Padding
//      =======
//      The Padding of a GUIElement determines how many pixels of padding will be added
//      around each of the contained children.
//
//      Bevel
//      =====
//      The Bevel is specified in pixels and gives a 3D-look to the GUIElement by adding
//      highlight and shadow to the border of the GUIElement. A positive Bevel value will
//      give the impression that the GUIElement rises out of the GUI and vice versa.
//
//      FillDirection
//      =============
//      The GUIElement's FillDirection determines how the childred contained in this
//      GUIElement will be positioned.
//
//      kFillDirectionHorizontal:
//       -------------------------
//      | ----------------------- |
//      || child | child | child ||
//      | ----------------------- |
//        -----------------------
//
//      kFillDirectionVertical:
//       ---------
//      | ------- |
//      || child ||
//      | ------- |
//      | ------- |
//      || child ||
//      | ------- |
//      | ------- |
//      || child ||
//      | ------- |
//       ---------
//
//      SizeConstraint
//      =============
//      The SizeConstraint determines when the GUIElement can be resized.
//
//      kAuto:
//      GUIElements with SizeConstraint kAuto in a Direction (w or h) will automatically
//      fill up the parents area of the GUI in that Direction.
//
//      kLocked:
//      GUIElements with kLocked SizeConstraint in a Direction will not be resized in that
//      direction.
//
//      kChangable:
//      GUIElements with kChangable will not change it's size automatically when parent's
//      size is changed, but is allowed to be resized by other means such mouse dragging
//      or other kind of user interaction.
//
//       ------------------
//      | ---------------- |<---- bevel
//      ||                ||
//      ||                ||
//      ||                ||
//      ||                ||
//      ||                ||
//      | ---------------- |
//       ------------------
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "Option.h"
#include "Animatable.h"

#include <deque>
using namespace std;

class Window;

typedef enum
{
    kLocked,    // this property cannot be changed
    kChangable, // this property can be changed, but will not change automatically
    kAuto       // this property changes automatically
} Constraint;

typedef struct
{
    Constraint w;
    Constraint h;
} SizeConstraint;

typedef enum
{
    kDirectionUp,
    kDirectionDown,
    kDirectionLeft,
    kDirectionRight,
    kDirectionBack,
    kDirectionForth
} Direction;

typedef enum
{
    kFillDirectionVertical,
    kFillDirectionHorizontal,
    kFillDirectionNull
} FillDirection;

typedef enum
{
    kLeft,
    kRight,
    kHCenter
} HorizontalAlignment;

typedef enum
{
    kTop,
    kBottom,
    kVCenter
} VerticalAlignment;

typedef struct
{
    HorizontalAlignment mHAlign;
    VerticalAlignment   mVAlign;
} Alignment;

class GUIElement : public Animatable
{
    // variables
    protected:
        Rectangle2D         mBounds;
        SInt32              mPadding;       // this will be added around all children
        SInt32              mBevel;         // cannot be bigger than mPadding
        SizeConstraint      mSizeConstraint;

        Color               mColor;
        // bevel color
        Color               mColorShadow;
        Color               mColorHighLight;
        GLfloat             mBrightDiff;
        GLfloat             mDarkDiff;
        // the color focused elements are marked with
        Color               mFocusColor;

        SInt32              mID;

        FillDirection       mFillDirection; // kFillDirectionVertical: rows   kFillDirectionHorizontal: cols
        Alignment           mAlignment;     // alignment of children
        deque<GUIElement*>  mChildren;      // GUIElements contained within this one
        GUIElement*         mParent;

        bool                mIsFocused;
        bool                mIsVisible;
        bool                mIsAnimatable;
        bool                mIsInTabOrder;

        Window*             mParentWindow;

    public:
        static Color        kDefaultFocusColor;

    // functions
    protected:
        GUIElement();
        Status drawFocusBorder();

    public:
        // GUIElement with aBounds size
        // Padding        will be 0
        // SizeConstraint will be kAuto
        // FillDirection  will be kFillDirectionHorizontal
        // Alignment      will be kTop,kLeft
        // Color          will be nice
        GUIElement(UInt32      aID,
                   Rectangle2D aBounds);

        GUIElement(UInt32         aID,
                   Rectangle2D    aBounds,
                   UInt32         aPadding,
                   SizeConstraint aSizeConstraint,
                   FillDirection  aFillDirection,
                   Alignment      aAlignment);

        virtual ~GUIElement();

        // get/set
        const Rectangle2D getBounds(void)
        {
            return mBounds;
        }
        virtual void setBounds(Rectangle2D aBounds)
        {
            mBounds.x = aBounds.x;
            mBounds.y = aBounds.y;
            mBounds.w = aBounds.w;
            mBounds.h = aBounds.h;
        }

        SInt32 getBevel(void)
        {
            return mBevel;
        }
        virtual void setBevel(SInt32 aBounds);

        UInt32 getPadding(void)
        {
            return mPadding;
        }
        virtual void setPadding(UInt32 aPadding)
        {
            mPadding = aPadding;
        }

        const SizeConstraint getSizeConstraint(void)
        {
            return mSizeConstraint;
        }
        virtual void setSizeConstraint(SizeConstraint aSizeConstraint)
        {
            mSizeConstraint = aSizeConstraint;
        }

        const Color getColor(void)
        {
            return mColor;
        }
        virtual void setColor(Color aColor);

        const Color getFocusColor(void)
        {
            return mFocusColor;
        }
        // set focus color for this element. if aRecurse is true then focus
        // color propagates to the children aswell.
        virtual void setFocusColor(Color aColor, bool aRecurse);

        UInt32 getID(void)
        {
            return mID;
        }
        void setID(UInt32 aID)
        {
            mID = aID;
        }

        FillDirection getFillDirection(void)
        {
            return mFillDirection;
        }
        virtual void setFillDirection(FillDirection aFillDirection)
        {
            mFillDirection = aFillDirection;
        }

        Alignment getAlignment(void)
        {
            return mAlignment;
        }
        virtual void setAlignment(Alignment aAlignment)
        {
            mAlignment = aAlignment;
        }

        // child-parent relation
        GUIElement* getParent(void)
        {
            return mParent;
        }
        virtual void setParent(GUIElement* aParent)
        {
            mParent = aParent;
        }

        deque<GUIElement*>* getChildren(void)
        {
            return &mChildren;
        }

        Window* getParentWindow(void)
        {
            return mParentWindow;
        }
        void setParentWindow(Window* aWindow)
        {
            if (!aWindow)
            {
                fprintf(stderr, "GUIElement::setParentWindow - argument Window is NULL\n");
            }
            mParentWindow = aWindow;
        }

        // Right now the EventManager will set a GUIElement as focused when the GUIElement
        // accepts a MouseEvent. this might not be so smart in all cases.
        // Can the GUIElement take the focus itself?
        bool isFocused(void)
        {
            return mIsFocused;
        }
        virtual void setFocus(bool aIsFocused);

        bool isVisible(void)
        {
            return mIsVisible;
        }
        virtual void setVisible(bool aIsVisible);

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUIElement
        virtual Status draw(void);

        // draws this GUIElement and its children
        Status drawTree(void);

        // add child
        virtual Status addChild(GUIElement* aGUIElement);
        virtual Status addChild(Window* aParentWindow, GUIElement* aGUIElement);
        virtual Status addChildBack(GUIElement* aGUIElement);

        // remove child - assumes that only one pointer to the child exists in array
        virtual Status removeChild(GUIElement* aGUIElement);

        // delete a GUIElement and all its children
        static Status removeTree(GUIElement* aRoot);

        // returns true if this GUIElement fits inside its parent
        virtual bool fitsInParent(void);

        // returns the smallest suitable width/height for this element by
        // recursively finding minimum width/height of all of the children.
        virtual UInt32 getMinimumSafeWidth(void);
        virtual UInt32 getMinimumSafeHeight(void);

        // returns true if this type of GUIElement is included in tab order
        virtual bool isInTabOrder(void);

        // animation stuff
        virtual bool isAnimatable(void) { return mIsAnimatable; }
        virtual void setAnimatable(bool aIsAnimatable) { mIsAnimatable = aIsAnimatable; }
        // implement Animatable interface (do nothing)
        virtual bool updateAnimation(void) { return false; }
};
