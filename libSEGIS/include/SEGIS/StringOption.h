//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  StringOption: 
//      Options which holds a string value
//

#pragma once

#include "Types.h"
#include "Option.h"
#include "Observable.h"
#include <deque>
using namespace std;

class Observer;

class StringOption : public Option
{
    // variables
    protected:

    // functions
    protected:

    public:
        StringOption(const WString & aString);
        virtual ~StringOption();

        virtual const WString & getStringValue();
        virtual void setStringValue(Observer* aCaller,
                                    const WString & aString);
};
