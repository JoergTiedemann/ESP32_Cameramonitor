#ifndef SERVER_H
#define SERVER_H

#include <ESPAsyncWebServer.h>
typedef struct webserver_picture_info_t
{
    AsyncWebServerRequest *request;
    String strPictureName;
} WebserverPictureInfo;

typedef void (*WebserverGetPictureCallback)(WebserverPictureInfo);

class webServer
{

private:    
    static void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    static void serveProgmem(AsyncWebServerRequest *request);
    void bindAll();
 public:
    WebserverGetPictureCallback m_getPictureCallback = NULL;
    WebserverPictureInfo m_PictureInfo;
    void WifiGetResult(String& JSON,const bool bScan);
    AsyncWebServer server = AsyncWebServer(80);
    AsyncWebSocket ws = AsyncWebSocket("/ws");
    ArRequestHandlerFunction requestHandler = serveProgmem;
    void begin(WebserverGetPictureCallback getPictureCallback = NULL);
};

extern webServer GUI;

#endif
