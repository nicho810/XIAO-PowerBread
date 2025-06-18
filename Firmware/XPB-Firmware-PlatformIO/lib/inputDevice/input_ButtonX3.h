#ifndef INPUT_BUTTONX3_H
#define INPUT_BUTTONX3_H

#include <Arduino.h>
#include "boardConfig.h"

// Class of button state
class ButtonState_X3
{
    public:
        bool button1_long_pressed = false;
        bool button2_long_pressed = false;
        bool button3_long_pressed = false;
        bool button1_short_pressed = false;
        bool button2_short_pressed = false;
        bool button3_short_pressed = false;
        bool key_shortPressed = false;
        uint8_t key_shortPressed_value = 0;
};


class InputButtonX3
{
    public:
        InputButtonX3(uint8_t pin);
        ~InputButtonX3();
        bool update(ButtonState_X3& buttonState);
        bool isPressed_short(uint8_t button, ButtonState_X3& buttonState);
        bool isPressed_long(uint8_t button, ButtonState_X3& buttonState);
        void reset_keystate(ButtonState_X3& buttonState);
        int readRawValue();
    private:
        uint8_t pin_;
};




#endif