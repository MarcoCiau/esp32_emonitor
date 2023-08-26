#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H
#include <Arduino.h>
#include <EEPROM.h>

class EEPROMStorage
{
private:
    /* data */
    void write8(uint8_t addr, uint8_t data);
    uint8_t read8(uint8_t addr);

    void write16(uint8_t addr, uint16_t data);
    uint16_t read16(uint8_t addr);

    void write32(uint8_t addr, uint32_t data);
    uint32_t read32(uint8_t addr);
public:
    EEPROMStorage();
    void begin();
    void resetDefaults();
    void setCalibration(uint8_t channel, uint32_t value);
    uint32_t getCalibration(uint8_t channel);
};

extern EEPROMStorage storage;

#endif