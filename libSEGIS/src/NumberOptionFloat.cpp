//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOptionFloat: 
//      Implementation
//


#include <SEGIS/Util.h>
#include <SEGIS/GraphicsUtil.h>
#include <SEGIS/NumberOptionFloat.h>

NumberOptionFloat::NumberOptionFloat()
    : NumberOption()
{
}

NumberOptionFloat::NumberOptionFloat(Float32 aMinValue,
                                     Float32 aMaxValue,
                                     Float32 aDefaultValue,
                                     Float32 aValueStep)
    : NumberOption(aMinValue, aMaxValue, aDefaultValue, aValueStep)
{
}

NumberOptionFloat::~NumberOptionFloat()
{
}

void
NumberOptionFloat::setValue(Observer* aObserver, Float32 aValue)
{
    mCurrentValue = min(mMaxValue, max(mMinValue, aValue));
    notifyObservers(aObserver);
}

Float32
NumberOptionFloat::getValue(void)
{
    return mCurrentValue;
}
