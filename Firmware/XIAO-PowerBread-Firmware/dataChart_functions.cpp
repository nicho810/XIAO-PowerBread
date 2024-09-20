#include "dataChart_functions.h"
#include "dataMonitor_functions.h" //only need dataMonitor_update_chAB_xy_by_Rotation()

// dataChart functions
void dataChart_changeRotation(int rotation) {
  tft.initR(INITR_GREENTAB);
  tft.setRotation(rotation);  //Rotate the LCD 180 degree (0-3)
  dataMonitor_update_chAB_xy_by_Rotation(rotation); //reset this variable in case of switch back to dataMonitor
}
