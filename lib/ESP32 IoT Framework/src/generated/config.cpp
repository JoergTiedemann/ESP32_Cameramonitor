#include <Arduino.h>
#include "config.h"

uint32_t configVersion = 1525968926; //generated identifier to compare config with EEPROM

const configData defaults PROGMEM =
{
	"Pumpen-Monitor",
	"V4.1",
	false,
	false,
	true,
	true,
	"de",
	300,
	120,
	200,
	true,
	15000,
	"",
	false,
	false,
	"http://www.bierhoehle.de/pumpe.bin"
};