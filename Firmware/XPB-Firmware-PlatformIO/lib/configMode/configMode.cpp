#include "configMode.h"
#include "lvgl_ui.h"
#include "currentSensor.h"

void ConfigMode::handleConfigMode(lv_obj_t*& ui_container, SysConfig& sysConfig, sysConfig_data& tmp_cfg_data) {
    // Lock the config state mutex to ensure other tasks see the correct state
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        // Ensure the isActive flag is set
        configState.isActive = true;
        configState.cursor = 0;
        configState.cursorLast = 0;
        configState.cursorStatus = 0;
        xSemaphoreGive(configStateMutex);
    }
    
    // Reset any pending UI change flags before creating the config UI
    functionMode_ChangeRequested = false;
    highLightChannel_ChangeRequested = false;
    
    // Init the config mode UI
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE) {
        lv_obj_clean(lv_scr_act());
        ui_container = configMode_initUI(tft_Rotation);
        forceUpdate_flag = 1;
        lv_disp_t *disp = lv_disp_get_default();
        lv_refr_now(disp);
        xSemaphoreGive(lvglMutex);
    }
}

void ConfigMode::applyConfigData(SysConfig& sysConfig, float& shuntResistorCHA, float& shuntResistorCHB, uint8_t& highLightChannel, volatile function_mode& current_functionMode) {
    shuntResistorCHA = sysConfig.cfg_data.shuntResistorCHA / 1000.0f;  // Convert from mΩ to Ω
    shuntResistorCHB = sysConfig.cfg_data.shuntResistorCHB / 1000.0f;  // Convert from mΩ to Ω
    highLightChannel = sysConfig.cfg_data.default_channel;
    current_functionMode = (function_mode)sysConfig.cfg_data.default_mode;
}

bool ConfigMode::enterConfigMode() {
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = true;
        configState.cursor = 0;     // Reset cursor position
        configState.cursorLast = 0; // Reset last cursor position
        configState.cursorStatus = 0; // Reset cursor status
        xSemaphoreGive(configStateMutex);
        
        // Reset any pending UI change requests
        functionMode_ChangeRequested = false;
        highLightChannel_ChangeRequested = false;
        
        return !wasActive;
    }
    return false;
}

bool ConfigMode::exitConfigMode() {
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        bool wasActive = configState.isActive;
        configState.isActive = false;
        configState.cursor = 0;
        configState.cursorLast = 0;
        configState.cursorStatus = 0;
        xSemaphoreGive(configStateMutex);
        
        // Set flag to rebuild the UI with the current function mode
        functionMode_ChangeRequested = true;
        
        return wasActive;
    }
    return false;
}

void ConfigMode::updateConfigCursor(int8_t newCursor) {
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        if (newCursor >= 0 && newCursor < configState.cursorMax) {
            configState.cursorLast = configState.cursor;
            configState.cursor = newCursor;
        }
        xSemaphoreGive(configStateMutex);
    }
}

bool ConfigMode::getConfigState(ConfigModeState *state) {
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        memcpy(state, &configState, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}

bool ConfigMode::updateConfigState(const ConfigModeState *state) {
    if (xSemaphoreTake(configStateMutex, portMAX_DELAY) == pdTRUE) {
        memcpy(&configState, state, sizeof(ConfigModeState));
        xSemaphoreGive(configStateMutex);
        return true;
    }
    return false;
}

