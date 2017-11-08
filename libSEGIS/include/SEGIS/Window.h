//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Window: 
//      A Window.
//      Can be...
//          ...resized
//          ...go fullscreen
//          ...be restored to previous size
//          ...used as an access point for GUIElements
//          ...distribute events
//

#pragma once

#include "Types.h"
#include "GUIElement.h"
#include "LayoutManager.h"

using namespace std;

class Window
{
    private: // variables
        Point2D             mWindowSize;
        GUIElement*         mGUIRootElement;
        bool                mIsFullScreen;
        Point2D             mPreviousWindowSize; // for restore after maximization
        Point2D             mMinimumSafeSize;
        SInt32              mID;
        char*               mName;

        deque<GUIElement*>  mTabElements;
        deque<GUIElement*>  mOverlayElements;
        GUIElement*         mFocusedElement;
        SInt32              mFocusedIndex;

        bool                mFirstResize;

        // (do not) allow resize to size smaller than minimum safe size
        bool                mRespectMinimumSafeSize; 

        TimeStamp           mLastAnimationTime;


    private: // functions
        EventSignal handleKeyEventInternal(KeyEvent* aKeyEvent);
        EventSignal handleMouseEvent(GUIElement* aGUIElement, 
                                     MouseEvent* aMouseEvent);
        void updateTabOrder(GUIElement* aRoot);
        void updateAnimation(GUIElement* aRoot);

        void updateMinimumSafeSize(void);

    public:
        Window(UInt32       aWidth, 
               UInt32       aHeight, 
               GUIElement*  aRoot,
               const char*  aName);
        virtual ~Window();
        
        // resize window and the contained GUIElements
        Status resize(UInt32 aWidth, UInt32 aHeight);

        // go fullscreen
        Status fullscreen(void);

        // restore size to pre-fullscreen size
        Status restore(void);

        // close the window - can this be done?
        Status close(void);

        // hide window
        Status hide(void);

        // draw all the components in the window
        Status draw(void);

        // get window size
        Point2D getSize(void);

        // get smallest possible width and height of windows before GUIElements
        // start to render incorrectly (e.g. out of bounds, on top of
        // each-other)
        UInt32 getMinimumSafeWidth(void);
        UInt32 getMinimumSafeHeight(void);
        Point2D getMinimumSafeSize(void);

        // control whether resizing to smaller size than safe size is allowed
        // or not
        bool getRespectMinimumSafeSize(void);
        void setRespectMinimumSafeSize(bool aRespectMinimumSafeSize);

        // get the root GUIElement
        GUIElement* getRootGUIElement();

        // returns the unique identifier for this window
        SInt32 getID(void);

        // tells whether Window is full screen or not
        bool isFullScreen(void)
        { 
            return mIsFullScreen;
        }

        // get the name of the window
        const char* getName(void)
        {
            return mName;   
        }

        // updates the mTabElements array of GUIElements that isInTabOrder
        virtual void updateTabOrder(void);
        virtual void addOverlayElement(GUIElement* aOverlayElement);
        virtual deque<GUIElement*> * getOverlayElements(void)
        {
            return &mOverlayElements;
        }
        virtual Status removeOverlayElement(GUIElement* aOverlayElement);

        // focus and tab order
        virtual GUIElement* getFocusedElement(void);
        virtual GUIElement* focusNextElement(void);
        virtual GUIElement* focusPrevElement(void);
        virtual void setFocusedElement(GUIElement* aGUIElement);
        virtual void setFocusColor(Color aColor);
        virtual Color getFocusColor();
        
        // event handling
        virtual EventSignal handleKeyEvent(KeyEvent* aKeyEvent);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);

        // animation stuff
        void updateAnimation(void);
        TimeStamp getLastAnimationTime(void) 
        { 
            return mLastAnimationTime; 
        }
        void setLastAnimationTime(TimeStamp aTimeStamp) 
        { 
            mLastAnimationTime = aTimeStamp;
        }
};
