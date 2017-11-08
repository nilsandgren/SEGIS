//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOptionInteger: 
//      Implementation
//


#include <SEGIS/Util.h>
#include <SEGIS/GraphicsUtil.h>
#include <SEGIS/NumberOptionInteger.h>

NumberOptionInteger::NumberOptionInteger()
    : NumberOption()
{
}

NumberOptionInteger::NumberOptionInteger(Float32 aMinValue,
                                        Float32 aMaxValue,
                                        Float32 aDefaultValue,
                                        Float32 aValueStep)
    : NumberOption(aMinValue, aMaxValue, aDefaultValue, aValueStep)
{
    mValueStep = (Float32) (SInt32) (aValueStep);
}

NumberOptionInteger::~NumberOptionInteger()
{
}

void
NumberOptionInteger::setValue(Observer* aObserver, Float32 aValue)
{
    // TODO use value step here
    mCurrentValue = (Float32) ((SInt32) min(mMaxValue, max(mMinValue, aValue)));
    notifyObservers(aObserver);
}

Float32
NumberOptionInteger::getValue(void)
{
    return mCurrentValue;
}
