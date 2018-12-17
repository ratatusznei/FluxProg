//biblioteca feita para comunicacao sem fio via bluetooth

#ifndef _BLUETOOTH_HPP_
#define _BLUETOOTH_HPP_
#include <Arduino.h>
#include "robotCte.hpp"

class Bluetooth{
    char* packet;
    uint8_t command;

public:
    Bluetooth();
    uint8_t getCommand();
    void sendPacket(uint8_t *readingBTS, unsigned int *readingU);
    ~Bluetooth(){}
};
#endif
