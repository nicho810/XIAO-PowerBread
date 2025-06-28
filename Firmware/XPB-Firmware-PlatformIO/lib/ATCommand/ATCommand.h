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
};

#endif