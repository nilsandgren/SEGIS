//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIComboBox: 
//      Subclass of GUIElement that hold a number of GUIComboBoxItems.
//
//      .------.
//      | item |
//      |------|
//      | item |
//      |------|
//      | item |
//      |------|
//      | item |
//      '------'
//

#pragma once

#include "GUIElement.h"

#include <deque>
using namespace std;

class  GUIComboBoxItem;

class GUIComboBox : public GUIElement
{
    // variables
    protected:
        GUIComboBoxItem*          mSelectedComboBoxItem;
        GUIComboBoxItem*          mFocusedComboBoxItem;
        SInt32                    mSelectedIndex;
        SInt32                    mFocusedIndex;
        deque< GUIComboBoxItem* > mComboBoxItems;
        bool                      mIsExpanded;
        Rectangle2D               mLargestChildSize;
        Color                     mTickColor;

    // functions
    protected:
        // update positions of elements in mComboBoxItems depending on mSelectedIndex
        void updateChildBounds(void);
    public:
        GUIComboBox(UInt32      aID,
                    Rectangle2D aBounds);

        GUIComboBox(UInt32        aID,
                    Rectangle2D   aBounds,
                    UInt32        aPadding,
                    SizeConstraint aSizeConstraint,
                    FillDirection aFillDirection,
                    Alignment     aAlignment);

        virtual ~GUIComboBox();

        Color getTickColor(void);
        void setTickColor(Color aColor);

        virtual void setFocus(bool aIsFocused);
        void focusNextItem(void);
        void focusPrevItem(void);

        virtual Status draw(void);

        virtual void setBounds(Rectangle2D aBounds);

        // adds a GUIComboBoxItem to mComboBoxItems
        virtual Status addChild(GUIComboBoxItem* aGUIComboBoxItem);
        virtual Status addChild(Window* aParentWindow, GUIComboBoxItem* aGUIComboBoxItem);
        // ordinary GUIElement cannot be added
        virtual Status addChild(GUIElement* aGUIElement);
        virtual Status addChild(Window* aParentWindow, GUIElement* aGUIElement);

        // removes GUIComboBoxItem from mComboBoxItems
        virtual Status removeChild(GUIComboBoxItem* aGUIComboBoxItem);

        virtual EventSignal handleKeyEvent(KeyEvent* aKeyEvent);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);

        // collapse (hide) the contained GUIComboBoxItems
        virtual void collapse(void);
        // expand (show) the contained GUIComboBoxItems
        virtual void expand(void);
        bool isExpanded(void) { return mIsExpanded; }
        // set currently selected GUIComboBoxItem
        GUIComboBoxItem* getSelectedItem(void);
        virtual SInt32 getSelectedIndex(void);
        virtual Status setSelectedItem(GUIComboBoxItem* aGUIComboBoxItem);
        virtual Status setFocusedItem(GUIComboBoxItem* aGUIComboBoxItem);

        virtual bool isInTabOrder(void) { return true; }

        virtual bool updateAnimation(void);
};
