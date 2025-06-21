#ifndef INPUT_BUTTONX3_H
#define INPUT_BUTTONX3_H

#include <Arduino.h>
#include "boardConfig.h"


// Class of button state
class ButtonState_X3
{
    public:
        bool key_shortPressed = false;
        uint8_t key_shortPressed_value = 0;
};

struct inputEventMessage {
    ButtonState_X3 buttonState;
    uint32_t timestamp;
};

class InputButtonX3
{
    public:
        InputButtonX3(uint8_t pin);
        ~InputButtonX3();
        void update(ButtonState_X3& buttonState);
        bool isPressed_short(uint8_t button, ButtonState_X3& buttonState);
        void reset_keystate(ButtonState_X3& buttonState);
        int readRawValue();
    private:
        uint8_t pin_;
        uint16_t lastAnalogValue_ = 0;
        ButtonState_X3 buttonState_last_;
};

#endif