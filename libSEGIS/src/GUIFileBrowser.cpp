/*
 *  GUIFileBrowser.cpp
 *  SEGIS
 *
 *  Created by Nils Andgren on 26/4 -11.
 *  Copyright 2011 Nils Andgren. All rights reserved.
 *
 */


#include <SEGIS/GUIFileBrowser.h>
#include <SEGIS/LayoutManager.h>
#include <SEGIS/Window.h>
#include <SEGIS/FontController.h>
#include <SEGIS/GraphicsUtil.h>

////////////////////////////////////////////////
//
//    ScrollButton
//
////////////////////////////////////////////////

GUIFileBrowser::ScrollButton::ScrollButton(Rectangle2D aBounds,
                                           GUIFileBrowser * aFileBrowser,
                                           FileListArea * aFileListArea)
    : mFileBrowser(aFileBrowser)
    , mFileListArea(aFileListArea)
    , mDirection(kDirectionUp)
{
    assert(aFileBrowser);
    if (!aFileBrowser)
    {
        fprintf(stderr, "GUIFileBrowser::ScrollButton::ScrollButton - NULL pointer passed as pointer to GUIFileBrowser\n");
    }
    
    assert(aFileListArea);
    if (!aFileListArea)
    {
        fprintf(stderr, "GUIFileBrowser::ScrollButton::ScrollButton - NULL pointer passed as pointer to FileListArea\n");
    }
    
    SizeConstraint lSizeConstraint = { kAuto, kLocked };
    GUIButton(0, aBounds, L"^");
    setParent(aFileBrowser);
    setSizeConstraint(lSizeConstraint);
    setPadding(2);
    setBevel(1);
    setBounds(aBounds);
    mIsInTabOrder = true;
}

GUIFileBrowser::ScrollButton::~ScrollButton()
{
}

Status
GUIFileBrowser::ScrollButton::setDirection(Direction aDirection)
{
    if (kDirectionUp == aDirection ||
        kDirectionDown == aDirection)
    {
        mDirection = aDirection;
        if (kDirectionUp == aDirection)
        {
            setText(L"^");
        }
        else
        {
            setText(L"v");
        }
        return kSucceeded;
    }
    return kFailed;
}


Status
GUIFileBrowser::ScrollButton::doAction()
{
    return mFileListArea->scroll(mDirection);
}
            



////////////////////////////////////////////////
//
//    ParentDirButton
//
////////////////////////////////////////////////

GUIFileBrowser::ParentDirButton::ParentDirButton(Rectangle2D aBounds, GUIFileBrowser * aFileBrowser)
    : mFileBrowser(aFileBrowser)
{
    assert(aFileBrowser);
    if (!aFileBrowser)
    {
        fprintf(stderr, "GUIFileBrowser::ParentDirButton::ParentDirButton - NULL pointer passed as pointer to parent GUIFileBrowser\n");
    }
    SizeConstraint lSizeConstraint = {kLocked, kAuto};
    GUIButton(0, aBounds, L"^");
    setText(L"^");
    setParent(aFileBrowser);
    setSizeConstraint(lSizeConstraint);
    setBounds(aBounds);
    setPadding(2);
    setBevel(1);
    mIsInTabOrder = true;
}

GUIFileBrowser::ParentDirButton::~ParentDirButton()
{
    mFileBrowser = 0;
}

Status
GUIFileBrowser::ParentDirButton::doAction()
{
    return mFileBrowser->goToParentDirectory();
}



////////////////////////////////////////////////
//
//    LoadPathButton
//
////////////////////////////////////////////////

GUIFileBrowser::HistoryButton::HistoryButton(Rectangle2D aBounds,
                                             GUIFileBrowser * aFileBrowser)
    : mFileBrowser(aFileBrowser)
    , mDirection(kDirectionBack)
{
    assert(aFileBrowser);
    if (!aFileBrowser)
    {
        fprintf(stderr, "GUIFileBrowser::HistoryButton::HistoryButton - NULL pointer passed as pointer to parent GUIFileBrowser\n");
    }
    
    SizeConstraint lSizeConstraint = {kLocked, kAuto};
    GUIButton(0, aBounds, L"<");
    setText(L"<");
    setParent(aFileBrowser);
    setSizeConstraint(lSizeConstraint);
    setBounds(aBounds);
    setPadding(2);
    setBevel(1);
    mIsInTabOrder = true;
}

Status
GUIFileBrowser::HistoryButton::setDirection(Direction aDirection)
{
    if (kDirectionBack == aDirection)
    {
        mDirection = aDirection;
        setText(L"<");
        return kSucceeded;
    }
    else if (kDirectionForth == aDirection)
    {
        mDirection = aDirection;
        setText(L">");
        return kSucceeded;
    }
    return kFailed;
        
}

GUIFileBrowser::HistoryButton::~HistoryButton()
{
    mFileBrowser = 0;
}

Status
GUIFileBrowser::HistoryButton::doAction()
{
    return mFileBrowser->navigateHistory(mDirection);
}

////////////////////////////////////////////////
//
//    LoadPathButton
//
////////////////////////////////////////////////
GUIFileBrowser::FileListArea::FileListArea(UInt32           aID,
                                           Rectangle2D      aBounds,
                                           UInt32           aPadding,
                                           SizeConstraint   aSizeConstraint,
                                           FillDirection    aFillDirection,
                                           Alignment        aAlignment,
                                           GUIFileBrowser * aFileBrowser)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mFileBrowser(aFileBrowser)
    , mFirstElementIndex(0)
    , mSelectedElementIndex(-1)
    , mListElementHeight(20)
{
    mIsInTabOrder = true;
}

GUIFileBrowser::FileListArea::~FileListArea()
{
    mFileBrowser = 0;
}

Status
GUIFileBrowser::FileListArea::clear()
{
    auto lIter = mDirectoryContents.begin();
    for (; lIter != mDirectoryContents.end(); ++lIter)
    {
        delete (*lIter);
    }
    mDirectoryContents.clear();
    mFirstElementIndex = -1;
    mSelectedElementIndex = -1;
    return kSucceeded;
}

Status
GUIFileBrowser::FileListArea::addPath(Path * aPath)
{
    mDirectoryContents.push_back(aPath);
    if (-1 == mFirstElementIndex)
    {
        mFirstElementIndex = 0;
    }
    return kSucceeded;
}

Status
GUIFileBrowser::FileListArea::scroll(Direction aDirection)
{
    int i = mFirstElementIndex;
    if (mDirectoryContents.size())
    {
        if (kDirectionUp == aDirection)
        {
            i--;
            if (i < 0)
            {
                i = 0;
            }
        }
        else
        {
            i++;
            if (i >= SInt32(mDirectoryContents.size()))
            {
                i = mDirectoryContents.size() - 1;
            }
        }
    }
    else
    {
        i = 0;
    }
    mFirstElementIndex = i;
    
    return kSucceeded;
}

Status
GUIFileBrowser::FileListArea::moveSelection(Direction aDirection)
{
    if (!mDirectoryContents.size())
    {
        return kSucceeded;
    }
    
    SInt32 lNumElements = mBounds.h / mListElementHeight;
    
    if (-1 == mSelectedElementIndex)
    {
        if (kDirectionUp == aDirection)
        {
            // select last element in list
            mSelectedElementIndex = mDirectoryContents.size() - 1;
            mFirstElementIndex = (mSelectedElementIndex + 1) - min((int)lNumElements, (int)mDirectoryContents.size());
        }
        else
        {
            // select first element in list
            mSelectedElementIndex = 0;
            mFirstElementIndex = 0;
        }
    }
    else
    {
        if (mSelectedElementIndex < mFirstElementIndex ||
            mSelectedElementIndex > mFirstElementIndex + lNumElements)
        {
            setSelectedIndex(mSelectedElementIndex);
        }
        
        if (kDirectionUp == aDirection)
        {
            // can only go up if not at element 0
            if (mSelectedElementIndex)
            {
                // do we need to scroll?
                if (mSelectedElementIndex - 1 < mFirstElementIndex)
                {
                    scroll(kDirectionUp);
                }
                mSelectedElementIndex--;
            }
        }
        else
        {
            // can only go down if not on last element
            if (mSelectedElementIndex + 1 < SInt32(mDirectoryContents.size()))
            {
                // do we need to scroll?
                if (mSelectedElementIndex + 1 >= mFirstElementIndex + lNumElements)
                {
                    scroll(kDirectionDown);
                }
                mSelectedElementIndex++;
            }
        }
    }
    return kSucceeded;
}

Status
GUIFileBrowser::FileListArea::setSelectedIndex(SInt32 aIndex)
{
    SInt32 lNumElements = mBounds.h / mListElementHeight;
    
    // selection is above current view - scroll up
    if (aIndex < mFirstElementIndex)
    {
        mFirstElementIndex = aIndex;
        mSelectedElementIndex = aIndex;
        return kSucceeded;
    }
    // selection is below current view - scroll down
    else if (aIndex > mFirstElementIndex + lNumElements)
    {
        mFirstElementIndex = aIndex - min((int)mDirectoryContents.size(), (int)lNumElements);
        mSelectedElementIndex = aIndex;
        return kSucceeded;
    }
    mSelectedElementIndex = aIndex;
    return kSucceeded;
}

Status
GUIFileBrowser::FileListArea::draw()
{
    Status lStatus = kSucceeded;
    
    int lBoxPosX = mBounds.x;
    int lBoxPosY = mBounds.y;
    int lBoxW = mBounds.w;
    int lBoxH = mBounds.h;
    
    FontController * lFontController = FontController::getFontController();
    TextRenderer* lTextRenderer = lFontController->getTextRenderer(TextRenderer_Default);
    
    glPushMatrix();
    
    GraphicsUtil::setViewPort2D(lBoxPosX, lBoxPosY, lBoxW, lBoxH);
    
    GraphicsUtil::drawQuad(0, 0, 0, lBoxH, lBoxW, lBoxH, lBoxW, 0, mColor);

    SInt32 lElementHeight = mListElementHeight;
    SInt32 lNumElements = mBounds.h / lElementHeight;
    SInt32 lY = mBounds.h - 2 - lElementHeight;
    
    if (lNumElements > SInt32(mDirectoryContents.size()) - mFirstElementIndex)
    {
        lNumElements = SInt32(mDirectoryContents.size()) - mFirstElementIndex;
    }
    
    if (lNumElements)
    {
        for (SInt32 i = mFirstElementIndex; i < mFirstElementIndex + lNumElements; ++i)
        {
            if (i == mSelectedElementIndex)
            {
                // draw focus border
                Rectangle2D lFocusBounds = {mBevel + 4, lY - 2,
                                            mBounds.w - mBevel - 6,
                                            mListElementHeight};
                Color lFocusColor = {255, 255, 255};
                GraphicsUtil::drawRectangle(lFocusBounds, lFocusColor);
            }
            if (is_directory(*mDirectoryContents[i]))
            {
                lTextRenderer->drawText(mBevel + 4, lY + 2, mDirectoryContents[i]->leaf().generic_wstring() + L"/");
                lY -= 20;
            }
            else
            {
                lTextRenderer->drawText(mBevel + 4, lY + 2, mDirectoryContents[i]->leaf().generic_wstring());
                lY -= 20;
            }
        }
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
        // draw bevel left
        GraphicsUtil::drawQuad(0, 0, 0, lBoxH, lBevel, lBoxH - lBevel, 
                               lBevel, lBevel, lColor1);
        
        // draw bevel top
        GraphicsUtil::drawQuad(0, lBoxH, lBoxW, lBoxH, lBoxW - lBevel, 
                               lBoxH - lBevel, lBevel, lBoxH - lBevel);
        
        // draw bevel right
        GraphicsUtil::drawQuad(lBoxW, lBoxH, lBoxW, 0, lBoxW - lBevel, 
                               lBevel, lBoxW - lBevel, lBoxH - lBevel, 
                               lColor2);
        
        // draw bevel bottom
        GraphicsUtil::drawQuad(0, 0, lBevel, lBevel, lBoxW - lBevel,
                               lBevel, lBoxW, 0);
    }
    
    glPopMatrix();
    
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "FileListArea::draw - Drawing FileListArea %d\n", mID);
    #endif
    
    return lStatus;
    
}

EventSignal
GUIFileBrowser::FileListArea::handleKeyEvent(KeyEvent * aKeyEvent)
{
    if(kKeyTypeNormal == aKeyEvent->mKeyType)
    {
        if (' ' == aKeyEvent->mKey || 13 == aKeyEvent->mKey) // space or enter
        {
            if (-1 != mSelectedElementIndex)
            {
                Path lPath = Path(*mDirectoryContents.at(mSelectedElementIndex));
                if (is_directory(lPath))
                {
                    mFileBrowser->load(lPath);
                    return kEventAbsorbed;
                }
            }
        }
        else if (127 == aKeyEvent->mKey &&
                 kKeyPressed == aKeyEvent->mKeyEventType)
        {
            mFileBrowser->goToParentDirectory();
            return kEventAbsorbed;
        }
    }
    else if (kKeyTypeSpecial == aKeyEvent->mKeyType)
    {
        if (kKeyPressed == aKeyEvent->mKeyEventType)
        {
            if (Keys::kUp == aKeyEvent->mKey)
            {
                moveSelection(kDirectionUp);
                return kEventAbsorbed;
            }
            else if (Keys::kDown == aKeyEvent->mKey)
            {
                moveSelection(kDirectionDown);
                return kEventAbsorbed;
            }
        }
    }
    return kEventIgnored;
}

EventSignal
GUIFileBrowser::FileListArea::handleMouseEvent(MouseEvent * aMouseEvent)
{
    if ((aMouseEvent->mDoubleClick || kMouseDown == aMouseEvent->mMouseEventType) &&
        Util::inBounds(mBounds, aMouseEvent->mMousePosition))
    {
        // did we hit a file label?
        UInt32 lMaxElements = mBounds.h / mListElementHeight;
        UInt32 lNumElements = min((int) lMaxElements, (int)(mDirectoryContents.size() - mFirstElementIndex));
        UInt32 lClickedElementIndex = (mBounds.h - (aMouseEvent->mMousePosition.y - mBounds.y)) / mListElementHeight;
        if (lNumElements > lClickedElementIndex)
        {
            mSelectedElementIndex = mFirstElementIndex + lClickedElementIndex;
        }
        if (aMouseEvent->mDoubleClick)
        {
            Path lPath = Path(*mDirectoryContents.at(mSelectedElementIndex));
            if (is_directory(lPath))
            {
                mFileBrowser->load(lPath);
            }            
        }
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    else if (kMouseUp == aMouseEvent->mMouseEventType &&
             Util::inBounds(mBounds, aMouseEvent->mMousePosition))
    {
        aMouseEvent->mAcceptor = this;
        return kEventAbsorbed;
    }
    return kEventIgnored;
}

////////////////////////////////////////////////
//
//    LoadPathButton
//
////////////////////////////////////////////////

GUIFileBrowser::LoadPathButton::LoadPathButton(Rectangle2D aBounds,
                                               GUIFileBrowser * aFileBrowser)
    : mFileBrowser(aFileBrowser)
{
    assert(aFileBrowser);
    if (!aFileBrowser)
    {
        fprintf(stderr, "GUIFileBrowser::LoadPathButton::LoadPathButton - NULL pointer passed as pointer to parent GUIFileBrowser\n");
    }
    SizeConstraint lSizeConstraint = {kLocked, kAuto};
    GUIButton(0, aBounds, L"Load");
    setText(L"Load");
    setParent(aFileBrowser);
    setSizeConstraint(lSizeConstraint);
    setBounds(aBounds);
    setPadding(2);
    setBevel(1);
    mIsInTabOrder = true;
}

GUIFileBrowser::LoadPathButton::~LoadPathButton()
{
    mFileBrowser = 0;
}

Status
GUIFileBrowser::LoadPathButton::doAction()
{
    return mFileBrowser->loadTextBoxPath();
}



////////////////////////////////////////////////
//
//    GUIFileBrowser
//
////////////////////////////////////////////////

GUIFileBrowser *
GUIFileBrowser::create(UInt32 aID,
                       Rectangle2D aBounds,
                       Path & aDirPath,
                       GUIElement * aParent)
{
    if (!is_directory(aDirPath))
    {
        fprintf(stderr, "GUIFileBrowser::create - Path is not a directory\n");
        return 0;
    }
    if (!exists(aDirPath))
    {
        fprintf(stderr, "GUIFileBrowser::create - Path does not exist\n");
        return 0;
    }
    
    // expand path
    aDirPath = boost::filesystem::system_complete(aDirPath);
    
    GUIFileBrowser * lFileBrowser = new GUIFileBrowser(aID, aBounds, aDirPath, aParent);
    return lFileBrowser;
}

GUIFileBrowser::~GUIFileBrowser()
{
    mObservers.clear();

    // TODO Deallocate a lot of stuff here
}

Status
GUIFileBrowser::addObserver(Observer * aObserver)
{
    mObservers.push_back(aObserver);
    return kSucceeded;
}

Status
GUIFileBrowser::removeObserver(Observer * aObserver)
{
    auto lObserverIterator = mObservers.begin();
    for (; lObserverIterator != mObservers.end(); ++lObserverIterator)
    {
        if (*lObserverIterator == aObserver)
        {
            mObservers.erase(lObserverIterator);
            return kSucceeded;
        }
    }
    return kFailed;
}

GUIFileBrowser::FileAction
GUIFileBrowser::getLastFileAction()
{
    return mLastFileAction;
}

const Path &
GUIFileBrowser::getCurrentPath()
{
    return mPath;
}

Status
GUIFileBrowser::goToParentDirectory()
{
    // Not at root
    if (mPath != "/")
    {
        mPath = Path(mPath.parent_path());
        reload();
    }
    return kSucceeded;
}

Status
GUIFileBrowser::loadTextBoxPath()
{
    Path lPath = Path(mPathBox->getText());
    return load(lPath);
}

Status
GUIFileBrowser::navigateHistory(Direction aDirection)
{
    // TODO take a little step in the history vector
    // load the new path
    return kSucceeded;
}

GUIFileBrowser::GUIFileBrowser(UInt32 aID,
                               Rectangle2D aBounds,
                               Path & aPath,
                               GUIElement * aParent)
    : GUIElement(aID, aBounds)
    , mLastFileAction(kFileAction_None)
    , mPath(aPath)
    , mPathArea(0)
    , mPathBox(0)
    , mLoadPathButton(0)
{
    setFillDirection(kFillDirectionVertical);
    setPadding(2);
    setColor(aParent->getColor());
    
    // path area
    {
        Color lTextBoxColor;
        lTextBoxColor.r = mColor.r * 0.65;
        lTextBoxColor.g = mColor.g * 0.65;
        lTextBoxColor.b = mColor.b * 0.65;
        
        Rectangle2D lBounds = {mBounds.x, mBounds.y, mBounds.w, 24};
        SizeConstraint lSizeConstraint = {kAuto, kLocked};
        Alignment lAlignment;
        lAlignment.mHAlign = kLeft;
        lAlignment.mVAlign = kTop;
        mPathArea = new GUIElement(1001, lBounds, 0, lSizeConstraint,
                                   kFillDirectionHorizontal, lAlignment);
        mPathArea->setSizeConstraint(lSizeConstraint);
        mPathArea->setPadding(2);
        mPathArea->setColor(mColor);
        
        lBounds.h = 20;
        mPathBox = new GUITextBox(1002, lBounds, aPath.generic_wstring());
        mPathBox->setColor(lTextBoxColor);
        
        lBounds.w = 35;
        mLoadPathButton = new LoadPathButton(lBounds, this);
        mLoadPathButton->setColor(mColor);
    }
    
    // button area
    {
        // history buttons
        Rectangle2D lBounds = {mBounds.x, mBounds.y, mBounds.w, 24};
        SizeConstraint lSizeConstraint = {kAuto, kLocked};
        Alignment lAlignment;
        lAlignment.mHAlign = kLeft;
        lAlignment.mVAlign = kTop;
        mButtonArea = new GUIElement(1003, lBounds, 0, lSizeConstraint,
                                    kFillDirectionHorizontal, lAlignment);
        mButtonArea->setSizeConstraint(lSizeConstraint);
        mButtonArea->setPadding(2);
        mButtonArea->setColor(mColor);
        
        lBounds.h = 20;
        lBounds.w = 20;
        mHistoryBackButton = new HistoryButton(lBounds, this);
        mHistoryBackButton->setDirection(kDirectionBack);
        mHistoryBackButton->setColor(mColor);
        mHistoryForwardButton = new HistoryButton(lBounds, this);
        mHistoryForwardButton->setDirection(kDirectionForth);
        mHistoryForwardButton->setColor(mColor);
        // parent directory button
        mParentDirButton = new ParentDirButton(lBounds, this);
        mParentDirButton->setColor(mColor);
    }
    
    // file list area
    {
        Rectangle2D lBounds = {mBounds.x, mBounds.y, mBounds.w, mBounds.h};
        SizeConstraint lSizeConstraint = {kAuto, kAuto};
        Alignment lAlignment;
        lAlignment.mHAlign = kLeft;
        lAlignment.mVAlign = kTop;
        mFileListArea = new FileListArea(1004, lBounds, 0, lSizeConstraint,
                                         kFillDirectionVertical, lAlignment,
                                         this);
        mFileListArea->setSizeConstraint(lSizeConstraint);
        mFileListArea->setPadding(4);
        mFileListArea->setBevel(-1);
        mFileListArea->setColor(mColor);
        
        Color lFileAreaColor;
        lFileAreaColor.r = mColor.r * 0.65;
        lFileAreaColor.g = mColor.g * 0.65;
        lFileAreaColor.b = mColor.b * 0.65;
        mFileListArea->setColor(lFileAreaColor);
    }
    
    // scroll up
    {
        Rectangle2D lBounds = {mBounds.x, mBounds.y, mBounds.w, 20};
        mScrollUpButton = new ScrollButton(lBounds, this, mFileListArea);
        mScrollUpButton->setDirection(kDirectionUp);
        mScrollUpButton->setColor(mColor);
    }

    // scroll down
    {
        Rectangle2D lBounds = {mBounds.x, mBounds.y, mBounds.w, 20};
        mScrollDownButton = new ScrollButton(lBounds, this, mFileListArea);
        mScrollDownButton->setDirection(kDirectionDown);
        mScrollDownButton->setColor(mColor);
    }
    
    load(mPath);
}

Status
GUIFileBrowser::load(Path & aPath)
{
    // std::cout << "GUIFileBrowser::laod - loading " << aPath.generic_string() << std::endl;
    
    // make sure this is a directory
    if (!is_directory(aPath))
    {
        fprintf(stderr, "GUIFileBrowser::load - Path %s is not a directory.\n", aPath.generic_string().c_str());
        return kFailed;
    }
 
    mPath = aPath;
    
    // deallocate objects in mDirectoryContents
    if (mFileListArea)
    {
        mFileListArea->clear();
    }
    
    // scan directory
    vector<Path> path_list;
    boost::filesystem::directory_iterator itr(aPath);
    boost::filesystem::directory_iterator end_itr;
    for (; itr != end_itr; ++itr)
    {
        Path * lPath = new Path(itr->path());
        if (mFileListArea)
        {
            mFileListArea->addPath(lPath);
        }
    }

    mPathBox->setText(mPath.generic_wstring());
    
    return kSucceeded;
}

Status
GUIFileBrowser::reload()
{
    return load(mPath);
}

Status
GUIFileBrowser::draw()
{
    // HACK!
    if (mParentWindow && !mPathArea->getParentWindow())
    {
        addChild(mParentWindow, mPathArea);
        mPathArea->addChild(mPathBox);
        mPathArea->addChild(mLoadPathButton);
        addChild(mParentWindow, mButtonArea);
        mButtonArea->addChild(mHistoryBackButton);
        mButtonArea->addChild(mHistoryForwardButton);
        mButtonArea->addChild(mParentDirButton);
        addChild(mParentWindow, mScrollUpButton);
        addChild(mParentWindow, mFileListArea);
        addChild(mParentWindow, mScrollDownButton);
    
        mParentWindow->updateTabOrder();
        
        setFocusColor(mParent->getFocusColor(), true);
    }
    
    
    GUIElement::draw();

    return kSucceeded;
}
