#ifndef DISPLAYMAN_H
#define DISPLAYMAN_H

#include <Arduino.h>


class CDisplayManager
{

private:
	int m_BkgndColor;
	int m_FgndColor;

public : 
    void begin();
    void ClearScreen(int BackColor);
    void Ausgabe(bool bInitScreen);

    
    CDisplayManager()
	{
		m_BkgndColor = 0;
		m_FgndColor = 0;
	}

    ~CDisplayManager()
	{
	}

};

extern CDisplayManager DisplayManager;

#endif


