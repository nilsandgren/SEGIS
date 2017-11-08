//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIFileBrowser:
//    Displays a file browser UI using boost::filesystem
//    Since SEGIS so far lacks a scrollviews this is handled internally by this class
//    
//    Double-click of a folder opens that folder in the GUIFileBrowser recursively
//    Selection or opening of files or folders is reported to Observers
//

#pragma once

#include "Types.h"
#include "Observable.h"
#include "Observer.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "GUITextBox.h"

#include <vector>

class GUIFileBrowser : public GUIElement, public Observable
{
    public:
        enum FileAction
        {
            kFileAction_None,
            kFileAction_FileSelected,
            kFileAction_DirectorySelected,
            kFileAction_FileOpened,
            kFileAction_DirectoryOpened
        };
    
        class FileListArea : public GUIElement
        {
            public:
                FileListArea(UInt32           aID,
                             Rectangle2D      aBounds,
                             UInt32           aPadding,
                             SizeConstraint   aSizeConstraint,
                             FillDirection    aFillDirection,
                             Alignment        aAlignment,
                             GUIFileBrowser * aFileBrowser);
                ~FileListArea();
                
                Status clear(); // clears the list
                Status addPath(Path * aPath); // add a directory entry
                Status scroll(Direction aDirection); // scrolls the view
                Status moveSelection(Direction aDirection);
                Status setSelectedIndex(SInt32 aIndex);
                
                // GUIElement overloading
                Status draw();
                EventSignal handleKeyEvent(KeyEvent * aKeyEvent);
                EventSignal handleMouseEvent(MouseEvent * aMouseEvent);
                bool isInTabOrder() { return true; }
            
            protected:
                GUIFileBrowser * mFileBrowser;
                
                vector<Path*> mDirectoryContents;
                SInt32 mFirstElementIndex;     // index of first element to be displayed
                SInt32 mSelectedElementIndex;  // index of selected element
                SInt32 mListElementHeight;
                
        };
    
    
        // Button for loading path in text field
        class LoadPathButton : public GUIButton
        {
            public:
                LoadPathButton(Rectangle2D aBounds, GUIFileBrowser * aFileBrowser);
                ~LoadPathButton();
               
            protected:
                Status doAction();
                
            private:
                GUIFileBrowser * mFileBrowser;
        };
    
        // Button for scrolling up or down
        class ScrollButton : public GUIButton
        {
            public:
                ScrollButton(Rectangle2D aBounds,
                             GUIFileBrowser * aFileBrowser,
                             FileListArea * aFileListArea);
                ~ScrollButton();
            
                Status setDirection(Direction aDirection);
            
            protected:
                Status doAction();
        
            private:
                GUIFileBrowser * mFileBrowser;
                FileListArea * mFileListArea;
                Direction mDirection;
        };
        
        // Button for navigating to parent directory
        class ParentDirButton : public GUIButton
        {
            public:
                ParentDirButton(Rectangle2D aBounds, GUIFileBrowser * aFileBrowser);
                ~ParentDirButton();
            
            protected:
                Status doAction();
            
            private:
                GUIFileBrowser * mFileBrowser;
        };
    
        // Button for navigating browsing history
        class HistoryButton : public GUIButton
        {
            public:
                HistoryButton(Rectangle2D aBounds, GUIFileBrowser * aFileBrowser);
                ~HistoryButton();
            
            public:    
                Status setDirection(Direction aDirection);
            protected:
                Status doAction();
                
            private:
                GUIFileBrowser * mFileBrowser;
                Direction mDirection;
        };
    
        static GUIFileBrowser * create(UInt32 aID,
                                       Rectangle2D aBounds,
                                       Path & aDirPath,
                                       GUIElement * aParent);
        ~GUIFileBrowser();

        Status addObserver(Observer * aObserver);
        Status removeObserver(Observer * aObserver);
        
        FileAction getLastFileAction();
        const Path & getCurrentPath();  // i.e. last opened directory
    
        // Status scroll(Direction aDirection);
        // Status moveSelection(Direction aDirection);
    
        Status goToParentDirectory();
        Status loadTextBoxPath();
        Status navigateHistory(Direction aDirection);
    
        Status draw();
    
    protected:
        GUIFileBrowser(UInt32 aID,
                       Rectangle2D aBounds,
                       Path & aDirPath,
                       GUIElement * aParent);
        Status load(Path & aPath);
        Status reload();
    
        vector<Observer *> mObservers;
        
        FileAction mLastFileAction;
        Path mPath;
    
        ScrollButton * mScrollUpButton;
        ScrollButton * mScrollDownButton;

        // text field for path and a button for loading it
        GUIElement * mPathArea;
        GUITextBox * mPathBox;
        LoadPathButton * mLoadPathButton;
    
        // buttons for history and going to parent directory
        GUIElement * mButtonArea;
        HistoryButton * mHistoryBackButton;
        HistoryButton * mHistoryForwardButton;
        ParentDirButton * mParentDirButton;
        
        FileListArea * mFileListArea;
};
