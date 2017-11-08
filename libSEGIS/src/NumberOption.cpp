//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOption: 
//      Implementation
//


#include <SEGIS/NumberOption.h>

NumberOption::NumberOption()
    : Option()
{
}

NumberOption::NumberOption(Float32 aMinValue,
                           Float32 aMaxValue,
                           Float32 aDefaultValue,
                           Float32 aValueStep)
    : Option()
    , mMinValue(aMinValue)
    , mMaxValue(aMaxValue)
    , mDefaultValue(aDefaultValue)
    , mValueStep(aValueStep)
    , mCurrentValue(mDefaultValue)

{
}

NumberOption::~NumberOption()
{
}

Float32
NumberOption::increaseValue(Observer* aObserver)
{
    mCurrentValue += mValueStep;
    mCurrentValue =  getValidValue(mCurrentValue);
    notifyObservers(aObserver);
    return mCurrentValue;
}

Float32
NumberOption::decreaseValue(Observer* aObserver)
{
    mCurrentValue -= mValueStep;
    mCurrentValue =  getValidValue(mCurrentValue);
    notifyObservers(aObserver);
    return mCurrentValue;
}

Float32 
NumberOption::getValidValue(Float32 aValue)
{
    Float32 lValid = min(mMaxValue, max(mMinValue, aValue));
    // use mValueStep if != 0.0f
    if (0.0f != mValueStep)
    {
        // XXX BUG - bug 0006 seems to be here
        // some kind of nasty rounding errors with the float
        SInt32 lStepMultiple = 0;
        if (lValid < 0.0f)
        {
            lStepMultiple = (SInt32) ((lValid / mValueStep) - mValueStep/2.0f);
        }
        else
        {
            lStepMultiple = (SInt32) ((lValid / mValueStep) + mValueStep/2.0f);
        }
        lValid = lStepMultiple * mValueStep;
#       ifdef DEBUG_OPTIONS
        fprintf(stderr, "NumberOption::getValidValue - %f -> %f\n", aValue, lValid);
#       endif
    }
    return lValid;
}
