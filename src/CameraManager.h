#ifndef CameraMan_H
#define CameraMan_H

#include <Arduino.h>
// #include <FirebaseJson.h>
#include "webServer.h"

class CCameraManager
{

private:

public : 
    void begin();
    String m_PictureName;
    String m_PictureNameSimple;
    String TakePicture();
    void InitMicroSDCard();
    void printCardType();
    void SendPicture(AsyncWebServerRequest *request);
    File myFile; 
    CCameraManager()
	{
	}

    ~CCameraManager()
	{
	}

};

extern CCameraManager CameraManager;

#endif