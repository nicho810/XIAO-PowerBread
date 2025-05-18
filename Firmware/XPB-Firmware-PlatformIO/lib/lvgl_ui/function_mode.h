#ifndef FUNCTION_MODE_H
#define FUNCTION_MODE_H

#include <stdint.h>  // Add this line for uint8_t

// Function mode enum definition
enum function_mode
{
    Mode_1,  // Basic Monitor Mode (previously dataMonitor)
    Mode_2,  // Chart Mode (previously dataMonitorChart)
    Mode_3,  // Count Mode (previously dataMonitorCount)
    Mode_4,  // Reserved for future use
};

// Mode descriptions (can be used for UI labels)
struct ModeConfig {
    const char* name;        // Mode name for display
    uint8_t containers;      // Number of containers needed
    bool needsHighlight;     // Whether this mode uses channel highlighting
    bool continuousUpdate;   // Whether this mode needs continuous updates
};

// Mode configurations
extern const ModeConfig MODE_CONFIGS[];

#endif // FUNCTION_MODE_H