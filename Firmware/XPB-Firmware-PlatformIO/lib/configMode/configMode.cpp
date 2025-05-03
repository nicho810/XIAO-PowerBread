#include "configMode.h"
#include "lvgl_ui.h"
#include "INA3221Sensor.h"

void ConfigMode::handleConfigMode(lv_obj_t*& ui_container, SysConfig& sysConfig, sysConfig_data& tmp_cfg_data) {
    // init the config mode UI
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
        lv_obj_clean(lv_scr_act());
        ui_container = configMode_initUI(tft_Rotation);
        forceUpdate_flag = 1;
        lv_disp_t *disp = lv_disp_get_default();
        lv_refr_now(disp);
        xSemaphoreGive(lvglMutex);
    }

    // Wait for the user to exit the config mode
    while (configState.isActive) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // save the config data to EEPROM
    sysConfig.saveConfig_to_EEPROM(tmp_cfg_data);
    Serial.println("> Config data saved to EEPROM.");
    sysConfig.loadConfig_from_EEPROM();
    Serial.println(sysConfig.output_all_config_data_in_String());
    Serial.flush();
}

void ConfigMode::applyConfigData(SysConfig& sysConfig, float& shuntResistorCHA, float& shuntResistorCHB, uint8_t& highLightChannel, volatile function_mode& current_functionMode) {
    shuntResistorCHA = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;
    shuntResistorCHB = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;
    highLightChannel = sysConfig.cfg_data.default_channel;
    current_functionMode = (function_mode)sysConfig.cfg_data.default_mode;
}

bool ConfigMode::enterConfigMode() {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = true;
        xSemaphoreGive(configStateMutex);
        return !wasActive;
    }
    return false;
}

bool ConfigMode::exitConfigMode() {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = false;
        configState.cursor = 0;
        configState.cursorLast = 0;
        configState.cursorStatus = 0;
        xSemaphoreGive(configStateMutex);
        return wasActive;
    }
    return false;
}

void ConfigMode::updateConfigCursor(int8_t newCursor) {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        if (newCursor >= 0 && newCursor < configState.cursorMax) {
            configState.cursorLast = configState.cursor;
            configState.cursor = newCursor;
        }
        xSemaphoreGive(configStateMutex);
    }
}

bool ConfigMode::getConfigState(ConfigModeState *state) {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        memcpy(state, &configState, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}

bool ConfigMode::updateConfigState(const ConfigModeState *state) {
    if (xSemaphoreTake(configStateMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        memcpy(&configState, state, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}

// // If configMode_initUI is a UI function, move its implementation here
// lv_obj_t* configMode_initUI(int rotation) {
//     // ... implementation ...
//     return nullptr; // Replace with actual UI object
// }
