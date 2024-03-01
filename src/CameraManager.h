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
    static String m_PictureName;
    String TakePicture();
    void InitMicroSDCard();
    void printCardType();
    static void SendPicture(WebserverPictureInfo PictureInfo);
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