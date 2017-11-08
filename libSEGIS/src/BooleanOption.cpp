//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  BoolenOption: 
//      Implementation
//

#include <SEGIS/Option.h>
#include <SEGIS/BooleanOption.h>
#include <SEGIS/Observer.h>

BooleanOption::BooleanOption()
    : Option()
    , mDefaultValue(false)
    , mCurrentValue(mDefaultValue)
{
}

BooleanOption::BooleanOption(bool aDefaultValue)
    : Option()
    , mDefaultValue(aDefaultValue)
    , mCurrentValue(mDefaultValue)
{
}

BooleanOption::~BooleanOption()
{
}

void 
BooleanOption::setValue(Observer* aObserver, bool aValue)
{
    mCurrentValue = aValue;
#   ifdef DEBUG_OPTIONS
    fprintf(stderr, "BooleanOption::setValue - value set to %d\n", mCurrentValue);
#   endif
    notifyObservers(aObserver);
}

bool 
BooleanOption::getValue(void)
{
    return mCurrentValue;
}
