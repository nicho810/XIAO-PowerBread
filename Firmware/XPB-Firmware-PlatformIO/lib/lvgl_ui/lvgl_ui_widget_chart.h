#ifndef LVGL_WIDGET_CHART_H
#define LVGL_WIDGET_CHART_H

#include "lvgl_ui_widget.h"


// Data Chart Widget Class
class Widget_DataChart: public Widget_Base {
private:
    lv_obj_t* chart;
    lv_chart_series_t* series;

public:
    Widget_DataChart(uint16_t x, uint16_t y, lv_color_t color, lv_color_t color_dark = xpb_color_Background);
    
    void addDataPoint(int32_t value);
    void setRange(int32_t min, int32_t max);
    void setPointCount(uint16_t count);
    lv_chart_series_t* getSeries() const { return series; }
};





#endif
