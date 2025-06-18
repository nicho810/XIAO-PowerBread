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

bool InputButtonX3::update(ButtonState_X3& buttonState)
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

    ButtonState_X3 buttonState_last;
    buttonState_last = buttonState;

    uint16_t analogValue = readRawValue();
    if (analogValue < 800) {
        buttonState.button1_short_pressed = false;
        buttonState.button2_short_pressed = false;
        buttonState.button3_short_pressed = false;
    }
    if (analogValue > 800 && analogValue < 1300) {
        buttonState.button1_short_pressed = true;
    }
    if (analogValue > 1300 && analogValue < 1800) {
        buttonState.button2_short_pressed = true;
    }
    if (analogValue > 1800 && analogValue < 2500) {
        buttonState.button3_short_pressed = true;
    }

    //check if button state is changed
    if (buttonState_last.button1_short_pressed != buttonState.button1_short_pressed ||
        buttonState_last.button2_short_pressed != buttonState.button2_short_pressed ||
        buttonState_last.button3_short_pressed != buttonState.button3_short_pressed) {
        return true;
    }
    return false;
}

bool InputButtonX3::isPressed_short(uint8_t button, ButtonState_X3& buttonState)
{
    if (button == 1) {
        return buttonState.button1_short_pressed;
    }
    if (button == 2) {
        return buttonState.button2_short_pressed;
    }
    if (button == 3) {
        return buttonState.button3_short_pressed;
    }
}

bool InputButtonX3::isPressed_long(uint8_t button, ButtonState_X3& buttonState)
{
    if (button == 1) {
        return buttonState.button1_long_pressed;
    }
    if (button == 2) {
        return buttonState.button2_long_pressed;
    }
    if (button == 3) {
        return buttonState.button3_long_pressed;
    }
}

void InputButtonX3::reset_keystate(ButtonState_X3& buttonState)
{
    buttonState.button1_short_pressed = false;
    buttonState.button2_short_pressed = false;
    buttonState.button3_short_pressed = false;
    buttonState.button1_long_pressed = false;
    buttonState.button2_long_pressed = false;
    buttonState.button3_long_pressed = false;
}