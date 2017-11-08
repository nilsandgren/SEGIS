//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  TimedObserver:
//      This interface is used to observe a timed observable (see TimedObservable) to 
//      get updated at a specific rate
//

#pragma once

#include "Types.h"

class TimedObservable;

class TimedObserver
{
    private:
        TimeStamp mUpdateInterval;
        TimeStamp mLastUpdateTime;
        
    public:
        TimedObserver() 
            : mUpdateInterval(1.0)
            , mLastUpdateTime(0.0)
        {
        }

        virtual ~TimedObserver() 
        {
        }

        TimeStamp getLastUpdateTime(void) { return mLastUpdateTime; }
        void      setLastUpdateTime(TimeStamp aTime) { mLastUpdateTime = aTime; }

        TimeStamp getUpdateInterval(void) { return mUpdateInterval; }
        void      setUpdateInterval(TimeStamp aTime) { mUpdateInterval = aTime; }

        // this function will be called in the observer when it needs updating
        virtual Status timedUpdate(void) = 0;
};
