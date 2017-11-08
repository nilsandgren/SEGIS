//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Observer:
//      This interface is used to observe an observable
//

#pragma once

#include "Types.h"

class Observable;

class Observer
{
    public:
        Observer() {}
        virtual ~Observer() {}

        // this function will be called when the observers need updating
        virtual Status update(const Observable* aObservable) = 0;
};
