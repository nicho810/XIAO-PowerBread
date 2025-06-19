#include "input_ButtonX3.h"

InputButtonX3::InputButtonX3(uint8_t pin)
{
    pin_ = pin;
    pinMode(pin_, INPUT_PULLUP);
}

InputButtonX3::~InputButtonX3()
{
}

int InputButtonX3::readRawValue()
{
    return analogRead(pin_);
}

void InputButtonX3::reset_keystate(ButtonState_X3& buttonState)
{
    buttonState.key_shortPressed = false;
    buttonState.key_shortPressed_value = 0;
    // buttonState.key_longPressed = false;
    // buttonState.key_longPressed_value = 0;
    // long_pressed_detect_time_button2 = 0;
}

void InputButtonX3::update(ButtonState_X3& buttonState)
{
    /*
    * Button Analog Value Table
    * ----------------------
    * Range     | Status | ADC Reading
    * 0-800      | no pressed   | ~0
    * 800-1300    | button1_short_pressed   | ~1130
    * 1300-1800   | button2_short_pressed   | ~1513
    * 1800-2500   | button3_short_pressed   | ~2298
    */

    uint16_t analogValue = readRawValue();
    if (analogValue < 800) {
        reset_keystate(buttonState);
    }
    else if (analogValue > 800 && analogValue < 1300 && lastAnalogValue_ > 800 && lastAnalogValue_ < 1300) {
        buttonState.key_shortPressed_value = 1;
    }
    else if (analogValue > 1300 && analogValue < 1800 && lastAnalogValue_ > 1300 && lastAnalogValue_ < 1800) {
        buttonState.key_shortPressed_value = 2;
    }
    else if (analogValue > 1800 && analogValue < 2500 && lastAnalogValue_ > 1800 && lastAnalogValue_ < 2500) {
        buttonState.key_shortPressed_value = 3;
    }
    lastAnalogValue_ = analogValue;


    // Set key_shortPressed based on value and only if the value is changed
    if (buttonState.key_shortPressed_value != 0 && buttonState.key_shortPressed_value != buttonState_last_.key_shortPressed_value) {
        buttonState.key_shortPressed = true;
    } else {
        buttonState.key_shortPressed = false;
    }

    // At the end, update the last state
    buttonState_last_ = buttonState;
}

bool InputButtonX3::isPressed_short(uint8_t button, ButtonState_X3& buttonState)
{
    if (buttonState.key_shortPressed && buttonState.key_shortPressed_value == button)
    {
        return true;
    }
    return false;
}

