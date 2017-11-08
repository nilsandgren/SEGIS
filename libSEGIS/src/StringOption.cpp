//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Option: 
//      Implementation
//


#include <SEGIS/Option.h>
#include <SEGIS/StringOption.h>
#include <SEGIS/Observer.h>

StringOption::StringOption(const WString & aString)
    : Option()
{
    mStringValue = WString(aString);
}

StringOption::~StringOption()
{
}

void
StringOption::setStringValue(Observer* aCaller, const WString & aString)
{
    // mStringValue = WString(aString);
    mStringValue = aString;
#   ifdef DEBUG_OPTIONS
    fprintf(stderr, "StringOption::setStringValue - set to: %s\n", mStringValue);
#   endif
    notifyObservers(aCaller);
}

const WString &
StringOption::getStringValue(void)
{
    return mStringValue;
}
