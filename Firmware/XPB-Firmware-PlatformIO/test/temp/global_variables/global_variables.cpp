#include "global_variables.h"

// Define the variable with the 'weak' attribute to avoid multiple definition errors
__attribute__((weak)) volatile bool ui_initialization_in_progress = false;

