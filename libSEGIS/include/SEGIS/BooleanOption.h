//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  BoolenOption: 
//      Option holding a boolean value
//

#pragma once

#include "Types.h"
#include "Option.h"

class Observer;

class BooleanOption : public Option
{
    // variables
    protected:
        bool mDefaultValue;
        bool mCurrentValue;

    // functions
    protected:

    public:
        BooleanOption();
        BooleanOption(bool aDefaultValue);
        virtual ~BooleanOption();

        virtual void setValue(Observer* aObserver, bool aValue);
        virtual bool getValue(void);

        bool getDefaultValue()
        {
            return mDefaultValue;
        }
        virtual void setDefaultValue(bool aDefaultValue)
        {
            mDefaultValue = aDefaultValue;
        }
};
