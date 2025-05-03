#ifndef CONFIG_MODE_H
#define CONFIG_MODE_H

#include <Arduino.h>
#include "sysConfig.h"
#include "lvgl.h"
#include "function_mode.h"

// Forward declarations
extern SemaphoreHandle_t lvglMutex;
extern uint8_t forceUpdate_flag;
extern volatile int tft_Rotation;

// Config mode state
struct ConfigModeState {
    bool isActive;
    int8_t cursor;
    int8_t cursorLast;
    int8_t cursorStatus;
    int8_t cursorMax;
};

class ConfigMode {
public:
    ConfigMode() : configState{false, 0, 0, 0, 11} {} // when added new config, update cursorMax here !!!
    ConfigModeState configState;
    bool enterConfigMode();
    bool exitConfigMode();
    void updateConfigCursor(int8_t newCursor);
    bool getConfigState(ConfigModeState *state);
    bool updateConfigState(const ConfigModeState *state);
    void handleConfigMode(lv_obj_t*& ui_container, SysConfig& sysConfig, sysConfig_data& tmp_cfg_data);
    void applyConfigData(SysConfig& sysConfig, float& shuntResistorCHA, float& shuntResistorCHB, uint8_t& highLightChannel, volatile function_mode& current_functionMode);
};

// Declare global instance
extern ConfigMode configMode;
extern SemaphoreHandle_t configStateMutex;

#endif // CONFIG_MODE_H
