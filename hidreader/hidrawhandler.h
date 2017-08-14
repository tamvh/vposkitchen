#ifndef HIDRAWHANDLER_H
#define HIDRAWHANDLER_H
#include "hidapi/hidapi/hidapi.h"
#include <iostream>
#include <iomanip>
#include "utils.h"

class HidrawHandler
{
public:
    HidrawHandler();
    void handler();

    static bool checkHid(uint8_t vid, uint8_t pid);
protected:
    static bool mappingChar(uint8_t* buffer, int len, char& out);
private:
    bool _forceHexData;
};

#endif // HIDRAWHANDLER_H
