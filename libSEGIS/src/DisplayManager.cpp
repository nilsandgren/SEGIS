//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  DisplayManager: 
//      Implementation
//

#include <SEGIS/DisplayManager.h>
#include <SEGIS/MainController.h>
#include <SEGIS/Window.h>

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{
}

DisplayManager*
DisplayManager::mTheDisplayManager = 0;

DisplayManager*
DisplayManager::getDisplayManager()
{
    if (0 == mTheDisplayManager)
    {
        mTheDisplayManager = new DisplayManager();
    }
    return mTheDisplayManager;
}

void 
DisplayManager::requestUpdate(GUIElement* aRoot)
{
    // check if GL has been initialzed. only draw if that is the case.
    MainController* lMainController = MainController::getMainController();
    if (lMainController->isInitialized())
    {
        aRoot->drawTree();
        Window* lWindow = aRoot->getParentWindow();
        if (lWindow)
        {
            deque< GUIElement* > * lOverlayElements = lWindow->getOverlayElements();
            auto lOverlayItem = lOverlayElements->begin();
            for ( ; lOverlayItem != lOverlayElements->end(); ++ lOverlayItem)
            {
                (*lOverlayItem)->draw();
            }
        }
        else
        {
            fprintf(stderr, 
                    "DisplayManager::requestUpdate - element %d (%p) has no parent window.\n",
                    aRoot->getID(),
                    (void*)aRoot);
        }
        glutSwapBuffers();
    }
}
