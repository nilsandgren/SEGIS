//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOption: 
//      Virtual base class for Option holding a number value
//      Takes floating point values as arguments since I'm lazy
//

#pragma once

#include "Types.h"
#include "Option.h"

class Observer;

class NumberOption : public Option
{
    // variables
    protected:
        Float32 mMinValue;
        Float32 mMaxValue;
        Float32 mDefaultValue;
        Float32 mValueStep;
        Float32 mCurrentValue;

    // functions
    protected:

    public:
        NumberOption();
        NumberOption(Float32 aMinValue,
                     Float32 aMaxValue,
                     Float32 aDefaultValue,
                     Float32 aValueStep);
        virtual ~NumberOption();

        virtual void setValue(Observer* aObserver, Float32 aValue) = 0;
        virtual Float32 getValue(void) = 0;

        Float32 getMinValue()
        {
            return mMinValue;
        }
        virtual void setMinValue(Float32 aMinValue)
        {
            mMinValue = aMinValue;
        }
        Float32 getMaxValue()
        {
            return mMaxValue;
        }
        virtual void setMaxValue(Float32 aMaxValue)
        {
            mMaxValue = aMaxValue;
        }
        Float32 getDefaultValue()
        {
            return mDefaultValue;
        }
        virtual void setDefaultValue(Float32 aDefaultValue)
        {
            mDefaultValue = aDefaultValue;
        }
        Float32 getValueStep()
        {
            return mValueStep;
        }
        virtual void setValueStep(Float32 aValueStep)
        {
            mValueStep = aValueStep;
        }

        // increases/decrease value by value step
        virtual Float32 increaseValue(Observer* aObserver);
        virtual Float32 decreaseValue(Observer* aObserver);

        // returns a value in bounds of min/max
        // should also take value step in account
        Float32 getValidValue(Float32 aValue);
};
