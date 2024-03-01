#include <Arduino.h>
#include "config.h"

uint32_t configVersion = 842860115; //generated identifier to compare config with EEPROM

const configData defaults PROGMEM =
{
	"Camera-Monitor",
	"V0.1",
	false,
	false,
	true,
	true,
	"de",
	8,
	10,
	0,
	false,
	false,
	true,
	0,
	false,
	15000,
	"",
	false,
	false,
	"http://www.bierhoehle.de/camera.bin"
};