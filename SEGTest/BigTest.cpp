//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Test implementation
//

#include <SEGIS/Types.h>
#include <SEGIS/GUIElements.h>

#include <SEGIS/LayoutManager.h>
#include <SEGIS/Events.h>
#include <SEGIS/EventManager.h>
#include <SEGIS/Window.h>
#include <SEGIS/MainController.h>
#include <SEGIS/BooleanOption.h>
#include <SEGIS/NumberOptionFloat.h>
#include <SEGIS/NumberOptionInteger.h>
#include <SEGIS/StringOption.h>

#include "ImageData.h"

#define FILE_BROWSER 1

int
main(int argc, char** argv)
{
    MainController * lMainController = MainController::getMainController();

    UInt32 mID = 0;
    SInt32 lWidth = 1024;
    SInt32 lHeight = 768;

    // some color schemes
    // #define DARK
    // #define BRIGHT
    // #define DARK2
    // #define SILLY
    #define DARK3

    #if defined ( DARK )
        Color lMainColor        = {0x3E, 0x3F, 0x47};
        Color lDarkDetailColor  = {0x30, 0x30, 0x40};
        Color lButtonColor      = {  53,   61,   42};
        Color lButtonLabelColor = {0xFF, 0xFF, 0xFF};
        Color lTickColor        = {0xFF, 0xFF, 0xFF};
        Color lLabelColor       = {0xFF, 0xFF, 0xFF};
        Color lTextBoxColor     = {0x30, 0x30, 0x30};
        Color lFocusColor       = {  78,  103,   50};
        Color lNiceGradColor0   = {0x30, 0x30, 0x38};
        Color lNiceGradColor1   = {0x60, 0x60, 0x68};
        Color lNiceGradColor2   = {0x60, 0x60, 0x68};
        Color lNiceGradColor3   = {0x30, 0x30, 0x38};
        // Color lColorBlack       = {0x00, 0x00, 0x00};
        // Color lColorWhite       = {0xFF, 0xFF, 0xFF};
        Color lMenuItemColor    = lMainColor;
    #elif defined ( BRIGHT )
        Color lMainColor        = {0xBE, 0xBE, 0xBE};
        Color lDarkDetailColor  = {0x70, 0x70, 0x70};
        Color lButtonColor      = {0x88, 0x88, 0x88};
        Color lButtonLabelColor = {0x00, 0x00, 0x00};
        Color lTickColor        = {0x00, 0x00, 0x00};
        Color lLabelColor       = {0x00, 0x00, 0x00};
        Color lTextBoxColor     = {0x88, 0x88, 0x88};
        Color lFocusColor       = {0x90, 0x90, 0x90};
        Color lMenuItemColor    = lMainColor;
        Color lNiceGradColor0   = {0xBE, 0xBE, 0xBE};
        Color lNiceGradColor1   = {0xBE, 0xBE, 0xBE};
        Color lNiceGradColor2   = {0xBE, 0xBE, 0xBE};
        Color lNiceGradColor3   = {0xBE, 0xBE, 0xBE};
    #elif defined ( DARK2 )
        Color lMainColor        = {0x30, 0x30, 0x30};
        Color lDarkDetailColor  = {0x20, 0x20, 0x20};
        Color lButtonColor      = {0x40, 0x40, 0x48};
        Color lButtonLabelColor = {0xc0, 0xc0, 0xff};
        Color lTickColor        = {0xc0, 0xc0, 0xff};
        Color lLabelColor       = {0xc0, 0xc0, 0xff};
        Color lTextBoxColor     = {0x20, 0x20, 0x20};
        Color lFocusColor       = {0x80, 0x80, 0x80};
        Color lMenuItemColor    = lMainColor;
        Color lNiceGradColor0   = {0x30, 0x30, 0x30};
        Color lNiceGradColor1   = {0x40, 0x40, 0x40};
        Color lNiceGradColor2   = {0x30, 0x30, 0x30};
        Color lNiceGradColor3   = {0x20, 0x20, 0x20};
    #elif defined ( SILLY )
        Color lMainColor        = {0x50, 0x80, 0x60};
        Color lDarkDetailColor  = {0x50, 0x60, 0x80};
        Color lButtonColor      = {0x60, 0x70, 0x90};
        Color lButtonLabelColor = {0xff, 0xff, 0xff};
        Color lTickColor        = {0xff, 0x70, 0x70};
        Color lLabelColor       = {0xff, 0xff, 0xee};
        Color lTextBoxColor     = {0x20, 0x40, 0x20};
        Color lFocusColor       = {0xff, 0x77, 0x77};
        Color lMenuItemColor    = lMainColor;
        Color lNiceGradColor0   = {0x60, 0x90, 0x70};
        Color lNiceGradColor1   = {0x80, 0x90, 0x60};
        Color lNiceGradColor2   = {0x90, 0x60, 0x70};
        Color lNiceGradColor3   = {0x50, 0x70, 0x80};
    #elif defined ( DARK3 )
        Color lMainColor        = {0x3E, 0x3F, 0x44};
        Color lDarkDetailColor  = {0x20, 0x20, 0x20};
        Color lButtonColor      = {  68,   30,   20};
        Color lButtonLabelColor = {0xFF, 0xFF, 0xFF};
        Color lTickColor        = {0xFF, 0xFF, 0xFF};
        Color lLabelColor       = {0xFF, 0xFF, 0xFF};
        Color lTextBoxColor     = {0x30, 0x30, 0x30};
        Color lFocusColor       = {  78,   25,   22};
        Color lNiceGradColor0   = {0x30, 0x30, 0x38};
        Color lNiceGradColor1   = {0x60, 0x60, 0x68};
        Color lNiceGradColor2   = {0x60, 0x60, 0x68};
        Color lNiceGradColor3   = {0x30, 0x30, 0x38};
        // Color lColorBlack       = {0x00, 0x00, 0x00};
        // Color lColorWhite       = {0xFF, 0xFF, 0xFF};
        Color lMenuItemColor    = lMainColor;
    #else
        #error "DARK, BRIGHT, DARK2, DARK3 or SILLY must be defined"
    #endif
    // Color lNiceRed          = { 255,   46,    0};
    // Color lNiceYellow       = { 255,  200,    0};

    SizeConstraint lSizeConstraint;
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;

    // some alignments
    const Alignment lCenterCenterAlignment = { kHCenter, kVCenter };
    const Alignment lLeftCenterAlignment = { kLeft, kVCenter };
    const Alignment lRightCenterAlignment = { kRight, kVCenter };
    const Alignment lLeftTopAlignment = { kLeft, kTop };

    Rectangle2D lBounds = {0, 0, lWidth, lHeight};

    // main view
    GUIElement* lMainView = new GUIElement(mID++, lBounds);
    lMainView->setColor(lMainColor);
    lMainView->setFillDirection(kFillDirectionVertical);

    Window* lMainWindow = new Window(lWidth, lHeight, lMainView, "nice window");
    if (!lMainWindow)
    {
        fprintf(stderr, "lMainWindow creation failed\n");
        return 1;
    }

    // menu bar
    lBounds.w = 200;
    lBounds.h = 48;
    GUIGradientPanel* lUberTopView = new GUIGradientPanel(mID++, lBounds);
    lUberTopView->setColor(lMainColor);
    lUberTopView->setColor(lNiceGradColor0, lNiceGradColor1, lNiceGradColor2, lNiceGradColor3);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lUberTopView->setSizeConstraint(lSizeConstraint);
    lUberTopView->setFillDirection(kFillDirectionHorizontal);
    lUberTopView->setAlignment(lLeftCenterAlignment);
    lUberTopView->setPadding(4);
    lUberTopView->setBevel(1);

    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lBounds.w = 200;
    lBounds.h = 40;
    GUIMenuBar* lMenuBar = new GUIMenuBar(mID++, lBounds);
    lMenuBar->setSizeConstraint(lSizeConstraint);
    lMenuBar->setFillDirection(kFillDirectionHorizontal);
    lMenuBar->setAlignment(lLeftCenterAlignment);
    lMenuBar->setColor(lButtonColor);

    lBounds.h = 30;
    lSizeConstraint.w = kAuto;
    GUIMenuItem* lMenuA0 = new GUIMenuItem(mID++, lBounds, L"It's a Menu");
    lMenuA0->setColor(lMenuItemColor);
    lMenuA0->setLabelColor(lLabelColor);
    lMenuA0->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA01 = new GUIMenuItem(mID++, lBounds, L"With some >>");
    lMenuA01->setColor(lMenuItemColor);
    lMenuA01->setLabelColor(lLabelColor);
    lMenuA01->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA02 = new GUIMenuItem(mID++, lBounds, L"And some >>");
    lMenuA02->setColor(lMenuItemColor);
    lMenuA02->setLabelColor(lLabelColor);
    lMenuA02->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA010 = new GUIMenuItem(mID++, lBounds, L"Stuff");
    lMenuA010->setColor(lMenuItemColor);
    lMenuA010->setLabelColor(lLabelColor);
    lMenuA010->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA011 = new GUIMenuItem(mID++, lBounds, L"And Nice Animations");
    lMenuA011->setColor(lMenuItemColor);
    lMenuA011->setLabelColor(lLabelColor);
    lMenuA011->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA020 = new GUIMenuItem(mID++, lBounds, L"More");
    lMenuA020->setColor(lMenuItemColor);
    lMenuA020->setLabelColor(lLabelColor);
    lMenuA020->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuA021 = new GUIMenuItem(mID++, lBounds, L"Stuff");
    lMenuA021->setColor(lMenuItemColor);
    lMenuA021->setLabelColor(lLabelColor);
    lMenuA021->setSizeConstraint(lSizeConstraint);

    lBounds.h = 30;
    GUIMenuItem* lMenuB0 = new GUIMenuItem(mID++, lBounds, L"Another One");
    lMenuB0->setColor(lButtonColor);
    lMenuB0->setLabelColor(lLabelColor);
    lMenuB0->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB01 = new GUIMenuItem(mID++, lBounds, L"And here >>");
    lMenuB01->setColor(lMenuItemColor);
    lMenuB01->setLabelColor(lLabelColor);
    lMenuB01->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB02 = new GUIMenuItem(mID++, lBounds, L"Hey Boy >>");
    lMenuB02->setColor(lMenuItemColor);
    lMenuB02->setLabelColor(lLabelColor);
    lMenuB02->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB010 = new GUIMenuItem(mID++, lBounds, L"Is a really long one");
    lMenuB010->setColor(lMenuItemColor);
    lMenuB010->setLabelColor(lLabelColor);
    lMenuB010->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB011 = new GUIMenuItem(mID++, lBounds, L"And another really long one");
    lMenuB011->setColor(lMenuItemColor);
    lMenuB011->setLabelColor(lLabelColor);
    lMenuB011->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB020 = new GUIMenuItem(mID++, lBounds, L"Whatever");
    lMenuB020->setColor(lMenuItemColor);
    lMenuB020->setLabelColor(lLabelColor);
    lMenuB020->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB021 = new GUIMenuItem(mID++, lBounds, L"Hey Girl >>");
    lMenuB021->setColor(lMenuItemColor);
    lMenuB021->setLabelColor(lLabelColor);
    lMenuB021->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB0210 = new GUIMenuItem(mID++, lBounds, L"Superstar Deejays");
    lMenuB0210->setColor(lMenuItemColor);
    lMenuB0210->setLabelColor(lLabelColor);
    lMenuB0210->setSizeConstraint(lSizeConstraint);

    GUIMenuItem* lMenuB0211 = new GUIMenuItem(mID++, lBounds, L"Here we go!");
    lMenuB0211->setColor(lMenuItemColor);
    lMenuB0211->setLabelColor(lLabelColor);
    lMenuB0211->setSizeConstraint(lSizeConstraint);

    lSizeConstraint.w = kLocked;


    // top
    lBounds.h = 64;
    GUIGradientPanel* lTopView = new GUIGradientPanel(mID++, lBounds);
    lTopView->setColor(lMainColor);
    lTopView->setColor(lNiceGradColor0, lNiceGradColor1, lNiceGradColor2, lNiceGradColor3);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lTopView->setSizeConstraint(lSizeConstraint);
    lTopView->setFillDirection(kFillDirectionHorizontal);
    lTopView->setPadding(1);
    lTopView->setBevel(1);

    // top left
    GUIElement* lTopLeftView = new GUIElement(mID++, lBounds);
    lTopLeftView->setColor(lMainColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lTopLeftView->setSizeConstraint(lSizeConstraint);
    lTopLeftView->setFillDirection(kFillDirectionHorizontal);
    lTopLeftView->setPadding(4);
    lTopLeftView->setBevel(0);
    lTopLeftView->setAlignment(lLeftCenterAlignment);
    lTopLeftView->setVisible(false);

    // top right
    GUIElement* lTopRightView = new GUIElement(mID++, lBounds);
    lTopRightView->setColor(lMainColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lTopRightView->setSizeConstraint(lSizeConstraint);
    lTopRightView->setFillDirection(kFillDirectionHorizontal);
    lTopRightView->setPadding(4);
    lTopRightView->setBevel(0);
    lTopRightView->setAlignment(lRightCenterAlignment);
    lTopRightView->setVisible(false);

    // button 1
    lBounds.w = 40;
    lBounds.h = 32;
    GUIButton* lButton1 = new GUIButton(mID++, lBounds, L"do");
    lButton1->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton1->setSizeConstraint(lSizeConstraint);
    lButton1->setPadding(2);
    lButton1->setBevel(1);
    lButton1->setLabelColor(lButtonLabelColor);

    // button 2
    GUIButton* lButton2 = new GUIButton(mID++, lBounds, L"some");
    lButton2->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton2->setSizeConstraint(lSizeConstraint);
    lButton2->setPadding(2);
    lButton2->setBevel(1);
    lButton2->setLabelColor(lButtonLabelColor);

    // button 3
    GUIButton* lButton3 = new GUIButton(mID++, lBounds, L"cool");
    lButton3->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton3->setSizeConstraint(lSizeConstraint);
    lButton3->setPadding(2);
    lButton3->setBevel(1);
    lButton3->setLabelColor(lButtonLabelColor);

    // button 4
    GUIButton* lButton4 = new GUIButton(mID++, lBounds, L"stuff");
    lButton4->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton4->setSizeConstraint(lSizeConstraint);
    lButton4->setPadding(2);
    lButton4->setBevel(1);
    lButton4->setLabelColor(lButtonLabelColor);

    // button 5
    GUIButton* lButton5 = new GUIButton(mID++, lBounds, L"right");
    lButton5->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton5->setSizeConstraint(lSizeConstraint);
    lButton5->setPadding(2);
    lButton5->setBevel(1);
    lButton5->setLabelColor(lButtonLabelColor);

    // button 6
    GUIButton* lButton6 = new GUIButton(mID++, lBounds, L"align");
    lButton6->setColor(lButtonColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lButton6->setSizeConstraint(lSizeConstraint);
    lButton6->setPadding(2);
    lButton6->setBevel(1);
    lButton6->setLabelColor(lButtonLabelColor);

    // fullscreen label
    lBounds.w = 120;
    lBounds.h = 24;
    GUILabel* lFullScreenLabel = new GUILabel(mID++, lBounds, L"Press f for fullscreen");
    lFullScreenLabel->setLabelColor(lLabelColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lFullScreenLabel->setSizeConstraint(lSizeConstraint);

    // bottom
    lBounds.h = 600-150;
    GUISplitView* lBottomView = new GUISplitView(mID++, lBounds);
    lBottomView->setColor(lMainColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lBottomView->setSizeConstraint(lSizeConstraint);
    lBottomView->setFillDirection(kFillDirectionHorizontal);
    lBottomView->setPadding(7);

    // left
    lBounds.w = 240;
    GUISplitView* lLeftView = new GUISplitView(mID++, lBounds);
    lLeftView->setColor(lMainColor);
    lSizeConstraint.w = kChangable;
    lSizeConstraint.h = kAuto;
    lLeftView->setSizeConstraint(lSizeConstraint);
    lLeftView->setFillDirection(kFillDirectionVertical);
    lLeftView->setPadding(6);
    lLeftView->setBevel(1);

    // right
    GUIElement* lRightView = new GUIElement(mID++, lBounds);
    lRightView->setColor(lMainColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lRightView->setSizeConstraint(lSizeConstraint);
    lRightView->setFillDirection(kFillDirectionVertical);
    lRightView->setPadding(6);
    lRightView->setBevel(1);

    // right inner
    GUIElement* lRightInnerView = new GUIElement(mID++, lBounds);
    // lRightInnerView->setColor(lMainColor);
    lRightInnerView->setColor(lDarkDetailColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lRightInnerView->setSizeConstraint(lSizeConstraint);
    lRightInnerView->setFillDirection(kFillDirectionVertical);
    lRightInnerView->setPadding(4);
    lRightInnerView->setBevel(-1);
    lRightInnerView->setAlignment(lLeftTopAlignment);

    GUILayerPanel* lLayerPanel = new GUILayerPanel(mID++, lBounds);
    lLayerPanel->setColor(lMainColor);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayerPanel->setSizeConstraint(lSizeConstraint);
    lLayerPanel->setFillDirection(kFillDirectionVertical);
    lLayerPanel->setPadding(0);
    lLayerPanel->setBevel(0);
    lLayerPanel->setAlignment(lLeftTopAlignment);
    lLayerPanel->setLabelColor(lLabelColor);

    // layer a
    lBounds.h = 350;
    lBounds.w = 330;
    GUIElement* lLayerA = new GUIElement(mID++, lBounds);
    lLayerA->setColor(lMainColor);
    lLayerA->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayerA->setSizeConstraint(lSizeConstraint);
    lLayerA->setFillDirection(kFillDirectionHorizontal);
    lLayerA->setPadding(4);
    lLayerA->setBevel(0);
    lLayerA->setAlignment(lCenterCenterAlignment);
    lLayerA->setVisible(false);

    // layer 0
    lBounds.h = 30;
    lBounds.w = 330;
    GUIElement* lLayer0 = new GUIElement(mID++, lBounds);
    lLayer0->setColor(lMainColor);
    lLayer0->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayer0->setSizeConstraint(lSizeConstraint);
    lLayer0->setFillDirection(kFillDirectionHorizontal);
    lLayer0->setPadding(4);
    lLayer0->setBevel(0);
    lLayer0->setAlignment(lLeftCenterAlignment);
    lLayer0->setVisible(false);

    // layer 1
    GUIElement* lLayer1 = new GUIElement(mID++, lBounds);
    lLayer1->setColor(lMainColor);
    lLayer1->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayer1->setSizeConstraint(lSizeConstraint);
    lLayer1->setFillDirection(kFillDirectionHorizontal);
    lLayer1->setPadding(4);
    lLayer1->setBevel(0);
    lLayer1->setAlignment(lLeftTopAlignment);
    lLayer1->setVisible(false);

    // layer 2
    GUIElement* lLayer2 = new GUIElement(mID++, lBounds);
    lLayer2->setColor(lMainColor);
    lLayer2->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayer2->setSizeConstraint(lSizeConstraint);
    lLayer2->setFillDirection(kFillDirectionVertical);
    lLayer2->setPadding(4);
    lLayer2->setBevel(0);
    lLayer2->setAlignment(lLeftTopAlignment);
    lLayer2->setVisible(false);

    // layer 2 - row 0
    GUIElement* lLayer2Row0 = new GUIElement(mID++, lBounds);
    lLayer2Row0->setColor(lMainColor);
    lLayer2Row0->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayer2Row0->setSizeConstraint(lSizeConstraint);
    lLayer2Row0->setFillDirection(kFillDirectionHorizontal);
    lLayer2Row0->setPadding(4);
    lLayer2Row0->setBevel(0);
    lLayer2Row0->setAlignment(lLeftCenterAlignment);
    lLayer2Row0->setVisible(false);
    // layer 2 - row 1
    GUIElement* lLayer2Row1 = new GUIElement(mID++, lBounds);
    lLayer2Row1->setColor(lMainColor);
    lLayer2Row1->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayer2Row1->setSizeConstraint(lSizeConstraint);
    lLayer2Row1->setFillDirection(kFillDirectionHorizontal);
    lLayer2Row1->setPadding(4);
    lLayer2Row1->setBevel(0);
    lLayer2Row1->setAlignment(lLeftCenterAlignment);
    lLayer2Row1->setVisible(false);


    // layer 3
    GUIElement* lLayer3 = new GUIElement(mID++, lBounds);
    lLayer3->setColor(lMainColor);
    lLayer3->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayer3->setSizeConstraint(lSizeConstraint);
    lLayer3->setFillDirection(kFillDirectionHorizontal);
    lLayer3->setPadding(4);
    lLayer3->setBevel(0);
    lLayer3->setAlignment(lLeftCenterAlignment);
    lLayer3->setVisible(false);

    // layer 4
    GUIElement* lLayer4 = new GUIElement(mID++, lBounds);
    lLayer4->setColor(lMainColor);
    lLayer4->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kLocked;
    lLayer4->setSizeConstraint(lSizeConstraint);
    lLayer4->setFillDirection(kFillDirectionHorizontal);
    lLayer4->setPadding(4);
    lLayer4->setBevel(0);
    lLayer4->setAlignment(lLeftCenterAlignment);
    lLayer4->setVisible(false);

    // layer 5
    GUIElement* lLayer5 = new GUIElement(mID++, lBounds);
    lLayer5->setColor(lMainColor);
    lLayer5->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayer5->setSizeConstraint(lSizeConstraint);
    lLayer5->setFillDirection(kFillDirectionHorizontal);
    lLayer5->setPadding(4);
    lLayer5->setBevel(0);
    lLayer5->setAlignment(lLeftCenterAlignment);
    lLayer5->setVisible(false);

    // layer 6
    GUIElement* lLayer6 = new GUIElement(mID++, lBounds);
    lLayer6->setColor(lMainColor);
    lLayer6->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayer6->setSizeConstraint(lSizeConstraint);
    lLayer6->setFillDirection(kFillDirectionHorizontal);
    lLayer6->setPadding(4);
    lLayer6->setBevel(0);
    lLayer6->setAlignment(lLeftCenterAlignment);
    lLayer6->setVisible(false);

    // layer 7
    GUIElement* lLayer7 = new GUIElement(mID++, lBounds);
    lLayer7->setColor(lMainColor);
    lLayer7->setBounds(lBounds);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLayer7->setSizeConstraint(lSizeConstraint);
    lLayer7->setFillDirection(kFillDirectionHorizontal);
    lLayer7->setPadding(4);
    lLayer7->setBevel(0);
    lLayer7->setAlignment(lLeftCenterAlignment);
    lLayer7->setVisible(false);

    // left bottom
    GUIGradientPanel* lLeftBottom = new GUIGradientPanel(mID++, lBounds);
    lLeftBottom->setColor(lMainColor);
    lLeftBottom->setColor(lNiceGradColor0, lNiceGradColor1, lNiceGradColor2, lNiceGradColor3);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kAuto;
    lLeftBottom->setSizeConstraint(lSizeConstraint);
    lLeftBottom->setFillDirection(kFillDirectionVertical);
    lLeftBottom->setPadding(4);
    lLeftBottom->setBevel(-2);
    lLeftBottom->setAlignment(lCenterCenterAlignment);

    // left top
    lBounds.h = 250;
    GUIGradientPanel* lLeftTop = new GUIGradientPanel(mID++, lBounds);
    lLeftTop->setColor(lMainColor);
    lLeftTop->setColor(lNiceGradColor0, lNiceGradColor1, lNiceGradColor2, lNiceGradColor3);
    lSizeConstraint.w = kAuto;
    lSizeConstraint.h = kChangable;
    lLeftTop->setSizeConstraint(lSizeConstraint);
    lLeftTop->setFillDirection(kFillDirectionVertical);
    lLeftTop->setPadding(4);
    lLeftTop->setBevel(-2);
    lLeftTop->setAlignment(lCenterCenterAlignment);

    #if FILE_BROWSER
    lBounds.w = 150;
    lBounds.h = 200;
    Path lPath = Path(".");
    GUIFileBrowser * lFileBrowser = GUIFileBrowser::create(mID++, lBounds, lPath, lLeftTop);
    #else
    // a label
    lBounds.w = 150;
    lBounds.h = 12;
    GUILabel* lLeftTopLabel = new GUILabel(mID++, lBounds, L"top left view");
    lLeftTopLabel->setLabelColor(lLabelColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lLeftTopLabel->setSizeConstraint(lSizeConstraint);
    lLeftTopLabel->setFillDirection(kFillDirectionVertical);
    lLeftTopLabel->setPadding(0);
    #endif

    // a label
    lBounds.w = 150;
    lBounds.h = 12;
    GUILabel* lLeftBottomLabel = new GUILabel(mID++, lBounds, L"Öl");
    lLeftBottomLabel->setLabelColor(lLabelColor);
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lLeftBottomLabel->setSizeConstraint(lSizeConstraint);
    lLeftBottomLabel->setFillDirection(kFillDirectionVertical);
    lLeftBottomLabel->setPadding(0);


    // some fun stuff in the middle
    lBounds.w = 95;
    lBounds.h = 16;
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    GUILabel* lLabel1 = new GUILabel(mID++, lBounds, L"Text Option:");
    GUILabel* lLabel2 = new GUILabel(mID++, lBounds, L"Int Option:");
    GUILabel* lLabelFloat_001 = new GUILabel(mID++, lBounds, L"Float (0.01):");
    GUILabel* lLabelFloat_050 = new GUILabel(mID++, lBounds, L"Float (0.5):");
    GUILabel* lCheckBoxLabel = new GUILabel(mID++, lBounds, L"A CheckBox:");
    GUILabel* lRadioLabel = new GUILabel(mID++, lBounds, L"RadioButtons:");
    lLabel1->setSizeConstraint(lSizeConstraint);
    lLabel2->setSizeConstraint(lSizeConstraint);
    lLabelFloat_001->setSizeConstraint(lSizeConstraint);
    lLabelFloat_050->setSizeConstraint(lSizeConstraint);
    lCheckBoxLabel->setSizeConstraint(lSizeConstraint);
    lRadioLabel->setSizeConstraint(lSizeConstraint);
    lLabel1->setLabelColor(lLabelColor);
    lLabel2->setLabelColor(lLabelColor);
    lLabelFloat_001->setLabelColor(lLabelColor);
    lLabelFloat_050->setLabelColor(lLabelColor);
    lCheckBoxLabel->setLabelColor(lLabelColor);
    lRadioLabel->setLabelColor(lLabelColor);

    Rectangle2D lBounds2;
    lBounds2.w = 300;
    lBounds2.h = 230;
    GUILabel* lFreetypeLabel = new GUILabel(mID++, lBounds2, L"Welcåme");
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;
    lFreetypeLabel->setSizeConstraint(lSizeConstraint);
    lFreetypeLabel->setTextRenderer(TextRenderer_Pretty);

    lBounds.w = 50;
    GUITextBox* lText1 = new GUITextBox(mID++, lBounds, L"");
    GUINumberBox* lIntBox = new GUINumberBox(mID++, lBounds, L"0", kNumberInteger);
    GUINumberBox* lFloatBox_001 = new GUINumberBox(mID++, lBounds, L"0", kNumberFloat);
    GUINumberBox* lFloatBox_050 = new GUINumberBox(mID++, lBounds, L"0", kNumberFloat);

    lText1->setSizeConstraint(lSizeConstraint);
    lIntBox->setSizeConstraint(lSizeConstraint);
    lFloatBox_001->setSizeConstraint(lSizeConstraint);
    lFloatBox_050->setSizeConstraint(lSizeConstraint);

    lText1->setColor(lTextBoxColor);
    lIntBox->setColor(lTextBoxColor);
    lFloatBox_001->setColor(lTextBoxColor);
    lFloatBox_050->setColor(lTextBoxColor);

    lText1->setTextColor(lLabelColor);
    lIntBox->setTextColor(lLabelColor);
    lFloatBox_001->setTextColor(lLabelColor);
    lFloatBox_050->setTextColor(lLabelColor);

    GUIButton* lButton01 = new GUIButton(mID++, lBounds, L"No");
    GUIButton* lButton02 = new GUIButton(mID++, lBounds, L"Yes");
    GUIButton* lButtonFloat_001 = new GUIButton(mID++, lBounds, L"OK");
    GUIButton* lButtonFloat_050 = new GUIButton(mID++, lBounds, L"OK");
    lButton01->setSizeConstraint(lSizeConstraint);
    lButton02->setSizeConstraint(lSizeConstraint);
    lButtonFloat_001->setSizeConstraint(lSizeConstraint);
    lButtonFloat_050->setSizeConstraint(lSizeConstraint);
    lButton01->setColor(lButtonColor);
    lButton02->setColor(lButtonColor);
    lButtonFloat_001->setColor(lButtonColor);
    lButtonFloat_050->setColor(lButtonColor);
    lButton01->setLabelColor(lButtonLabelColor);
    lButton02->setLabelColor(lButtonLabelColor);
    lButtonFloat_001->setLabelColor(lButtonLabelColor);
    lButtonFloat_050->setLabelColor(lButtonLabelColor);

    NumberOptionFloat* lNumberOptionFloat_001 = new NumberOptionFloat(-100.0f,
                                                                      100.0f,
                                                                      0.0f,
                                                                      0.01f);
    NumberOptionFloat* lNumberOptionFloat_050 = new NumberOptionFloat(-100.0f,
                                                                      100.0f,
                                                                      0.0f,
                                                                      0.50f);
    NumberOptionInteger* lNumberOptionInteger = new NumberOptionInteger(-100.0f,
                                                                        100.0f,
                                                                        0.0f,
                                                                        1.0f);
    StringOption* lStringOption = new StringOption(L"Hello");

    // for the checkbox
    BooleanOption* lBooleanOption = new BooleanOption(false);
    // for the group of radiobuttons
    BooleanOption* lBooleanOption1 = new BooleanOption(true);
    BooleanOption* lBooleanOption2 = new BooleanOption(false);
    BooleanOption* lBooleanOption3 = new BooleanOption(false);

    lBounds.w = 16;
    lBounds.h = 100;
    lSizeConstraint.w = kLocked;
    lSizeConstraint.h = kLocked;

    GUISlider* lSliderInteger = new GUISlider(mID++, lBounds, false);
    lSliderInteger->setIsHorizontal(false);
    lSliderInteger->setSizeConstraint(lSizeConstraint);
    lSliderInteger->setColor(lMainColor);
    lSliderInteger->setHandleColor(lButtonColor);

    lBounds.w = 100;
    lBounds.h = 16;

    GUISlider* lSliderFloat_001 = new GUISlider(mID++, lBounds, true);
    lSliderFloat_001->setSizeConstraint(lSizeConstraint);
    lSliderFloat_001->setColor(lMainColor);
    lSliderFloat_001->setHandleColor(lButtonColor);

    GUISlider* lSliderFloat_050 = new GUISlider(mID++, lBounds, true);
    lSliderFloat_050->setSizeConstraint(lSizeConstraint);
    lSliderFloat_050->setColor(lMainColor);
    lSliderFloat_050->setHandleColor(lButtonColor);

    lBounds.w = 18;
    lBounds.h = 18;
    GUIKnob* lKnobInteger = new GUIKnob(mID++, lBounds);
    lKnobInteger->setSizeConstraint(lSizeConstraint);
    lKnobInteger->setColor(lMainColor);
    lKnobInteger->setHandleColor(lButtonColor);
    lKnobInteger->setTickColor(lTickColor);

    GUIKnob* lKnobFloat_001 = new GUIKnob(mID++, lBounds);
    lKnobFloat_001->setSizeConstraint(lSizeConstraint);
    lKnobFloat_001->setColor(lMainColor);
    lKnobFloat_001->setHandleColor(lButtonColor);
    lKnobFloat_001->setTickColor(lTickColor);

    GUIKnob* lKnobFloat_050 = new GUIKnob(mID++, lBounds);
    lKnobFloat_050->setSizeConstraint(lSizeConstraint);
    lKnobFloat_050->setColor(lMainColor);
    lKnobFloat_050->setHandleColor(lButtonColor);
    lKnobFloat_050->setTickColor(lTickColor);

    lBounds.w = 16;
    lBounds.h = 16;
    GUICheckBox* lCheckBox = new GUICheckBox(mID++, lBounds);
    lCheckBox->setSizeConstraint(lSizeConstraint);
    lCheckBox->setColor(lButtonColor);
    lCheckBox->setTickColor(lTickColor);

    // GUIRadioButton stuff
    GUIRadioButton* lRadioButton1 = new GUIRadioButton(mID++, lBounds);
    GUIRadioButton* lRadioButton2 = new GUIRadioButton(mID++, lBounds);
    GUIRadioButton* lRadioButton3 = new GUIRadioButton(mID++, lBounds);
    lRadioButton1->setSizeConstraint(lSizeConstraint);
    lRadioButton2->setSizeConstraint(lSizeConstraint);
    lRadioButton3->setSizeConstraint(lSizeConstraint);
    lRadioButton1->setColor(lButtonColor);
    lRadioButton2->setColor(lButtonColor);
    lRadioButton3->setColor(lButtonColor);
    lRadioButton1->setTickColor(lTickColor);
    lRadioButton2->setTickColor(lTickColor);
    lRadioButton3->setTickColor(lTickColor);

    lBounds.w = 100;
    lBounds.h = 24;
    RadioButtonGroup* lRadioGroup = new RadioButtonGroup();

    lRadioGroup->addRadioButton(lRadioButton1);
    lRadioGroup->addRadioButton(lRadioButton2);
    lRadioGroup->addRadioButton(lRadioButton3);

    lBounds.w = 64;
    lBounds.h = 16;
    GUIComboBox* lComboBox = new GUIComboBox(mID++, lBounds);
    lComboBox->setColor(lButtonColor);
    lComboBox->setTickColor(lTickColor);
    GUIComboBoxItem* lComboBoxItem01 = new GUIComboBoxItem(mID++, lBounds, L"Hello1");
    lComboBoxItem01->setColor(lButtonColor);
    lComboBoxItem01->setLabelColor(lLabelColor);
    lComboBoxItem01->setBooleanOption(lBooleanOption1);
    GUIComboBoxItem* lComboBoxItem02 = new GUIComboBoxItem(mID++, lBounds, L"Hello2");
    lComboBoxItem02->setColor(lButtonColor);
    lComboBoxItem02->setLabelColor(lLabelColor);
    lComboBoxItem02->setBooleanOption(lBooleanOption2);
    GUIComboBoxItem* lComboBoxItem03 = new GUIComboBoxItem(mID++, lBounds, L"Hello3");
    lComboBoxItem03->setColor(lButtonColor);
    lComboBoxItem03->setLabelColor(lLabelColor);
    lComboBoxItem03->setBooleanOption(lBooleanOption3);

    GUITexturePanel* lTexturePanel1 = new GUITexturePanel(mID++, lBounds);
    lTexturePanel1->setColor(lDarkDetailColor);

    lBounds.w = 352;
    lBounds.h = 288;

    UInt08* lImageData = (UInt08*) gImageData;
    lTexturePanel1->setTextureProperties(lBounds.w, lBounds.h, kColorYUV420, kLinearNearest);
    lTexturePanel1->setAutoResize(true);
    lTexturePanel1->setAnimatable(true);

    GUITexturePanel* lTexturePanel2 = new GUITexturePanel(mID++, lBounds);
    lTexturePanel2->setColor(lDarkDetailColor);

    lBounds.w = 352;
    lBounds.h = 288;

    lTexturePanel2->setTextureProperties(lBounds.w, lBounds.h, kColorRGB24, kLinearNearest);
    lTexturePanel2->setAutoResize(true);
    lTexturePanel2->setAnimatable(true);

    // GUIBitmapPanel
    lBounds.w = 352;
    lBounds.h = 288;
    GUIBitmapPanel* lBitmapPanel = new GUIBitmapPanel(mID++, lBounds);
    lBitmapPanel->setBitmapProperties(lBounds.w, lBounds.h, kColorYUV420);
    lBitmapPanel->setColor(lDarkDetailColor);
    lBitmapPanel->setZoomFactor(1.0f);

    // step buttons
    GUIStepButtonPair* lStepButtonsInteger = new GUIStepButtonPair(mID++, kFillDirectionVertical);
    GUIStepButtonPair* lStepButtonsFloat_001 = new GUIStepButtonPair(mID++, kFillDirectionVertical);
    GUIStepButtonPair* lStepButtonsFloat_050 = new GUIStepButtonPair(mID++, kFillDirectionVertical);
    lStepButtonsInteger->setColor(lButtonColor);
    lStepButtonsFloat_001->setColor(lButtonColor);
    lStepButtonsFloat_050->setColor(lButtonColor);
    lStepButtonsInteger->setArrowColor(lTickColor);
    lStepButtonsFloat_001->setArrowColor(lTickColor);
    lStepButtonsFloat_050->setArrowColor(lTickColor);

    // connect gui elements to options
    // int option
    lSliderInteger->setNumberOption(lNumberOptionInteger);
    lIntBox->setNumberOption(lNumberOptionInteger);
    lStepButtonsInteger->setNumberOption(lNumberOptionInteger);
    lKnobInteger->setNumberOption(lNumberOptionInteger);
    // float option 0.01 step
    lSliderFloat_001->setNumberOption(lNumberOptionFloat_001);
    lFloatBox_001->setNumberOption(lNumberOptionFloat_001);
    lStepButtonsFloat_001->setNumberOption(lNumberOptionFloat_001);
    lKnobFloat_001->setNumberOption(lNumberOptionFloat_001);
    // float option 0.5 step
    lSliderFloat_050->setNumberOption(lNumberOptionFloat_050);
    lFloatBox_050->setNumberOption(lNumberOptionFloat_050);
    lStepButtonsFloat_050->setNumberOption(lNumberOptionFloat_050);
    lKnobFloat_050->setNumberOption(lNumberOptionFloat_050);

    // string option
    lText1->setStringOption(lStringOption);
    // boolean options
    lCheckBox->setBooleanOption(lBooleanOption);
    lRadioButton1->setBooleanOption(lBooleanOption1);
    lRadioButton2->setBooleanOption(lBooleanOption2);
    lRadioButton3->setBooleanOption(lBooleanOption3);

    lMainView->addChild(lUberTopView);
        lUberTopView->addChild(lMenuBar);
            lMenuBar->addChild(lMenuA0);
                lMenuA0->addChild(lMenuA01);
                    lMenuA01->addChild(lMenuA010);
                    lMenuA01->addChild(lMenuA011);
                lMenuA0->addChild(lMenuA02);
                    lMenuA02->addChild(lMenuA020);
                    lMenuA02->addChild(lMenuA021);
            lMenuBar->addChild(lMenuB0);
                lMenuB0->addChild(lMenuB01);
                    lMenuB01->addChild(lMenuB010);
                    lMenuB01->addChild(lMenuB011);
                lMenuB0->addChild(lMenuB02);
                    lMenuB02->addChild(lMenuB020);
                    lMenuB02->addChild(lMenuB021);
                        lMenuB021->addChild(lMenuB0210);
                        lMenuB021->addChild(lMenuB0211);
    lMainView->addChild(lTopView);
        lTopView->addChild(lTopLeftView);
            lTopLeftView->addChild(lButton1);
            lTopLeftView->addChild(lButton2);
            lTopLeftView->addChild(lButton3);
            lTopLeftView->addChild(lButton4);
            lTopLeftView->addChild(lFullScreenLabel);
            lTopLeftView->addChild(lMenuBar);
        lTopView->addChild(lTopRightView);
            lTopRightView->addChild(lButton5);
            lTopRightView->addChild(lButton6);
    lMainView->addChild(lBottomView);
        lBottomView->addChild(lLeftView);
        lBottomView->addChild(lRightView);
            lRightView->addChild(lRightInnerView);
                lRightInnerView->addChild(lLayerPanel);
                    lLayerPanel->addChild(lLayerA);
                        lLayerA->addChild(lFreetypeLabel);
                    lLayerPanel->addChild(lLayer0);
                        lLayer0->addChild(lLabel1);
                        lLayer0->addChild(lText1);
                        lLayer0->addChild(lButton01);
                    lLayerPanel->addChild(lLayer1);
                        lLayer1->addChild(lLabel2);
                        lLayer1->addChild(lIntBox);
                        lLayer1->addChild(lStepButtonsInteger);
                        lLayer1->addChild(lSliderInteger);
                        lLayer1->addChild(lKnobInteger);
                        lLayer1->addChild(lButton02);
                    lLayerPanel->addChild(lLayer2);
                        lLayer2->addChild(lLayer2Row0);
                            lLayer2Row0->addChild(lLabelFloat_001);
                            lLayer2Row0->addChild(lFloatBox_001);
                            lLayer2Row0->addChild(lStepButtonsFloat_001);
                            lLayer2Row0->addChild(lSliderFloat_001);
                            lLayer2Row0->addChild(lKnobFloat_001);
                            lLayer2Row0->addChild(lButtonFloat_001);
                        lLayer2->addChild(lLayer2Row1);
                            lLayer2Row1->addChild(lLabelFloat_050);
                            lLayer2Row1->addChild(lFloatBox_050);
                            lLayer2Row1->addChild(lStepButtonsFloat_050);
                            lLayer2Row1->addChild(lSliderFloat_050);
                            lLayer2Row1->addChild(lKnobFloat_050);
                            lLayer2Row1->addChild(lButtonFloat_050);
                    lLayerPanel->addChild(lLayer3);
                        lLayer3->addChild(lCheckBoxLabel);
                        lLayer3->addChild(lCheckBox);
                    lLayerPanel->addChild(lLayer4);
                        lLayer4->addChild(lRadioLabel);
                        lLayer4->addChild(lRadioButton1);
                        lLayer4->addChild(lRadioButton2);
                        lLayer4->addChild(lRadioButton3);
                        lLayer4->addChild(lComboBox);
                            lComboBox->addChild(lComboBoxItem01);
                            lComboBox->addChild(lComboBoxItem02);
                            lComboBox->addChild(lComboBoxItem03);
                    lLayerPanel->addChild(lLayer5);
                        lLayer5->addChild(lTexturePanel1);
                    lLayerPanel->addChild(lLayer6);
                        lLayer6->addChild(lBitmapPanel);
                    lLayerPanel->addChild(lLayer7);
                        lLayer7->addChild(lTexturePanel2);

                    lLayerPanel->setLayerLabelText(0, L"welcome");
                    lLayerPanel->setLayerLabelText(1, L"text");
                    lLayerPanel->setLayerLabelText(2, L"int");
                    lLayerPanel->setLayerLabelText(3, L"float");
                    lLayerPanel->setLayerLabelText(4, L"check");
                    lLayerPanel->setLayerLabelText(5, L"radio and combo");
                    lLayerPanel->setLayerLabelText(6, L"texture");
                    lLayerPanel->setLayerLabelText(7, L"bitmap");
                    lLayerPanel->setLayerLabelText(8, L"texture 2");
            lLeftView->addChild(lLeftTop);
                #if FILE_BROWSER
                lLeftTop->addChild(lFileBrowser);
                #else
                lLeftTop->addChild(lLeftTopLabel);
                #endif
            lLeftView->addChild(lLeftBottom);
                lLeftBottom->addChild(lLeftBottomLabel);

    lMainWindow->setFocusColor(lFocusColor);

    // init window and graphics
    // init global controller class (inits GLUT)
    //      this should set the neccessary gl callbacks
    lMainController->init(argc, argv, lMainWindow);

    lTexturePanel1->setTextureData(lImageData);
    Path lImagePath = Path("/home/nilsa/Pictures/screen.jpg");
    lTexturePanel2->loadTextureData(lImagePath);
    lBitmapPanel->setBitmapData(lImageData);
    // free(lImageData);
    // lImageData = 0;

    // start main loop in global controller class
    // this starts the event handling loop
    // XXX !!! no code below this line will be executed !!! XXX
    lMainController->start();

    GUIElement::removeTree(lMainView);
    delete lMainWindow;
    delete lNumberOptionFloat_001;
    delete lNumberOptionFloat_050;
    delete lStringOption;
    delete lBooleanOption;
    delete lBooleanOption1;
    delete lBooleanOption2;
    delete lBooleanOption3;
    delete lRadioGroup;

    return 0;
}

