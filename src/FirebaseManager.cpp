
#include <WiFi.h>
#include <FirebaseESP32.h>

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
void streamCallback(StreamData data)
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
    dash.data.RuntimeMonitor = result.boolValue;
  }
  json->clear();
}

FirebaseAuth fbauth;
FirebaseConfig fbconfig;
bool bGlobalFirebaseError = false;

extern time_t now;
extern int m_aktuelleTagesLaufzeit;
extern int m_aktuelleTagesLaufanzahl;
extern int m_GesamtEinschaltzeit;
extern float m_MinTemperatur;
extern float m_MaxTemperatur;

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

  if (Firebase.getJSON(fbdo, sensorPath, query))
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

/*****************************************************************************
 * Anzeige der Tageswerte zu Diagnosezwecken
*****************************************************************************/

void CFirebaseManager::QueryTageswerteDiag()
{
    time_t current_ts = time(nullptr);
    QueryFilter query;
    query.orderBy("LoggingTimestamp");
    query.startAt(0);
    query.endAt( current_ts);
    // get only last 8 results
    query.limitToLast(4);
    // Get filtered data
    if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte", query))
    {
        FirebaseJson *json = fbdo.to<FirebaseJson *>();
        // Print all object data
        size_t len = json->iteratorBegin();
        FirebaseJson::IteratorValue value;
        // for (size_t i = 0; i < len; i++)
        // {
        //     value = json->valueAt(i);
        //     Serial_Printf((const char *)FPSTR("Index:%d, Type: %s, Name: %s, Value: %s\n"), i, value.type == FirebaseJson::JSON_OBJECT ? (const char *)FPSTR("object") : (const char *)FPSTR("array"), value.key.c_str(), value.value.c_str());
        // }
        // json->iteratorEnd();
        len = json->iteratorBegin();
        for (size_t i = 0; i < len/4; i++)
        {
            value = json->valueAt(i*4);
            Serial.printf("index:%d Json Datensatz Name:%s  Value:%s\n",i*4,value.key.c_str(),value.value.c_str());
            FirebaseJson jsonsub;
            FirebaseJsonData result;
            jsonsub.setJsonData(value.value.c_str());
            jsonsub.get(result,"LoggingTagesLaufzeit");
             if (result.typeNum == FirebaseJson::JSON_INT )
              Serial.println("Tageslaufzeit:"+String(result.intValue));

            jsonsub.get(result,"LoggingTimestamp");
             if (result.typeNum == FirebaseJson::JSON_INT)
             {
               Serial.println("Timestamp:"+String(result.intValue));
               time_t logtime = result.intValue;
               struct tm * timeinfo;
               timeinfo = localtime(&logtime);  
               char buffer [80];
               strftime (buffer,80,"%H:%M:%S %d.%m.%y:",timeinfo);
               Serial.println("LogZeit:"+String(buffer));
             }
            jsonsub.get(result,"Tageszyklen");
             if (result.typeNum == FirebaseJson::JSON_INT)
              Serial.println("Zyklen:"+String(result.intValue));

            // Serial.println("Printed JSONDatatype:"+result.type);
            // Serial_Printf((const char *)FPSTR("%d, Type: %s, Name: %s, Value: %s\n"), i, value.type == FirebaseJson::JSON_OBJECT ? (const char *)FPSTR("object") : (const char *)FPSTR("array"), value.key.c_str(), value.value.c_str());
        }
        json->iteratorEnd();
        json->clear();
    }
    else
    {
       Serial.printf("QueryTageswerte Error:%s\n",fbdo.errorReason().c_str());
       bGlobalFirebaseError = true;
    }

    // Clear all query parameters
    query.clear();
}


void CFirebaseManager::QueryTageswerte()
{
    // Serial.printf("Query Tageswerte  Heap:%d\n",ESP.getFreeHeap());
    time_t current_ts = time(nullptr);
    QueryFilter query;
    query.orderBy("LoggingTimestamp");
    query.startAt(0);
    query.endAt( current_ts);
    // get only last 8 results
    query.limitToLast(4);
    // Get filtered data
    // if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/pumpenlogging", query))
    if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte", query))
        Serial.printf("Set json... ok\n");
    else
    {
       Serial.printf("Set json... %s\n",fbdo.errorReason().c_str());
       bGlobalFirebaseError = true;
    }
    // // Serial.printf("Get json... %s\n", Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/pumpenlogging/1673887984") ? "ok" : fbdo.errorReason().c_str());

    // if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    //   printResult(fbdo); // see addons/RTDBHelper.h

    if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte",query)) 
    {
        //FirebaseJson object that returned from getJson
        FirebaseJson &json = fbdo.jsonObject();

        //temporary object to store the data from parsing
        //this will be reused over and over
        FirebaseJsonData data;

        //parse data by key or path, the result is in FirebaseJsonData object
        json.get(data, "/path/to/node");
        printf("Ergebnis direkt\n");
        printResult(fbdo); // see addons/RTDBHelper.h
        printf("Ergebnis des json direkt\n");
        String jstr;
        json.toString(jstr);
        printf("Jsonstring:%s\n",jstr.c_str()); // see addons/RTDBHelper.h

        //or parse the complex nested atrray in nested object
        //json.get(data, "/path/to/node/[0]/[3]"); //Where [0] is index 0, [3] is index 3

        //parsing ok?
        if (data.success) 
        {
            json.get(data, "/path/to/node");
            printf("Ergebnis geparsed \n");
            Serial.println(data.to<String>());
        }
    }
    else
    {
        bGlobalFirebaseError = true;
        Serial.printf("GetJson not ok:%s\n",fbdo.errorReason().c_str());
    }

    if (Firebase.getArray(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte",query)) 
    {
        Serial.println("getArray ok");
        printResult(fbdo); // see addons/RTDBHelper.h
    }
    else
    {
        bGlobalFirebaseError = true;
        Serial.printf("GetArray not ok:%s\n",fbdo.errorReason().c_str());
    }


    // Clear all query parameters
    query.clear();
    // Serial.printf("Query Tageswerte Ende Heap:%d\n",ESP.getFreeHeap());

}


void CFirebaseManager::QueryTest()
{
    Serial.printf("Query Filter Heap:%d\n",ESP.getFreeHeap());
    time_t current_ts = time(nullptr);
    QueryFilter query;
    query.orderBy("LoggingTimestamp");
    query.startAt(0);
    query.endAt( current_ts);
    // get only last 8 results
    query.limitToLast(4);
    // Get filtered data
    // if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/pumpenlogging", query))
    if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte", query))
        Serial.printf("Set json... ok\n");
    else
    {
       Serial.printf("Set json... %s\n",fbdo.errorReason().c_str());
       bGlobalFirebaseError = true;
    }
    Serial.printf("Get json... %s\n", Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenlogging/1673887984") ? "ok" : fbdo.errorReason().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
      printResult(fbdo); // see addons/RTDBHelper.h

    if (Firebase.getJSON(fbdo, "/Wasserwerk/Pumpenmonitor/Tageswerte",query)) 
    {
        //FirebaseJson object that returned from getJson
        FirebaseJson &json = fbdo.jsonObject();

        //temporary object to store the data from parsing
        //this will be reused over and over
        FirebaseJsonData data;

        //parse data by key or path, the result is in FirebaseJsonData object
        json.get(data, "/path/to/node");
        printf("Ergebnis direkt\n");
        printResult(fbdo); // see addons/RTDBHelper.h
        printf("Ergebnis des json direkt\n");
        String jstr;
        json.toString(jstr);
        printf("Jsonstring:%s\n",jstr.c_str()); // see addons/RTDBHelper.h

        //or parse the complex nested atrray in nested object
        //json.get(data, "/path/to/node/[0]/[3]"); //Where [0] is index 0, [3] is index 3

        //parsing ok?
        if (data.success) 
        {
            json.get(data, "/path/to/node");
            printf("Ergebnis geparsed \n");
            Serial.println(data.to<String>());
        }
    }
    else
    {
        bGlobalFirebaseError = true;
        Serial.printf("GetJson not ok:%s\n",fbdo.errorReason().c_str());
    }

    if (Firebase.getArray(fbdo, "/Wasserwerk/Pumpenlogging",query)) 
    {
        Serial.println("getArray ok");
        printResult(fbdo); // see addons/RTDBHelper.h
    }
    else
    {
        bGlobalFirebaseError = true;
        Serial.printf("GetArray not ok:%s\n",fbdo.errorReason().c_str());
    }


    // Clear all query parameters
    query.clear();
    Serial.printf("Query Filter Ende Heap:%d\n",ESP.getFreeHeap());

}

void CFirebaseManager::WriteFirebaseTemperatur()
{
    String aktLogPath;
    // Serial.printf("Firebase start Write Heap:%d\n",ESP.getFreeHeap());
    if ((!configManager.data.ConnectToCloud) || (!Firebase.ready()))
      return;


    // erstmal die aktuelle Temperatur ind den Status wegschreiben
    if (now == NULL)
        jsonTemp.set(timePath, String(0));
    else
        jsonTemp.set(timePath, String(now));

    jsonTemp.set(tempAkttempPath.c_str(), String(dash.data.Temperatur));
    if (!Firebase.RTDB.setJSON(&fbdo, TempParentPath.c_str(), &jsonTemp))
    {
      Serial.printf("Write Temperatur Fehler: json... %s\n",fbdo.errorReason().c_str());
      DiagManager.PushDiagData("Write Temperaturstatus Firebase Error: "+fbdo.errorReason());
      bGlobalFirebaseError = true;
    }

    // so nun die Tageswerte  in einen separaten Knoten unterhalb von Heizungsmonitor  aber auf keinen Fall in den Status
    FirebaseJson parent;
    // Tagesdataum auf 12:00:00 normieren
    struct tm * timeinfo;
    timeinfo = localtime(&now);  
    char buffer [80];
    strftime (buffer,80,"%H:%M:%S %d.%m.%y:",timeinfo);
    struct tm t;
    t.tm_year = timeinfo->tm_year;
    t.tm_mon = timeinfo->tm_mon;           // Month, 0 - jan
    t.tm_mday = timeinfo->tm_mday;          // Day of the month
    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = timeinfo->tm_isdst;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    time_t tagestime = mktime(&t);

     // und nun die Tageswerte wegschreiben
    parent.set(temploggingtimestampPath.c_str(),tagestime);
    parent.set(temploggingAktTemp.c_str(),dash.data.Temperatur);
    parent.set(temploggingMinTemp.c_str(),m_MinTemperatur);
    parent.set(temploggingMaxTemp.c_str(),m_MaxTemperatur);

    aktLogPath = "/Heizung/Heizungsmonitor" + temptageswertePath +"/"+String(tagestime);
    if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
    {
      Serial.printf("Write Temperaturtageswerte Fehler: json... %s\n",fbdo.errorReason().c_str());
      DiagManager.PushDiagData("Write Temperaturtageswerte Firebase Error: "+fbdo.errorReason());
      // wir versuchen es ein 2. Mal
      if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
      {
        Serial.printf("2. Write Temperaturtageswerte Fehler: json... %s\n",fbdo.errorReason().c_str());
        DiagManager.PushDiagData("2. Write Temperaturtageswerte Firebase Error: "+fbdo.errorReason());
        bGlobalFirebaseError = true;
      }
    }
    // Serial.printf("Firebase Endwrite Heap:%d\n",ESP.getFreeHeap());



    // // so nun die Logging Daten in einen separaten Knoten unterhalb von Heizungsmonitor aber auf keinen Fall in den Status
    parent.set(temploggingtimestampPath.c_str(),now);
    parent.set(temploggingAktTemp.c_str(),dash.data.Temperatur);
    aktLogPath = "/Heizung" + temploggingPath +"/"+String(now);
    // // String sensorPath = "/Wasserwerk/Pumpenmonitor/Pumpenstatus/pumpenlogging";
    if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
    {
      Serial.printf("Wrtite Temperaturloggging Fehler: json... %s\n",fbdo.errorReason().c_str());
      DiagManager.PushDiagData("Wrtite Temperaturloggging  Firebase Error: "+fbdo.errorReason());
      bGlobalFirebaseError = true;
    }
}



void CFirebaseManager::WriteFirebasePumpenzustand(bool bWithLogging,bool bCalcWerte)
{
    String aktLogPath;
    // Serial.printf("Firebase start Write Heap:%d\n",ESP.getFreeHeap());
    if ((!configManager.data.ConnectToCloud) || (!Firebase.ready()))
      return;

    // if (!m_httpConnected)
    // {
    //     Serial.printf("Write Pumpenstatus httpConnection Lost\n");
    //     DiagManager.PushDiagData("Write Pumpenstatus httpConnection Lost");
    //     bGlobalFirebaseError = true;
    //     return;
    // }


    // erstmal den Pumpenzustand wegschreiben
    if (now == NULL)
        json.set(timePath, String(0));
    else
        json.set(timePath, String(now));
    long aktTagesLaufzeit = m_aktuelleTagesLaufzeit;
    long aktTagesLaufAnzahl = m_aktuelleTagesLaufanzahl;
    if (bCalcWerte)
    {
      aktTagesLaufzeit = m_aktuelleTagesLaufzeit+dash.data.aktuelleLaufzeit;
      if (dash.data.aktuelleLaufzeit > 0)
        aktTagesLaufAnzahl = m_aktuelleTagesLaufanzahl+1;
    }

    json.set(pummpenabschalterrorPath.c_str(), String(dash.data.PumpenAbschaltError));
    json.set(pummpenruntimemonitorPath.c_str(), String(dash.data.RuntimeMonitor));
    json.set(pummpenzustandPath.c_str(), String(dash.data.Pumpenzustand));
    json.set(pummpenaktuellelaufzeitPath.c_str(), String(dash.data.aktuelleLaufzeit));
    json.set(pummpentageslaufzeitPath.c_str(), String(aktTagesLaufzeit));
    if (!Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json))
    {
      Serial.printf("Write Pumpenstatus Fehler: json... %s\n",fbdo.errorReason().c_str());
      DiagManager.PushDiagData("Write Pumpenstatus Firebase Error: "+fbdo.errorReason());
      bGlobalFirebaseError = true;
    }

    // so nun die Tageswerte  in einen separaten Knoten unterhalb von Pumpenmonitor aber auf keinen Fall in den Status
    FirebaseJson parent;
    // Tagesdataum auf 12:00:00 normieren
    struct tm * timeinfo;
    timeinfo = localtime(&now);  
    char buffer [80];
    strftime (buffer,80,"%H:%M:%S %d.%m.%y:",timeinfo);
    // Serial.printf(PSTR("Protokollzeit jetzt:%s Wert:%d\n"),buffer,now);
    // Serial.printf(PSTR("Jahr:%d Monat:%d Tag:%d\n"),timeinfo->tm_year,timeinfo->tm_mon,timeinfo->tm_mday);
    struct tm t;
    t.tm_year = timeinfo->tm_year;
    t.tm_mon = timeinfo->tm_mon;           // Month, 0 - jan
    t.tm_mday = timeinfo->tm_mday;          // Day of the month
    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = timeinfo->tm_isdst;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    time_t tagestime = mktime(&t);

    // dies zur Diagnose
    // struct tm * prottimeinfo;
    // prottimeinfo = localtime(&tagestime);  
    // strftime (buffer,80,"%H:%M:%S %d.%m.%y:",prottimeinfo);
    // Serial.printf(PSTR("Protokollzeit normiert:%s Wert:%d\n"),buffer,tagestime);

    // und nun die Tageswerte wegschreiben
    parent.set(pummpenloggingtimestampPath.c_str(),tagestime);
    parent.set(pummpenloggingtageslaufzeitPath.c_str(),aktTagesLaufzeit);
    parent.set(pummpenloggingtageszyklenPath.c_str(),aktTagesLaufAnzahl);
    aktLogPath = "/Wasserwerk/Pumpenmonitor" + pummpentageeswertePath +"/"+String(tagestime);
    if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
    {
      Serial.printf("Write Pumpentageswerte Fehler: json... %s\n",fbdo.errorReason().c_str());
      DiagManager.PushDiagData("Write Pumpentageswerte Firebase Error: "+fbdo.errorReason());
      // wir versuchen es ein 2. Mal
      if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
      {
        Serial.printf("2. Write Pumpentageswerte Fehler: json... %s\n",fbdo.errorReason().c_str());
        DiagManager.PushDiagData("2. Write Pumpentageswerte Firebase Error: "+fbdo.errorReason());
        bGlobalFirebaseError = true;
      }
    }
    // Serial.printf("Firebase Endwrite Heap:%d\n",ESP.getFreeHeap());

 pummpenzustandPath = "/pumpenzustand";
    pummpenabschalterrorPath = "/PumpenAbschaltError";
    pummpenruntimemonitorPath = "/RuntimeMonitor";
    pummpenaktuellelaufzeitPath = "/aktuelleLaufzeit";
    pummpentageslaufzeitPath = "/TagesLaufzeit";
    pummpenloggingtageszyklenPath = "/Tageszyklen";
    pummpenloggingPath = "/Pumpenlogging";
    pummpenloggingtageslaufzeitPath = "LoggingTagesLaufzeit";
    pummpenloggingtimestampPath = "LoggingTimestamp";
    pummpentageeswertePath = "/Tageswerte";


    // // so nun die Logging Daten in einen separaten Knoten unterhalb von Pumpenmonitor aber auf keinen Fall in den Status
    if (bWithLogging)
    {
      parent.set(pummpenloggingtimestampPath.c_str(),now);
      parent.set(pummpenloggingtageslaufzeitPath.c_str(),m_aktuelleTagesLaufzeit+dash.data.aktuelleLaufzeit);
      aktLogPath = "/Wasserwerk" + pummpenloggingPath +"/"+String(now);
      // // String sensorPath = "/Wasserwerk/Pumpenmonitor/Pumpenstatus/pumpenlogging";
      if (!Firebase.RTDB.setJSON(&fbdo, aktLogPath.c_str(), &parent))
      {
        Serial.printf("Wrtite Pumpenloggging Fehler: json... %s\n",fbdo.errorReason().c_str());
        DiagManager.PushDiagData("Wrtite Pumpenloggging  Firebase Error: "+fbdo.errorReason());
        bGlobalFirebaseError = true;
      }
    }
    // Serial.printf("Firebase Endwrite Heap:%d\n",ESP.getFreeHeap());
    //clearDatabase();
    // if (bWithQuery)
    //   QueryTest();
}

void CFirebaseManager::SendTopicTestMessage()
{
    DiagManager.PushDiagData("Testnachricht an alle senden");
    Serial.println("SendTopic Message");

    fbdo.fcm.setNotifyMessage("Meldung", "Die Wasserpumpe lief heute seit: " + String(m_aktuelleTagesLaufzeit) + " Sekunden");
    fbdo.fcm.setTopic("Pumpe");

    FirebaseJson msg;
    msg.add("myData", m_count);

    fbdo.fcm.setDataMessage(msg.raw());

    // Firebase.broadcastMessage(fbdo)
    // Firebase.sendTopic(fbdo)
    // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
        Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}

void CFirebaseManager::SendTopicAlarmMessage()
{
    Serial.println("SendTopic Alarm Message");
    DiagManager.PushDiagData("Alarmnachricht senden weil Pumpe abgeschaltet wurde");

    fbdo.fcm.setNotifyMessage("Alarmnachricht", "Die Wasserpumpe wurde notabgeschaltet. Sie läuft aktuell seid: " + String(dash.data.aktuelleLaufzeit) + " Sekunden");
    fbdo.fcm.setTopic("Pumpe");

    FirebaseJson msg;
    msg.add("myData", m_count);

    fbdo.fcm.setDataMessage(msg.raw());

    // Firebase.broadcastMessage(fbdo)
    // Firebase.sendTopic(fbdo)
    // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
        Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}


void CFirebaseManager::SendTopicWarnMessage()
{
    Serial.println("SendTopic Warn  Message");
    DiagManager.PushDiagData("Warnnachricht senden weil Pumpe lange läuft");

    fbdo.fcm.setNotifyMessage("Warnmeldung", "Die Wasserpumpe läuft lange. Sie läuft aktuell seid: " + String(m_GesamtEinschaltzeit) + " Sekunden");
    fbdo.fcm.setTopic("Pumpe");

    FirebaseJson msg;
    msg.add("myData", m_count);

    fbdo.fcm.setDataMessage(msg.raw());

    // Firebase.broadcastMessage(fbdo)
    // Firebase.sendTopic(fbdo)
    // Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Send top... %s\n", Firebase.sendTopic(fbdo) ? "ok" : fbdo.errorReason().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
        Serial.println(fbdo.fcm.getSendResult());

    Serial.println();
}

void CFirebaseManager::BeendeStream()
{
    Firebase.endStream(stream);
    Firebase.removeStreamCallback(stream);
    // Firebase.end(stream); 
    stream.clear();   
    m_bStreamIsRunning = false;
    Serial.println("Stream beendet");
    DiagManager.PushDiagData("Firebase Stream beendet");
}

void CFirebaseManager::StarteStream()
{
    // und nun das Zeug für den Firebase Stream um benachrichtigt zu werden wenn Daten von der Webanwendung geaendert wurden
    if (!Firebase.beginStream(stream, streamPath))
      Serial.printf("Stream begin error, %s\n\n", stream.errorReason().c_str());
    Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
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
    fbdo.fcm.begin(FIREBASE_FCM_SERVER_KEY);
    fbdo.fcm.setPriority("high");
    fbdo.fcm.setTimeToLive(1000);
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
  
  // wenn die Temperatur um 1 Grad gewechselt hat  dann schreiben wir die Temperaturund wir loggen einen Eintrag ins Temperaturprotokoll 
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && 
      (m_bTempLogging) &&
      (millis() - sendTempPrevMillis > configManager.data.FirebaseUpdateIntervall  || sendTempPrevMillis == 0)
     ){
        sendTempPrevMillis = millis();
        //  Serial.println("WriteTemperaturZustand");
        DiagManager.PushDiagData("Write Temperaturstatus Wert: "+String(dash.data.Temperatur));
        WriteFirebaseTemperatur();
        m_bTempLogging = false;
     }
  

  // wenn die Pumpe eingeschaltet ist dann schreiben wir den Pumpenzustand und wir loggen einen Eintrag ins Lauzeitprotokoll im festgelegten Zeitintervall
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && 
      (dash.data.Pumpenzustand) &&
      (millis() - sendDataPrevMillis > configManager.data.FirebaseUpdateIntervall  || sendDataPrevMillis == 0)
     ){
         sendDataPrevMillis = millis();
        //  Serial.println("Refresh WriteFirebasePumpenzustand Begin");
         WriteFirebasePumpenzustand(true,true);
        //  Serial.println("Refresh WriteFirebasePumpenzustand Ende");
     }
  
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && 
      (dash.data.Pumpenzustand) &&
      (dash.data.PumpenAbschaltError == false) &&
      (m_GesamtEinschaltzeit > configManager.data.WarnOnTime)
     )
     {
       if (!m_bCloundWarnmeldung)
        {
            m_bCloundWarnmeldung = true;
            SendTopicWarnMessage();
        }
     }
  if (!dash.data.Pumpenzustand)
      m_bCloundWarnmeldung = false;

  // dies hier ist nur zum Test um was abzufragen  
  // if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.QueryTest)) {
  //       QueryTageswerteDiag();
  //   }

  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.MessageTest)) {
        SendTopicTestMessage();
        dash.data.MessageTest = false;
    }


  // RuntimeMonitor ("Laufzeitueberwachung aktivieren" wurde an oder ausgeschaltet
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.RuntimeMonitor != m_CloudRuntimeMonitor)) {
        m_CloudRuntimeMonitor = dash.data.RuntimeMonitor;
          WriteFirebasePumpenzustand();
        // QueryTageswerte();
    }

  // AbschaltError hat gewechselt
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.PumpenAbschaltError != m_CloudAbschaltError)) {
        m_CloudAbschaltError = dash.data.PumpenAbschaltError;
          WriteFirebasePumpenzustand();
  }

  // Notabschaltung hat ausgeloest
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (dash.data.PumpenAbschaltError) && (!m_bNotabschaltNachrichtgesendet)) {
        SendTopicAlarmMessage();
        m_bNotabschaltNachrichtgesendet = true;
  }
  // Notabschaltung wieder freigegeben ?
  if ((configManager.data.ConnectToCloud) && Firebase.ready() && (!dash.data.PumpenAbschaltError) && (m_bNotabschaltNachrichtgesendet)) {
        m_bNotabschaltNachrichtgesendet = false;
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