#include "function_mode.h"

const ModeConfig MODE_CONFIGS[] = {
    // Mode_1 (Basic Monitor)
    {
        .name = "Basic Monitor",
        .containers = 2,
        .needsHighlight = false,
        .continuousUpdate = false
    },
    // Mode_2 (Chart)
    {
        .name = "Chart View",
        .containers = 2,
        .needsHighlight = true,
        .continuousUpdate = true
    },
    // Mode_3 (Count)
    {
        .name = "Statistics",
        .containers = 5,
        .needsHighlight = true,
        .continuousUpdate = true
    },
    // Mode_4 (Future use)
    {
        .name = "Reserved",
        .containers = 1,
        .needsHighlight = false,
        .continuousUpdate = false
    }
};
