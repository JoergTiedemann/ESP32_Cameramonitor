#ifndef FIREBASEMAN_H
#define FIREBASEMAN_H

#include <Arduino.h>
// #include <FirebaseJson.h>
// By John Main - TronicsBench.com
//
const int ledPin = 4; // FLASH LIGHT LED on GPIO4
const int redled = 33; // Other LED on GPIO33

class CFirebaseManager
{

private:

public : 
    void begin();
    void loop();
    void firebaseSignIn();
    void clearDatabase();
    void SendTopicTestMessage();
    void SendTopicAlarmMessage();
    void SendTopicWarnMessage();
    void BeendeStream();
    void StarteStream();
    void UploadPicture(String filename);

    FirebaseJson json;
    FirebaseJson jsonTemp;
    
    String timePath;
    String pummpenzustandPath;
    String pummpenabschalterrorPath;
    String pummpenruntimemonitorPath;
    String pummpenaktuellelaufzeitPath;
    String pummpentageslaufzeitPath;
    String pummpenloggingtageszyklenPath;
    String pummpenloggingPath;
    String pummpenloggingtageslaufzeitPath;
    String pummpenloggingtimestampPath;
    String pummpentageeswertePath;

    String tempAkttempPath;
    String temploggingPath;
    String temploggingAktTemp;
    String temploggingMinTemp;
    String temploggingMaxTemp;
    String temploggingtimestampPath;
    String temptageswertePath;

    String parentPath;
    String TempParentPath;
    String streamPath;
    bool m_bFirebaseTokenReady;
    bool m_bConnectState;
    bool m_httpConnected;
    bool m_bNoInit;
    unsigned long sendDataPrevMillis;    
    unsigned long sendTempPrevMillis;    
    unsigned long m_count;
    long m_CloudAbschaltError;
    bool m_bCloundWarnmeldung;
    bool m_bTempLogging;
    long m_CloudRuntimeMonitor;
    bool m_bNotabschaltNachrichtgesendet;
    bool m_bStreambeendenWegenFehler;
    bool m_bStreamIsRunning;

    CFirebaseManager()
	{
	}

    ~CFirebaseManager()
	{
	}

};

extern CFirebaseManager FirebaseManager;

#endif


