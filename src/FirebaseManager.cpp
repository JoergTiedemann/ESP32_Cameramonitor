
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "FirebaseManager.h"
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


#include "configManager.h"
#include "DiagManager.h"
#include "dashboard.h"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCSwdcpciYBuYy4NgA3kNYhaqirZZ5n-Xw "
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://espdata-b473e-default-rtdb.europe-west1.firebasedatabase.app" 


//#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-kouyy@espdata-b473e.iam.gserviceaccount.com"
//const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQClzHhsqFU5rPsZ\nkcfFZr2Z4K7AlSCmhyuVuCGDTzC1WmcebkcOheI3r/MvAd/BF8fu7bOQ7H2fCVCD\nVVG/zyqhnlbnfwot0Ou+rwbfbVqM8FXOu3vOQUIosRrr+wjXfwPMJdWbkINL9iqC\n4P2V28I6RDHSH+hMtXXnwNg99u8l9ad81mttX17aIZJy6I49meMOoWHOjdxZzmEB\njPgL4tdz02PoUqaVqC2DkVn3Xek0GOJ5iBrcdwAwWenHDz2fHVA1H1dbkLaSVhJ0\nNgCHQrAgjZ3hgGZetZw9pkL7xowLEBqfNCdL8ZGecoHRx5qtL4ppfRSSwxi+48bD\nTweQ5jwVAgMBAAECggEAGkfOYN7P34az/fTAdzcBX8ZLEkYGeQ8HpsGbN6P8+F9z\nj/xym/KMjeorFcI/dEKE6Qeu77kPcn7jTxCzlr/2UHb3lys+cvkLnTFDhB/ie2D5\ny6f73buKCpwuxHAJZ0CABW3hLY7wbkEqB46Knk9JZWX7ha8+vwYBF0gB9CnTn4RO\n1+Wv9jrgG3Rb0trP3B49xtkPlxuK+gUfCLnApFhSKtZ05OIoy8EpiYLjhrlDShT4\n5ZJnXty9WTuA6oPg2xXeSJCM2HmCPLvrGHwDJ4FRpb73b58vdjr0+3VFwY5PBOBR\nzqwgRhzFqeUuyDzh0u6OLaKunHwUI158l2K197YP4wKBgQDdsN2v7zmJiWj+iFdT\nZXMnNUePYLqVvNWFzKKgA4xtDuIFdvQCxuNR6tqUmNZgmB/hC9c6hsm30kPCW6ca\ncIrcUaIck8XQ9C4qQFuxIsSGsHPaibJJCVKxvRW8KEuKAq/XhbvRxG5trhs7y/TU\nlNIrIjQbzLOd7uvUAeW9ygo6SwKBgQC/dTjN/mmgkfxgmF6zkfGdBaKO/w/ordm1\nsvasRx8oEiI/XxZJVekShLJ1G2Gcdp+L3rsqlkO72wMF3fiOIIcsYAQ5cenwNmx8\nVRxLKFWzqlWXwkhDAuT/0zEZEpc5Cl4u9TJSzfI2vNX5K4pHkGGVX9Eg+rsp0JkV\nmbDik39nHwKBgAzY8R//ByCXXARUNmtY18GP7uO6QVXoKZ+lak9HpV20nRif+ign\nCol2yNAPw2y5pC9QllqH8Sy7/L1IvnhjyUhXWsO5NJ+VZw9/x1COqUD8/NnPXQuq\nhKZUROwwkaAf47zjvSy864Fjs6RVf4zKSxvA0bBCvq4KaJRSv9Ot38XdAoGASnCA\nQo9dcEHyd3gmsfjXkS6ksqZ3AAfAMwY/JGKOuZy7VhpZZvARK9MOM7wOqq/wfoh+\nCyPbeAnCViCkyFXbWKSIX/FrfKscaQcKdipbj/x1RQkT4YIZmWH6n/37cAEY93b8\nNve96gxnwXZtUD/RwSs8+26hILPrIVvdS2r/yQ8CgYAruqLyDeC+Uo/Ae3rlBov8\nvBXCZVU9mZZIRTk0ceO/4eDJ+36s7Aj049lR32fVrMHgxk1qyib57lboqpCnQ+Eg\nqHV96YuUzs7rWTYUYPqVSB4PYb/G1mDMQt/a6vq3gAU0AYMQNTj8ZShMsdumjfNI\nYMRZTSd0zYNsyk74Jlns3A==\n-----END PRIVATE KEY-----\n";
#define FIREBASE_FCM_SERVER_KEY "AAAADw9OqRk:APA91bGoPdboiH9dUAMlSMYNS-rEOAijcPmtUXR-qUQqpU4QZWpyf9-spBjrfSDNk0tdZHivXW9IwvzGXRdgbhNQIpiXjizJkpDmeG22Y2F6VWEeTrIa_Bx21dk4iP1X1-3ZlOd7oclv"


// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "pumpenmonitor@tiedemann.de"


// //Define Firebase Data object
FirebaseData fbdo;
//FirebaseData streamfbdo;
FirebaseData stream;
// extern void streamCallback(StreamData data);
void streamCallback(FirebaseStream data)
{
  // Serial.printf("stream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
  //               data.streamPath().c_str(),
  //               data.dataPath().c_str(),
  //               data.dataType().c_str(),
  //               data.eventType().c_str());
  // printResult(data); // see addons/RTDBHelper.h
  // Serial.println();

  // This is the size of stream payload received (current and max value)
  // Max payload size is the payload size under the stream path since the stream connected
  // and read once and will not update until stream reconnection takes place.
  // This max value will be zero as no payload received in case of ESP8266 which
  // BearSSL reserved Rx buffer size is less than the actual stream payload.
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());

  FirebaseJson *json = data.to<FirebaseJson *>();
  FirebaseJsonData result;
  json->get(result,"MaxOnTime");
  if (result.typeNum == FirebaseJson::JSON_INT )
  {
    Serial.println("MaxOnTime:"+String(result.intValue));
    configManager.data.MaxOnTime = result.intValue;
  }

  json->get(result,"PumpenFreigabe");
  if (result.typeNum == FirebaseJson::JSON_BOOL)
  {
    Serial.println("PumpenFreigabe:"+String(result.boolValue));
    // das hier ist falsch das muss in der Config DB wierder zurückgesetzt werden 
    // das soll hier nur ein Event zum Reset sein und in der Weboberflaeche soll das nur angezeigt ewrden wenn der Fehler auch aufgetreten ist 
    // dash.data.RuntimeMonitor = result.boolValue;
  }
  json->clear();
}

FirebaseAuth fbauth;
FirebaseConfig fbconfig;
bool bGlobalFirebaseError = false;

extern time_t now;

CFirebaseManager FirebaseManager;


void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    DiagManager.PushDiagData("stream timed out, resuming...");
    Serial.println("stream timed out, resuming...\n");
 }

  if (!stream.httpConnected())
  {
    DiagManager.PushDiagData("httpConnected Error code: %d, reason: %s", stream.httpCode(), stream.errorReason().c_str());
    FirebaseManager.m_bStreambeendenWegenFehler = true;
  }
}





void CFirebaseManager::firebaseSignIn()
{
    fbauth.user.email = USER_EMAIL;
    fbauth.user.password = configManager.data.FirebasePassword;
    fbconfig.api_key = API_KEY;
    /* Assign the RTDB URL (required) */
    fbconfig.database_url = DATABASE_URL; 

    fbauth.user.email = USER_EMAIL;
    fbauth.user.password = configManager.data.FirebasePassword;
    fbdo.setResponseSize(4096);

    // fbdo.setBSSLBufferSize(1024, 1024);
    // //Set the size of HTTP response buffers in the case where we want to work with large data.```
    // fbdo.setResponseSize(1024);


    Firebase.reconnectWiFi(true);

    /* Assign the callback function for the long running token generation task */
    fbconfig.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
      // Assign the maximum retry of token generation
    fbconfig.max_token_generation_retry = 5;
    //config.signer.preRefreshSeconds = 3420; // force token refresh in 3 minutes (57 minutes earlier)

    /** Timeouts setzen habe das gegenüber dem Dewfault massiv verlaengert. */
    //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
    fbconfig.timeout.wifiReconnect = 60 * 1000;
    //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
    fbconfig.timeout.socketConnection = 60 * 1000;
    //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
    fbconfig.timeout.sslHandshake = 2 * 60 * 1000;
    //Server response read timeout in ms (1 sec - 1 min).
    fbconfig.timeout.serverResponse = 60 * 1000;
    //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
    fbconfig.timeout.rtdbKeepAlive = 120 * 1000;
    //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
    fbconfig.timeout.rtdbStreamReconnect = 50 * 1000;
    //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
    //will return false (error) when it called repeatedly in loop.
    fbconfig.timeout.rtdbStreamError = 30 * 1000;



    /* Reset stored authen and config */
    Firebase.reset(&fbconfig);    
    Firebase.begin(&fbconfig, &fbauth);
    if (Firebase.ready())
    {   
        DiagManager.PushDiagData("Firebase Sign In Ready");
        Serial.println("Firebase Sign In Ready");
    }
    else
    {
        Serial.printf("Firebase Sign In NOT Ready Password:%s<-\n",configManager.data.FirebasePassword);
        DiagManager.PushDiagData("Firebase Sign In NOT Ready");
    }
}


void CFirebaseManager::clearDatabase() 
{
    
  String sensorPath = "/Wasserwerk/Pumpenmonitor/Pumpenstatus/pumpenlogging";
  QueryFilter query;
  //Get the current timestamp
  time_t current_ts = time(nullptr);

  //Define the total seconds of duration form now that you need to keep the data
//   unsigned long dataRetentionPeriod = 30 * 60 * 60 * 24; // 30 days
  unsigned long dataRetentionPeriod = 60; // 60 Seconds days

  double lastTS = current_ts - dataRetentionPeriod;

  Serial.printf("current :%lld lastTs:%f\n",current_ts,lastTS);

  //query for all data begin with time (timestamp) 0 to the last timestamp
  //limit the return result only last 8 data.
  query.orderBy("LoggingTimestamp").startAt(0).endAt(lastTS).limitToLast(8);

  if (Firebase.RTDB.getJSON(&fbdo, sensorPath, &query))
  {
    Serial.printf("datatype:%s length:%d String:%s\n",fbdo.dataType(),fbdo.jsonString().length(),fbdo.jsonString().c_str());
  
    if (fbdo.dataType() == "json" && fbdo.jsonString().length() > 4)
    {
      //parse the query result
      FirebaseJson *myJson = fbdo.jsonObjectPtr();
      size_t len = myJson->iteratorBegin();

      String key, value;
      int otype = 0;
      for (size_t i = 0; i < len; i++)
      {
        yield();
        myJson->iteratorGet(i, otype, key, value);
        if (otype == FirebaseJson::JSON_OBJECT && key.length() > 1)
        {
          //Here is the path of obsolete data in which its timestamp exceeds the specific period.
          String path = sensorPath + "/" + key;
          Serial.printf("Deleting path:%s\n",path.c_str());
          //Delete that data
        //   Firebase.deleteNode(firebaseData, path);
        }
      }
      myJson->iteratorEnd();
      myJson->clear();
    }
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
  query.clear();
}




void CFirebaseManager::SendTopicTestMessage()
{
    DiagManager.PushDiagData("Testnachricht an alle senden");
    Serial.println("SendTopic Message");
    

    // fbdo.fcm.setNotifyMessage("Meldung", "Die Wasserpumpe lief heute");
    // fbdo.fcm.setTopic("Pumpe");

    // FirebaseJson msg;
    // msg.add("myData", m_count);

    // fbdo.fcm.setDataMessage(msg.raw());

    // // Firebase.broadcastMessage(fbdo)
    // // Firebase.sendTopic(fbdo)
    // // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    // Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    // if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    //     Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}

void CFirebaseManager::SendTopicAlarmMessage()
{
    Serial.println("SendTopic Alarm Message");
    DiagManager.PushDiagData("Alarmnachricht senden weil Pumpe abgeschaltet wurde");

    // fbdo.fcm.setNotifyMessage("Alarmnachricht", "Die Wasserpumpe wurde notabgeschaltet. Sie läuft aktuell seid:");
    // fbdo.fcm.setTopic("Pumpe");

    // FirebaseJson msg;
    // msg.add("myData", m_count);

    // fbdo.fcm.setDataMessage(msg.raw());

    // // Firebase.broadcastMessage(fbdo)
    // // Firebase.sendTopic(fbdo)
    // // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    // Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    // if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    //     Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}


void CFirebaseManager::SendTopicWarnMessage()
{
    Serial.println("SendTopic Warn  Message");
    DiagManager.PushDiagData("Warnnachricht senden weil Pumpe lange läuft");

    // fbdo.fcm.setNotifyMessage("Warnmeldung", "Die Wasserpumpe läuft lange. Sie läuft aktuell seid:");
    // fbdo.fcm.setTopic("Pumpe");

    // FirebaseJson msg;
    // msg.add("myData", m_count);

    // fbdo.fcm.setDataMessage(msg.raw());

    // // Firebase.broadcastMessage(fbdo)
    // // Firebase.sendTopic(fbdo)
    // // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    // Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    // if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    //     Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}

void CFirebaseManager::BeendeStream()
{
    Firebase.RTDB.endStream(&fbdo);
    Firebase.RTDB.removeStreamCallback(&fbdo);
    // Firebase.end(stream); 
    stream.clear();   
    m_bStreamIsRunning = false;
    Serial.println("Stream beendet");
    DiagManager.PushDiagData("Firebase Stream beendet");
}

void CFirebaseManager::StarteStream()
{
    // und nun das Zeug für den Firebase Stream um benachrichtigt zu werden wenn Daten von der Webanwendung geaendert wurden
    if (!Firebase.RTDB.beginStream(&stream, streamPath))
      Serial.printf("Stream begin error, %s\n\n", stream.errorReason().c_str());
    Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
    m_bStreamIsRunning = true;
    // You can use TCP KeepAlive For more reliable stream operation and tracking the server connection status, please read this for detail.
    // https://github.com/mobizt/Firebase-ESP32#enable-tcp-keepalive-for-reliable-http-streaming
    // stream.keepAlive(5, 5, 1);
 
    Serial.println("Stream gestartet");
    DiagManager.PushDiagData("Firebase Stream gestartet");
}

//function to call in setup
void CFirebaseManager::begin()
{
  if ((configManager.data.FirebasePassword =="") || (configManager.data.FirebasePassword[0] == 0))
  {
    m_bNoInit = true;
    return;
  }
    m_bNoInit = false;
    m_bStreambeendenWegenFehler = false;
    m_bStreamIsRunning = false;
    timePath = "/timestamp";
    pummpenzustandPath = "/pumpenzustand";
    pummpenabschalterrorPath = "/PumpenAbschaltError";
    pummpenruntimemonitorPath = "/RuntimeMonitor";
    pummpenaktuellelaufzeitPath = "/aktuelleLaufzeit";
    pummpentageslaufzeitPath = "/TagesLaufzeit";
    pummpenloggingtageszyklenPath = "/Tageszyklen";
    pummpenloggingPath = "/pumpenlogging";
    pummpenloggingtageslaufzeitPath = "LoggingTagesLaufzeit";
    pummpenloggingtimestampPath = "LoggingTimestamp";
    pummpentageeswertePath = "/Tageswerte";

    tempAkttempPath = "/aktuelleTemp";
    temploggingPath = "/Temperaturlogging";
    temploggingAktTemp  = "AktTemp";
    temploggingMinTemp  = "MinTemp";
    temploggingMaxTemp  = "MaxTemp";
    temploggingtimestampPath = "LoggingTimestamp";
    temptageswertePath = "/Tageswerte";

    parentPath = "/Wasserwerk/Pumpenmonitor/Pumpenstatus";
    TempParentPath = "/Heizung/Heizungsmonitor/Heizungstatus";
    streamPath = "/Wasserwerk/PumpenConfig";
    m_bFirebaseTokenReady = false;
    m_bConnectState = false;
    m_httpConnected = false;
    sendDataPrevMillis = 0;
    sendTempPrevMillis = 0;
    m_count = 0;
    m_CloudAbschaltError = 0;
    m_CloudRuntimeMonitor = 0;
    m_bNotabschaltNachrichtgesendet = false;
    m_bTempLogging = false;

 //*************************************************************************************
    //hier nun das Ganze Firebase Zeug
    //*************************************************************************************
    /* Assign the api key (required) */
    fbconfig.api_key = API_KEY;
    /* Assign the RTDB URL (required) */
    fbconfig.database_url = DATABASE_URL; 

    fbauth.user.email = USER_EMAIL;
    fbauth.user.password = configManager.data.FirebasePassword;

    // /* Sign up */
    // if (Firebase.signUp(&config, &auth, USER_EMAIL, USER_PASSWORD;)){
    //     Serial.println("Firebase ok");
    //     signupOK = true;
    // }
    // else{
    //     Serial.printf("Firebase Error:%s\n", config.signer.signupError.message.c_str());
    // }

    //fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 512 /* Tx buffer size in bytes from 512 - 16384 */);
    //fbdo.setBSSLBufferSize(4096 , 512);
    // fbdo.setBSSLBufferSize(8192 , 1024);
    fbdo.setResponseSize(4096);

    // fbdo.setBSSLBufferSize(1024, 1024);
    // //Set the size of HTTP response buffers in the case where we want to work with large data.```
    // fbdo.setResponseSize(1024);


    Firebase.reconnectWiFi(true);

    /* Assign the callback function for the long running token generation task */
    fbconfig.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
      // Assign the maximum retry of token generation
    fbconfig.max_token_generation_retry = 5;
    //config.signer.preRefreshSeconds = 3420; // force token refresh in 3 minutes (57 minutes earlier)

    /** Timeouts setzen habe das gegenüber dem Dewfault massiv verlaengert. */
    //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
    fbconfig.timeout.wifiReconnect = 60 * 1000;
    //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
    fbconfig.timeout.socketConnection = 60 * 1000;
    //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
    fbconfig.timeout.sslHandshake = 2 * 60 * 1000;
    //Server response read timeout in ms (1 sec - 1 min).
    fbconfig.timeout.serverResponse = 60 * 1000;
    //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
    fbconfig.timeout.rtdbKeepAlive = 120 * 1000;
    //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
    fbconfig.timeout.rtdbStreamReconnect = 50 * 1000;
    //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
    //will return false (error) when it called repeatedly in loop.
    fbconfig.timeout.rtdbStreamError = 30 * 1000;

    firebaseSignIn();
    if (!Firebase.ready())
    {
      m_bNoInit = true;
      configManager.data.ConnectToCloud = false;
      return;
    }

    // Getting the user UID might take a few seconds
    Serial.println("Getting Firebase User UID");
     while ((fbauth.token.uid) == "") {
         Serial.print('.');
         delay(1000);
     }
    // Print user UID
    String uid;
    uid = fbauth.token.uid.c_str();
    Serial.print("Firebase User UID: ");
    Serial.println(uid);

    // so hier das Zeug fuer das Firebase Messaging
    // required for legacy HTTP API
    // fbdo.fcm.begin(FIREBASE_FCM_SERVER_KEY);
    // fbdo.fcm.setPriority("high");
    // fbdo.fcm.setTimeToLive(1000);
    // You can use TCP KeepAlive For more reliable stream operation and tracking the server connection status, please read this for detail.
    // https://github.com/mobizt/Firebase-ESP32#enable-tcp-keepalive-for-reliable-http-streaming
    fbdo.keepAlive(5, 5, 1);
 
 
    // // und nun das Zeug für den Firebase Stream um benachrichtigt zu werden wenn Daten von der Webanwendung geaendert wurden
    // if (!Firebase.beginStream(stream, streamPath))
    //   Serial.printf("Stream begin error, %s\n\n", stream.errorReason().c_str());

    // Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);

    /** Timeout options, below is default config.
    //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
    config.timeout.wifiReconnect = 10 * 1000;
    //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
    config.timeout.socketConnection = 30 * 1000;
    //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
    config.timeout.sslHandshake = 2 * 60 * 1000;
    //Server response read timeout in ms (1 sec - 1 min).
    config.timeout.serverResponse = 10 * 1000;
    //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
    config.timeout.rtdbKeepAlive = 45 * 1000;
    //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
    config.timeout.rtdbStreamReconnect = 1 * 1000;
    //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
    //will return false (error) when it called repeatedly in loop.
    config.timeout.rtdbStreamError = 3 * 1000;
    */
}

void CFirebaseManager::loop()
{
  if ((configManager.data.FirebasePassword =="") || (configManager.data.FirebasePassword[0] == 0))
  {
    return;
  }
  else
  {
    if (m_bNoInit)
      begin();
  }

  if (configManager.data.ConnectToCloud)
        m_bFirebaseTokenReady = Firebase.ready();
  else
        m_bFirebaseTokenReady = false;


  if (configManager.data.ConnectToCloud)
  {
    m_httpConnected = fbdo.httpConnected();
    if ((!m_httpConnected))//&& (m_bConnectState))
    {
          // Serial.println("fbdo httpConnection verloren");
          // DiagManager.PushDiagData("fbdo httpConnection verloren");
    }
  }
  else
    m_httpConnected = false;

  if ((m_bConnectState) && (!configManager.data.ConnectToCloud))
   {
          Firebase.reset(&fbconfig);
          // Firebase.end(fbdo);    
          // Firebase.end(stream);    
          fbdo.clear();
          stream.clear();    
          Serial.println("Firebase ausgeloggt");
          DiagManager.PushDiagData("Firebase ausgeloggt");
          m_bConnectState = false;
   }

  if ((!m_bConnectState) && (configManager.data.ConnectToCloud))
   {
          firebaseSignIn();
          Serial.println("Firebase eingeloggt");
          DiagManager.PushDiagData("Firebase eingeloggt");
          m_bConnectState  = true;    
   }


//***********************************************************************
// Firebase Zeug
//***********************************************************************
// wenn wir einen Fehler haben dann loggen wir aus und danach wieder ein
if (bGlobalFirebaseError)
{
    Serial.println("globaler Firebasefehler -> Logout");
    DiagManager.PushDiagData("globaler Firebase Fehler -> Ausloggen");
    m_bStreambeendenWegenFehler = false;
    if ((configManager.data.UseStream) && (m_bStreamIsRunning))
      BeendeStream();
    bGlobalFirebaseError = false;
    Firebase.reset(&fbconfig);    
    // Firebase.refreshToken(&fbconfig);
    // Firebase.end(fbdo);    
    // Firebase.end(stream);    
    fbdo.clear();
    stream.clear();    
    firebaseSignIn();
    Serial.println("Firebase wieder eingeloggt");
    DiagManager.PushDiagData("Firebase eingeloggt");
    if (configManager.data.UseStream)
    {
        StarteStream();
    }
 }

  if ((configManager.data.ConnectToCloud) && (Firebase.isTokenExpired()))
  {
    Firebase.refreshToken(&fbconfig);
    Serial.println("Refresh token");
    DiagManager.PushDiagData("Firebase Refresh token");
    if ((configManager.data.ConnectToCloud) && !Firebase.ready())
    {
          delay(500);
          // Firebase.reset(&fbconfig);    
          // bGlobalFirebaseError = true;
          Serial.println("Firebase Refresh failed -> 500ms warten");
          DiagManager.PushDiagData("Firebase Refresh failed ->  500ms warten");
    }
  }
  

  // // wenn die Pumpe eingeschaltet ist dann schreiben wir den Pumpenzustand und wir loggen einen Eintrag ins Lauzeitprotokoll im festgelegten Zeitintervall
  // if ((configManager.data.ConnectToCloud) && Firebase.ready() && 
  //     (dash.data.Pumpenzustand) &&
  //     (millis() - sendDataPrevMillis > configManager.data.FirebaseUpdateIntervall  || sendDataPrevMillis == 0)
  //    ){
  //        sendDataPrevMillis = millis();
  //       //  Serial.println("Refresh WriteFirebasePumpenzustand Begin");
  //        WriteFirebasePumpenzustand(true,true);
  //       //  Serial.println("Refresh WriteFirebasePumpenzustand Ende");
  //    }
  

  // dies hier ist nur zum Test um was abzufragen  
  // if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.QueryTest)) {
  //       QueryTageswerteDiag();
  //   }

  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.MessageTest)) {
        SendTopicTestMessage();
        dash.data.MessageTest = false;
    }


   
  if ((configManager.data.ConnectToCloud) && (Firebase.ready()))
  {
    if (configManager.data.UseStream)
    {
      if ((!m_bStreamIsRunning) && (!m_bStreambeendenWegenFehler))
        StarteStream();
      if ((m_bStreamIsRunning) && (m_bStreambeendenWegenFehler))
      {
        m_bStreambeendenWegenFehler = false;
        BeendeStream();
      }
      // if (!stream.httpConnected())
      // {
      //   Serial.println("Firebase Stream httpConnection lost");
      // }
    }
    else
    {
      if (m_bStreamIsRunning)
      {
        m_bStreambeendenWegenFehler = false;
        BeendeStream();
      }
    }
  }
}

void fcsUploadCallback(FCS_UploadStatusInfo info)
{
    if (info.status == fb_esp_fcs_upload_status_init){
        Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    }
    else if (info.status == fb_esp_fcs_upload_status_complete)
    {
        Serial.println("Upload completed\n");
        FileMetaInfo meta = fbdo.metaData();
        Serial.printf("Name: %s\n", meta.name.c_str());
        Serial.printf("Bucket: %s\n", meta.bucket.c_str());
        Serial.printf("contentType: %s\n", meta.contentType.c_str());
        Serial.printf("Size: %d\n", meta.size);
        Serial.printf("Generation: %lu\n", meta.generation);
        Serial.printf("Metageneration: %lu\n", meta.metageneration);
        Serial.printf("ETag: %s\n", meta.etag.c_str());
        Serial.printf("CRC32: %s\n", meta.crc32.c_str());
        Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
        Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_error){
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}