/**
 * [INPUT]: Arduino.h, boardConfig.h (pin_dial)
 * [OUTPUT]: DialFunction 类 (init/read/readDialStatus)
 * [POS]: 旋钮输入抽象，ADC 值 → 离散事件 (0=reset, 1=up, 2=down, 3=press)
 * [PROTOCOL]: 变更时更新此头部，然后检查 CLAUDE.md
 */

#ifndef dialSwitch_h
#define dialSwitch_h

#include <Arduino.h>
#include "boardConfig.h"


// extern volatile int dialStatus;
// extern volatile int lastDialStatus;


class DialFunction {
    public:
        void init();
        int read();
        int readDialStatus();  // Change return type to int
};

#endif