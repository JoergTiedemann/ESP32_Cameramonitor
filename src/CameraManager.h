#ifndef CameraMan_H
#define CameraMan_H

#include <Arduino.h>
// #include <FirebaseJson.h>

class CCameraManager
{

private:

public : 
    void begin();
    String m_PictureName;
    String TakePicture();
    void InitMicroSDCard();
    void DeInitMicroSDCard();

    CCameraManager()
	{
	}

    ~CCameraManager()
	{
	}

};

extern CCameraManager CameraManager;

#endif