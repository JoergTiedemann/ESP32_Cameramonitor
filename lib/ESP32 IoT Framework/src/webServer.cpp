#include "webServer.h"
#include "ArduinoJson.h"
#include <Arduino.h>

// #ifdef ESP32
     #include "LITTLEFS.h"
     #define LittleFS LITTLEFS
// #elif defined(ESP8266)
//     #include "LittleFS.h"
// #endif



// Include the header file we create with webpack
#include "generated/html.h"

//Access to other classes for GUI functions
#include "WiFiManager.h"
#include "configManager.h"
#include "updater.h"
#include "dashboard.h"
#include "DiagManager.h"
#include "OTAManager.h"

extern void streamJpg(AsyncWebServerRequest *request);
extern void sendJpg(AsyncWebServerRequest *request);
extern void setCameraVar(AsyncWebServerRequest *request);
extern void getCameraStatus(AsyncWebServerRequest *request);


// #include <FS.h>                // SD Card ESP32
//#include <Firebase_ESP_Client.h>
// #include "addons\sdhelper.h"


void webServer::begin(WebserverGetPictureCallback getPictureCallback)
{
#ifdef ESP32
    // TODO: Remove enter/exit traces after ESP32 build stable.  Experienced frequent crashing
    // during initial port when TCP stack not initialized before starting webserver instance.
    Serial.println(PSTR("webServer::begin enter"));
#endif
    m_getPictureCallback = getPictureCallback;

    //to enable testing and debugging of the interface
    DefaultHeaders::Instance().addHeader(PSTR("Access-Control-Allow-Origin"), PSTR("*"));

    server.serveStatic("/download", LittleFS, "/");

    server.onNotFound(requestHandler);

    //handle uploads
    server.on(PSTR("/upload"), HTTP_POST, [](AsyncWebServerRequest *request) {}, handleFileUpload);

    bindAll();

#ifdef ESP32
    Serial.println(PSTR("Calling server.begin();  Will fail on ESP32 if tcp stack not initialized.  Ensure WiFiManager was called first."));
#endif

    server.addHandler(&ws);
    server.begin();

#ifdef ESP32
   Serial.println(PSTR("webServer::begin done."));
#endif
}

void webServer::WifiGetResult(String& JSON,const bool bScan)
{
    StaticJsonDocument<2000> jsonBuffer;
    // Serial.printf("Get Wifidata ActiveScan:%d Scan:%d\n",WiFiManager.m_bNetworkScan,bScan);

    jsonBuffer["captivePortal"] = WiFiManager.isCaptivePortal();
    jsonBuffer["ssid"] = WiFiManager.SSID();
    jsonBuffer["strength"] = String(WiFiManager.RSSI());
    Serial.printf("Get Wifi\n");
    String str,strenc;
    //nun noch die Wlanscandaten 
    JsonArray wifiInfo  = jsonBuffer.createNestedArray("wifiInfo");
    if (WiFiManager.m_bNetworkScan)
    {
            str = "Netzwerkscan läuft noch";
            wifiInfo.add(str);
    }
    else
    {
        if (bScan)
            WiFiManager.StartNetworkscan();
        if (WiFiManager.m_ScanResult > 0)
        {
            for (int i=0;i < WiFiManager.m_ScanResult;i++)
            {
                String tstr;
                tstr = WiFiManager.GetScanString(i);
                wifiInfo.add(tstr);
            }
        }
        else
        {
            if (WiFiManager.m_bNetworkScan)
            {
                str = "Netzwerkscan läuft noch";
                wifiInfo.add(str);
            }
        }
    }
    serializeJson(jsonBuffer, JSON);
}

void webServer::bindAll()
{
    //Restart the ESP
    server.on(PSTR("/api/restart"), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, PSTR("text/html"), ""); //respond first because of restart
        ESP.restart();
    });

    //update WiFi details
    server.on(PSTR("/api/wifi/set"), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, PSTR("text/html"), ""); //respond first because of wifi change
        WiFiManager.setNewWifi(request->arg("ssid"), request->arg("pass"));
    });

    //update WiFi details with static IP
    server.on(PSTR("/api/wifi/setStatic"), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, PSTR("text/html"), ""); //respond first because of wifi change
        WiFiManager.setNewWifi(request->arg("ssid"), request->arg("pass"), request->arg("ip"), request->arg("sub"), request->arg("gw"), request->arg("dns"));
    });

    //update WiFi details
    server.on(PSTR("/api/wifi/forget"), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, PSTR("text/html"), ""); //respond first because of wifi change
        WiFiManager.forget();
    });

    //get WiFi details
    server.on(PSTR("/api/wifi/get"), HTTP_GET, [](AsyncWebServerRequest *request) {
        String JSON;
        GUI.WifiGetResult(JSON,false);
        Serial.printf("Wifi Get response:%s<-Ende\n",JSON.c_str());
        request->send(200, PSTR("text/html"), JSON);
    });

    server.on(PSTR("/api/wifi/getScan"), HTTP_GET, [](AsyncWebServerRequest *request) {
        String JSON;
        GUI.WifiGetResult(JSON,true);
        Serial.printf("Wifi Getscan response:%s<-Ende\n",JSON.c_str());
        request->send(200, PSTR("text/html"), JSON);
    });

     //get diagnosedaten
    server.on(PSTR("/api/diagnosticdata/get"), HTTP_GET, [](AsyncWebServerRequest *request) {
        String JSON;
        StaticJsonDocument<6000> jsonBuffer;
        Serial.printf("Get Diagdata\n");

        JsonArray files = jsonBuffer.createNestedArray("files");
        JsonArray variablenames  = jsonBuffer.createNestedArray("variablenames");
        JsonArray variablevalues  = jsonBuffer.createNestedArray("variablevalues");

        //get diagnosticdata listing
        for (int i = 0;i < DiagManager.GetLogSize();i++)
        {
            String str;
            str = DiagManager.GetLogString(i);
            files.add(str);
        }

        //Feste Werte ins JSON buegeln
        // jsonBuffer["variabletxt1"] = String("ABC Bimbam");
        // jsonBuffer["variablevalue1"] = String("ABC");
        // jsonBuffer["variabletxt2"] = String("Testvariable");
        // jsonBuffer["variablevalue2"] = String("DEF");
        // jsonBuffer["variabletxt3"] = String("Variable 3");
        // jsonBuffer["variablevalue3"] = String("123");

        //Hier gehts weiter die Dataen muessen besetzt werden 
        // for (int i = 0;i < 4;i++)
        // {
        //     String str;
        //     str = "Variable" + String(i);
        //     variablenames.add(str);
        //     str = "Wert" + String((i+1)*10);
        //     variablevalues.add(str);
        // }

        for (int i = 0;i < DiagManager.GetVariableMonitorSize();i++)
        {
            variablenames.add(DiagManager.GetVariableMonitorName(i));
            variablevalues.add(DiagManager.GetVariableMonitorValue(i));
        }

        serializeJson(jsonBuffer, JSON);
        // Serial.printf("Diagdata:%s<-Ende\n",JSON.c_str());
        request->send(200, PSTR("text/html"), JSON);

    });

    //get firmwareurl
    server.on(PSTR("/api/firmwareurl/get"), HTTP_GET, [](AsyncWebServerRequest *request) {
        String JSON;
        StaticJsonDocument<1000> jsonBuffer;
        jsonBuffer["firmwareurl"] = String(configManager.data.FirmwareURL); 
        serializeJson(jsonBuffer, JSON);
        Serial.printf("api/firmwareurl/get:%s<-Ende\n",JSON.c_str());
        request->send(200, PSTR("text/html"), JSON);
    });
    //do Firmwareupdate via firmwareurl 
    server.on(PSTR("/api/firmwareurl/DoUpdate"), HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, PSTR("text/html"), ""); //respond first because of restart
        Serial.printf("Anforderung von FirmwareUpdate\n");
        OTAManager.m_bDoUpdate = true;
    });

    //get Picture
    server.on(PSTR("/api/getpicture"), HTTP_GET, [](AsyncWebServerRequest *request) {
        GUI.m_PictureInfo.request = request;
        GUI.m_PictureInfo.strPictureName = "StandardPic";
        GUI.m_getPictureCallback(GUI.m_PictureInfo);
    });


// void AsyncWebServerRequest::send(FS &fs, const String& path, const String& contentType, bool download, AwsTemplateProcessor callback){
//   if(fs.exists(path) || (!download && fs.exists(path+".gz"))){
//     send(beginResponse(fs, path, contentType, download, callback));
//   } else send(404);
// }


    //get file listing
    server.on(PSTR("/api/files/get"), HTTP_GET, [](AsyncWebServerRequest *request) {

        String JSON;
        StaticJsonDocument<1000> jsonBuffer;
        JsonArray files = jsonBuffer.createNestedArray("files");

#ifdef ESP32
        //get file listing

        // 'Dir' and 'LittleFS.openDir' not implemented in ESP32, instead dir and 
        // files are represented as 'File'.  See docs/example:
        // https://github.com/lorol/ESPAsyncWebServer/blob/8c77d0e63f55160953fda843baa11435b05ae0bd/src/SPIFFSEditor.cpp#L187
        File dir = LittleFS.open("/");
        File entry = dir.openNextFile();
        
        // Copy enumerated filenames to temp array until serialized.  Reason... String mem refs 
        // returned by entry.name() could be reused, resulting in garbage chars returned to clients.
        StringArray strFiles;

        while (entry)
        {
            String fileName = String(entry.name());
            strFiles.add(fileName);
            files.add((strFiles.nth(strFiles.length() - 1))->c_str());
            entry = dir.openNextFile();
        }
#elif defined(ESP8266)
        //get file listing
        Dir dir = LittleFS.openDir("");
        while (dir.next())
            files.add(dir.fileName());
#endif

#ifdef ESP32
        //get used and total data

        // NOTE: esp_littlefs_info called twice, paying file block traversal each time.  Meh, that's
        // OK for small # of files.
        jsonBuffer["used"] = String(LittleFS.usedBytes()); 
        jsonBuffer["max"] = String(LittleFS.totalBytes());
#elif defined(ESP8266)
        //get used and total data
        FSInfo fs_info;
        LittleFS.info(fs_info);
        jsonBuffer["used"] = String(fs_info.usedBytes);
        jsonBuffer["max"] = String(fs_info.totalBytes);
#endif

        serializeJson(jsonBuffer, JSON);

#ifdef ESP32
        strFiles.free();
#endif

        request->send(200, PSTR("text/html"), JSON);
    });

    //remove file
    server.on(PSTR("/api/files/remove"), HTTP_POST, [](AsyncWebServerRequest *request) {

#ifdef ESP32
        LittleFS.remove("/" + request->arg("filename"));
        request->send(200, PSTR("text/html"), "");
#elif defined(ESP8266)
        LittleFS.remove("/" + request->arg("filename"));
        request->send(200, PSTR("text/html"), "");
#endif
    });

    //update from LittleFS
    server.on(PSTR("/api/update"), HTTP_POST, [](AsyncWebServerRequest *request) {        
        updater.requestStart("/" + request->arg("filename"));
        request->send(200, PSTR("text/html"), "");
    });

    //update status
    server.on(PSTR("/api/update-status"), HTTP_GET, [](AsyncWebServerRequest *request) {
        String JSON;
        StaticJsonDocument<200> jsonBuffer;

        jsonBuffer["status"] = updater.getStatus();
        serializeJson(jsonBuffer, JSON);

        request->send(200, PSTR("text/html"), JSON);
    });

    //send binary configuration data
    server.on(PSTR("/api/config/get"), HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream(PSTR("application/octet-stream"));
        response->write(reinterpret_cast<char*>(&configManager.data), sizeof(configManager.data));
        request->send(response);
    });

    //receive binary configuration data from body
    server.on(
        PSTR("/api/config/set"), HTTP_POST,
        [this](AsyncWebServerRequest *request) {},
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {},
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            
            static uint8_t buffer[sizeof(configManager.data)];
            static uint32_t bufferIndex = 0;

            for (size_t i = 0; i < len; i++)
            {
                buffer[bufferIndex] = data[i];
                bufferIndex++;
            }

            if (index + len == total)
            {
                bufferIndex = 0;
                configManager.saveRaw(buffer);
                request->send(200, PSTR("text/html"), "");
            }

        });

    //receive binary configuration data from body
    server.on(
        PSTR("/api/dash/set"), HTTP_POST,
        [this](AsyncWebServerRequest *request) {},
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {},
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            // Serial.printf("Set Dashdata start:%d Laenge:%d\n",(request->arg("start")).toInt(),(request->arg("length")).toInt());
            memcpy(reinterpret_cast<uint8_t *>(&(dash.data)) + (request->arg("start")).toInt(), data, (request->arg("length")).toInt());
            request->send(200, PSTR("text/html"), "");
            // Serial.printf("inputwitch:%d \n",dash.data.inputswitch);
            // Serial.printf("inputbutton:%d \n",dash.data.inputbutton);

        });

    server.on(PSTR("/stream"), HTTP_GET, streamJpg);
    server.on(PSTR("/capture"), HTTP_GET, sendJpg);
    server.on(PSTR("/control"), HTTP_GET, setCameraVar);
    /* setzten von Parametern
    http://<ipaddr>/control?var=<parametername>&val=<wert>
    
    z.B. zum setzen der Bildgrösse auf VGA
    http://192.168.178.55/control?var=framesize&val=8
    */
    server.on(PSTR("/status"), HTTP_GET, getCameraStatus);        
}

// Callback for the html
void webServer::serveProgmem(AsyncWebServerRequest *request)
{    
    // Dump the byte array in PROGMEM with a 200 HTTP code (OK)
    AsyncWebServerResponse *response = request->beginResponse_P(200, PSTR("text/html"), html, html_len);

    // Tell the browswer the content is Gzipped
    response->addHeader(PSTR("Content-Encoding"), PSTR("gzip"));
    
    request->send(response);    
}

void webServer::handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    static File fsUploadFile;

    if (!index)
    {
        if (!filename.startsWith("/"))
            filename = "/" + filename;

        Serial.print(PSTR("Start file upload, filename="));
        Serial.println(filename);

        fsUploadFile = LittleFS.open(filename, "w");

#ifdef ESP32
        // Open write fail?
        if (!LittleFS.exists(filename))
        {
            Serial.println(PSTR("[E] LittleFS.open() failed.  Is LittleFS initialized?  Was updater.begin() called?"));
        }
#endif

    }

    for (size_t i = 0; i < len; i++)
    {
        fsUploadFile.write(data[i]);
    }

    if (final)
    {
        String JSON;
        StaticJsonDocument<100> jsonBuffer;
#ifdef ESP32
        if (!filename.startsWith("/"))
            filename = "/" + filename;

        jsonBuffer["success"] = LittleFS.exists(filename);
#elif defined(ESP8266)
        jsonBuffer["success"] = fsUploadFile.isFile();
#endif
        serializeJson(jsonBuffer, JSON);

        request->send(200, PSTR("text/html"), JSON);
        fsUploadFile.close();
    }
}

webServer GUI;