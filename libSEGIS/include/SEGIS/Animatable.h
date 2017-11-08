//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Animatable:
//      Interface for an object that can animate itself
//

#pragma once

#include "Types.h"

class Animatable
{
    public:
        Animatable() {}
        virtual ~Animatable() {}

        // called when object should update it's animation state.
        // no drawing should be done in this function, redisplay of the 
        // object will be taken care of if implementing class is a subclass
        // of GUIElement and resides within a Window.
        // return true if redisplay is needed
        virtual bool updateAnimation(void) = 0;
};
