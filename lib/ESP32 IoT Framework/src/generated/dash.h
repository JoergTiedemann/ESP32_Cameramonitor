#ifndef DASH_H
#define DASH_H

struct dashboardData
{
	uint16_t aktuelleLaufzeit;
	float Temperatur;
	bool Pumpenzustand;
	bool PumpenAbschaltError;
	bool RuntimeMonitor;
	bool Testschalter;
	bool QueryTest;
	bool MessageTest;
	uint16_t AnzeigeinMin;
};

#endif