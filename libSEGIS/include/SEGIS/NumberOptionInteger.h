//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOptionInteger: 
//      Option class for integer values
//

#pragma once

#include "Types.h"
#include "NumberOption.h"

class NumberOptionInteger : public NumberOption
{
    // variables
    protected:

    // functions
    protected:

    public:
        NumberOptionInteger();
        NumberOptionInteger(Float32 aMinValue,
                            Float32 aMaxValue,
                            Float32 aDefaultValue,
                            Float32 aValueStep);
        virtual ~NumberOptionInteger();

        // easier to just keep float here
        virtual void setValue(Observer* aObserver, Float32 aValue);
        virtual Float32 getValue(void);
};
