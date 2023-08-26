#include "EEPROMStorage.h"
//Private Methods
void EEPROMStorage::write8(uint8_t addr, uint8_t data)
{
  EEPROM.write(addr, data);
  EEPROM.commit();
}

uint8_t EEPROMStorage::read8(uint8_t addr)
{
  uint8_t data = EEPROM.read(addr);

  if (data == 0xFF) data = 0;
  return data;
}

void EEPROMStorage::write16(uint8_t addr, uint16_t data)
{
  EEPROM.write(addr, (data & 0xFF00) >> 8);
  EEPROM.write(addr + 1, (data & 0x00FF));
  EEPROM.commit();
}

uint16_t EEPROMStorage::read16(uint8_t addr)
{
  uint16_t data =  ( (EEPROM.read(addr)) << 8) | ( EEPROM.read(addr + 1) ); 
  if (data == 0xFFFF) data = 0;
  return data;
}

void EEPROMStorage::write32(uint8_t addr, uint32_t data)
{
  EEPROM.write(addr, (data >> 24) & 0xFF);
  EEPROM.write(addr + 1, (data >> 16) & 0xFF);
  EEPROM.write(addr + 2, (data >> 8) & 0xFF);
  EEPROM.write(addr + 3, (data) & 0xFF);
  EEPROM.commit();

}

uint32_t EEPROMStorage::read32(uint8_t addr)
{
  uint32_t data = 0;
  data = (EEPROM.read(addr + 3) & 0x000000FF);
  data = data | (EEPROM.read(addr + 2) << 8);
  data = data | (EEPROM.read(addr + 1) << 16);
  data = data | (EEPROM.read(addr) << 24);
  return data;
}


EEPROMStorage::EEPROMStorage()
{
}

void EEPROMStorage::begin()
{
  EEPROM.begin(512);
}

void EEPROMStorage::resetDefaults()
{

}

void EEPROMStorage::setCalibration(uint8_t channel, uint32_t value)
{
  uint8_t start = 0;
  write32(start + channel, value);
}

uint32_t EEPROMStorage::getCalibration(uint8_t channel)
{
  uint8_t start = 0;
  return read32(start + channel);
}


EEPROMStorage storage;