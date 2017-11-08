//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Option: 
//      Base class for options which holds some kind of value.
//

#pragma once

#include "Types.h"
#include "Observable.h"
#include <deque>
using namespace std;

class Observer;

class Option : public Observable
{
    // variables
    protected:
        deque<Observer*> mObservers;
        WString mStringValue; 

    // functions
    protected:
        // notify observers about the change in state of this option
        // tell all observers except the one responsible
        // for the change
        void notifyObservers(const Observer* aObserver);

    public:
        Option();
        virtual ~Option();

        virtual const WString & getStringValue(void);
        
        // implement the Observable interface
        virtual Status addObserver(Observer* aObserver);
        virtual Status removeObserver(Observer* aObserver);
};
