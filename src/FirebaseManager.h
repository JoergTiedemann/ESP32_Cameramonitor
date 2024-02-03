#ifndef FIREBASEMAN_H
#define FIREBASEMAN_H

#include <Arduino.h>
// #include <FirebaseJson.h>

class CFirebaseManager
{

private:

public : 
    void begin();
    void loop();
    void firebaseSignIn();
    void clearDatabase();
    void QueryTest();
    void QueryTageswerte();
    void QueryTageswerteDiag();
    void SendTopicTestMessage();
    void SendTopicAlarmMessage();
    void SendTopicWarnMessage();
    void WriteFirebasePumpenzustand(bool bWithLogging = false,bool bCalcWerte = false);
    void WriteFirebaseTemperatur();
    void BeendeStream();
    void StarteStream();

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


