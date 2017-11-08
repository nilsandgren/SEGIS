//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  TimedObservable:
//      Interface for an object that can be observed by a timed observer to get
//      updated at a specific rate (see TimedObserver). Implementing class should call
//      each attacehd observer at its specific rate.
//

#pragma once

#include "Types.h"

class TimedObserver;

class TimedObservable
{
    public:
        TimedObservable() {}
        virtual ~TimedObservable() {}

        // add an observer to the list of observable objects
        virtual Status addTimedObserver(TimedObserver* aObserver) = 0;
        // remove an observer from the list of observable objects
        virtual Status removeTimedObserver(TimedObserver* aObserver) = 0;
};
