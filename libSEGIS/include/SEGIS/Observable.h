//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Observable:
//      Interface for an object that can be observed by an observer
//

#pragma once

#include "Types.h"

class Observer;

class Observable
{
    public:
        Observable() {}
        virtual ~Observable() {}

        // add an observer to the list of observable objects
        virtual Status addObserver(Observer* aObserver) = 0;
        // remove an observer from the list of observable objects
        virtual Status removeObserver(Observer* aObserver) = 0;
};
