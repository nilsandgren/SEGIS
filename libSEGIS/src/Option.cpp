//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Option: 
//      Implementation
//


#include <SEGIS/Option.h>
#include <SEGIS/Observer.h>

Option::Option()
{
}

Option::~Option()
{
}

const WString & 
Option::getStringValue(void)
{
    return mStringValue;
}
        
Status
Option::addObserver(Observer* aObserver)
{
    mObservers.push_back(aObserver);
    return kSucceeded;
}

Status
Option::removeObserver(Observer* aObserver)
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

void
Option::notifyObservers(const Observer* aCaller)
{
    // tell all observers, except the one responsible for the change
    // about our current state
    for (UInt32 lIndex = 0; lIndex < mObservers.size(); lIndex++)
    {
        if (mObservers[lIndex] != aCaller)
        {
            mObservers[lIndex]->update(this);
        }
    }
}
