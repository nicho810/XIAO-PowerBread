#ifndef ATCOMMAND_H
#define ATCOMMAND_H

#include "Arduino.h"
#include "sysConfig.h"

extern SysConfig sysConfig;


class ATCommand {
public:
    ATCommand();
    ~ATCommand();

    bool processCommand(String& command, String& response);
    bool sendError(String& error);
    bool sendOK(String& response);

private:
    typedef bool (*ATHandler)(String&, String&);

    struct ATCommandEntry {
        const char* command;
        ATHandler handler;
        int argStartIndex;
    };

    // Handler functions as static private methods
    static bool handleAT(String& cmd, String& resp);
    static bool handleConfig(String& cmd, String& resp);
    static bool handleReload(String& cmd, String& resp);
    static bool handleDefaultMode(String& cmd, String& resp);
    static bool handleDefaultChannel(String& cmd, String& resp);
    static bool handleShuntResistorCh1(String& cmd, String& resp);
    static bool handleShuntResistorCh2(String& cmd, String& resp);
    static bool handleShuntResistorCh3(String& cmd, String& resp);
    static bool handleShuntResistorCh4(String& cmd, String& resp);
    static bool handleSerialEnable(String& cmd, String& resp);
    static bool handleSerialBaudrate(String& cmd, String& resp);
    static bool handleSerialMode(String& cmd, String& resp);
    static bool handleSerialPrintInterval(String& cmd, String& resp);
    static bool handleChartUpdateInterval(String& cmd, String& resp);
    static bool handleChartScaleMode(String& cmd, String& resp);
    static bool handleChartScale(String& cmd, String& resp);
    static bool handleWifiEnable(String& cmd, String& resp);
    static bool handleWifiSsid(String& cmd, String& resp);
    static bool handleWifiPassword(String& cmd, String& resp);
    static bool handleWebMode(String& cmd, String& resp);
    static bool handleWebInterval(String& cmd, String& resp);

    static const ATCommandEntry commandTable[];
};

#endif