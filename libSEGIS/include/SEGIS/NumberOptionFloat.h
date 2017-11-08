//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  NumberOptionFloat: 
//      Option class for floating point values
//

#pragma once

#include "Types.h"
#include "NumberOption.h"

class NumberOptionFloat : public NumberOption
{
    // variables
    protected:

    // functions
    protected:

    public:
        NumberOptionFloat();
        NumberOptionFloat(Float32 aMinValue,
                          Float32 aMaxValue,
                          Float32 aDefaultValue,
                          Float32 aValueStep);
        virtual ~NumberOptionFloat();

        virtual void setValue(Observer* aObserver, Float32 aValue);
        virtual Float32 getValue(void);
};
