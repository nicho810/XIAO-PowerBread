#ifndef dialSwitch_h
#define dialSwitch_h

#include <Arduino.h>
#include "../boardConfig.h"


// extern volatile int dialStatus;
// extern volatile int lastDialStatus;


class DialFunction {
    public:
        void init();
        int read();
        int readDialStatus();  // Change return type to int
};

#endif