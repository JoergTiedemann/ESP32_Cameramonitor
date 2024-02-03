#ifndef BOARDINFO_H
#define BOARDINFO_H

#include <Arduino.h>

class CBoardsInformation
{
public : 
    void PrintBoardInformation();
    CBoardsInformation()
	{
	}
    ~CBoardsInformation()
	{
	}
};
extern CBoardsInformation BoardInformation;

#endif
