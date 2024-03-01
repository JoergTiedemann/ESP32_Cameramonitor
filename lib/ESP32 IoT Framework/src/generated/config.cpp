#include <Arduino.h>
#include "config.h"

uint32_t configVersion = 140516194; //generated identifier to compare config with EEPROM

const configData defaults PROGMEM =
{
	"Camera-Monitor",
	"V0.1",
	false,
	false,
	true,
	true,
	"de",
	300,
	20,
	120,
	200,
	true,
	15000,
	"",
	false,
	false,
	"http://www.bierhoehle.de/camera.bin"
};