#include <Arduino.h>

#ifdef ESP32
    #include "LITTLEFS.h"   // https://github.com/lorol/LITTLEFS/tree/master/examples/LITTLEFS_PlatformIO
    #define LittleFS LITTLEFS
#elif defined(ESP8266)
    #include "LittleFS.h"
    #include <TZ.h>
#endif

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "configManager.h"
#include "timeSync.h"

#include "dashboard.h"
#include "DiagManager.h"
#include "DisplayManager.h"
#include "BoardsInformation.h"
#include "OTAManager.h"
#include <DHT.h>
#include "freertos/task.h"
#include "freertos/semphr.h"


#include <FirebaseESP32.h>
#include "FirebaseManager.h"
#define D13 13
#define A0 36


#define DHT_SENSOR_PIN  4 // ESP32 pin GPIO4 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
float m_Temperatur;
float m_Feuchtigkeit;
xSemaphoreHandle xMutexTemp;


char buffer [80];

float f1 = 3.145268;
int i1 = 123;
String s1("ABC die Katze");
String strTime("No Time");
String strBootTime("");

bool m_PumpeIsRunning = false;
int m_PumpenStromInput = 0;
uint32_t m_FreeHeap =0;
bool b1 = false;
bool bShownRuntimemonitor = false;
struct tm * timeinfo = NULL;
int m_Startzeit = 0;
int m_DashStartzeit = 0;
int m_GesamtEinschaltzeit = 0;
String strLastRun("");
int m_aktuelleTagesLaufzeit = 0;
int m_aktuelleTagesLaufanzahl = 0;
int m_CurrentIntervall = 0;
long m_historicLaufzeit[cihistoricdatalength];
long m_historicLaufAnzahl[cihistoricdatalength];
String strStartTime("");
bool bCloudConnected  = false;
time_t now = NULL;
float m_MinTemperatur;
float m_MaxTemperatur;
float m_AktLogTemperatur;


struct task
{    
    unsigned long rate;
    unsigned long previous;
};

task taskA = { .rate = 500, .previous = 0 };
task taskCloud = { .rate = 1000, .previous = 0 };

void saveCallback() {
    Serial.println("EEPROM saved"); 
    DiagManager.PushDiagData("Konfiguration in EEPROM gesichert");
}

void get_DHT22Data(void *params)
{
  while (true)
  {

      // read humidity
    float humi  = dht_sensor.readHumidity();
      // read temperature in Celsius
    float tempC = dht_sensor.readTemperature();

    if (xSemaphoreTake(xMutexTemp, portMAX_DELAY))
    {
        if (dash.data.Testschalter)
        {
            if (isnan(m_Temperatur))
                m_Temperatur = 20;
            if (isnan(tempC))
                tempC = m_Temperatur;
            tempC = tempC + 0.7;
            if (tempC > 24)
            tempC = 20;
        }
        m_Temperatur = tempC;
        m_Feuchtigkeit = humi;
      xSemaphoreGive(xMutexTemp);
    }
    // 4sec warten (min 2sec notwendig)
    vTaskDelay(4000 / portTICK_PERIOD_MS);
  }
}



void setup() 
{
    Serial.begin(115200);
    pinMode(D13,OUTPUT);
    BoardInformation.PrintBoardInformation();
    DisplayManager.begin();

    LittleFS.begin();
    updater.begin();
    configManager.begin();
    configManager.setConfigSaveCallback(saveCallback);
    WiFiManager.begin(configManager.data.projectName);
    GUI.begin();

    DisplayManager.begin();
    DiagManager.begin(20,10);

    Serial.println("Hello world, setup");

    if (!WiFiManager.isCaptivePortal())
    {
        // Configure device time using NTP server, with Amsterdam timezone
        #ifdef ESP32
            // ESP32 sntp APIs expect posix based time zone expressions, see https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
            // Zeitzone Berlin
            timeSync.begin(PSTR("CET-1CEST,M3.5.0,M10.5.0/3"));
            //Wait for connection
            timeSync.waitForSyncResult(5000);
        #elif defined(ESP8266)
            timeSync.begin(TZ_Europe_Berlin);
        #endif


        #ifdef ESP32
            // use asctime to serialize local time to string
            now = time(nullptr);
            struct tm * timeinfo;
            timeinfo = localtime(&now);  
            char buffer [80];
            strftime (buffer,80,"%H:%M:%S %d.%m.%y:",timeinfo);
            strTime = String(buffer);
            Serial.print(PSTR("Current time in Berlin: "));
            Serial.println(strTime);
        
        #elif defined(ESP8266)
            //Wait for connection
            timeSync.waitForSyncResult(10000);

            if (timeSync.isSynced())
            {
                time_t now = time(nullptr);
                Serial.print(PSTR("Current time in Amsterdam: "));
                Serial.print(asctime(localtime(&now)));
            }
            else 
            {
                Serial.println("Timeout while receiving the time");
            }

        #endif
        FirebaseManager.begin();
    }
    dash.begin(500);
    OTAManager.begin();
    //ueberwachung immer einschalten nach Neustart -> nein wollen wir erstmal nicht
    dash.data.RuntimeMonitor = configManager.data.ZeitCheckInit;

    DiagManager.AddVariableToMonitor(0,String("Letzter Systemstart"),&strBootTime);
    DiagManager.AddVariableToMonitor(1,String("Temperatur"),&m_Temperatur);
    DiagManager.AddVariableToMonitor(2,String("Feuchtigkeit"),&m_Feuchtigkeit);
    DiagManager.AddVariableToMonitor(3,String("Analogwert"),&m_PumpenStromInput);
    DiagManager.AddVariableToMonitor(4,String("Sytemzeit"),&strTime);
    DiagManager.AddVariableToMonitor(5,String("aktuelle GesamtEinschaltzeit"),&m_GesamtEinschaltzeit);
    DiagManager.AddVariableToMonitor(6,String("PumpeIsRunning"),&m_PumpeIsRunning);
    DiagManager.AddVariableToMonitor(7,String("FirebaseTokenReady"),&FirebaseManager.m_bFirebaseTokenReady);
    DiagManager.AddVariableToMonitor(8,String("fbdo.httpConnected"),&FirebaseManager.m_httpConnected);
    DiagManager.AddVariableToMonitor(9,String("Free Heap"),&m_FreeHeap);
    // DiagManager.AddVariableToMonitor(8,String("ComboMode"),&dash.data.mode);
    // DiagManager.AddVariableToMonitor(9,String("CfgTestCombo"),&configManager.data.TestCombo);
    DisplayManager.Ausgabe(true);

    //Temperatursensor lesen
    m_MinTemperatur = 9999;
    m_MaxTemperatur = -9999;
    m_AktLogTemperatur = -9999;

    dht_sensor.begin(); // initialize the DHT sensor
    // Analog Lesetask und Mutex starten
    xMutexTemp = xSemaphoreCreateMutex();
    xTaskCreate(&get_DHT22Data, "Current1_task", 2048, NULL, 2, NULL);

}

// hier wird das Intervall eingegeben das im Dashboardbarchart geloggt werden soll 
// normalerweise sind das die Tage zum Test kann aber auch min eingestellt werden
// der Wert der geloggt wird, sind immer Sekunden
long GetIntervallValue()
{
    if (timeinfo != NULL)
    {
        // return timeinfo->tm_min;
        return timeinfo->tm_yday;
    }
    else
        return 0;
}
 int i = 0;

void loop() 
{
    // Serial.print("Hello world, loop ");
    // Serial.println(i++);
    // delay(1000);
    m_FreeHeap = ESP.getFreeHeap();
    // if (timeSync.isSynced())

    struct tm timeinfo2;
    if(getLocalTime(&timeinfo2))
    {
        now = time(nullptr);
        //strTime = String(asctime(localtime(&now)));
        timeinfo = localtime(&now);  
        if (m_CurrentIntervall == 0)
            m_CurrentIntervall = GetIntervallValue();
        char buffer [80];
        //strftime (buffer,80,"%d.%m.%y %H:%M:%S",timeinfo);
        strftime (buffer,80,"%H:%M:%S",timeinfo);
        strTime = String(buffer);
        if (strBootTime == "")
        {
            strftime (buffer,80,"%d.%m.%y %H:%M:%S",timeinfo);
            strBootTime = String(buffer);
            Serial.println("BootTime:"+strBootTime);
        }
    }
    else 
    {
        strTime = String("No Time");
    }

    //software interrupts
    DisplayManager.Ausgabe(false);
    WiFiManager.loop();
    updater.loop();
    dash.loop();
    configManager.loop();
    FirebaseManager.loop();
    if (OTAManager.m_bDoUpdate)
    {
        OTAManager.DoOTAUpdate(configManager.data.FirmwareURL);
    }

        //your code here
    //your code here
    //task A
    if (taskA.previous == 0 || (millis() - taskA.previous > taskA.rate))
    {
        taskA.previous = millis();
        if (dash.data.Pumpenzustand)
        {
            if (b1== false)
            {
                DiagManager.PushDiagData("Pumpe ein !");
                b1 = true;
                m_Startzeit = millis() / 1000;
                m_DashStartzeit = m_Startzeit;
                strftime (buffer,80,"%H:%M:%S",timeinfo);
                strStartTime = String(buffer);
                DisplayManager.Ausgabe(true);
                FirebaseManager.WriteFirebasePumpenzustand(true);
            }
   	        dash.data.aktuelleLaufzeit = millis() / 1000 - m_DashStartzeit;
            m_GesamtEinschaltzeit = millis() / 1000 - m_Startzeit;
        }
        else
        {
            if (b1 == true)
            {
                // DiagManager.PushDiagData("Pumpe aus erkannt");
                b1 = false;
                // Pumpe wurde abgeschaltet -> alles was kleiner 1 sek war wird auf 1 Sek aufgerundet
                if (dash.data.aktuelleLaufzeit < 1)
                    dash.data.aktuelleLaufzeit = 1;
                if (m_GesamtEinschaltzeit < 1)
                    m_GesamtEinschaltzeit = 1;
                m_aktuelleTagesLaufzeit  = m_aktuelleTagesLaufzeit + dash.data.aktuelleLaufzeit;
                m_aktuelleTagesLaufanzahl++;
    	        dash.data.aktuelleLaufzeit = 0;
                FirebaseManager.WriteFirebasePumpenzustand(true);
                DiagManager.PushDiagData("Pumpe aus, letzte Laufzeit: %d Sek, Gesamt:%d Sek, heutige Zyklen:%d",m_GesamtEinschaltzeit,m_aktuelleTagesLaufzeit,m_aktuelleTagesLaufanzahl);
                strLastRun = String(m_GesamtEinschaltzeit) + String("Sek");
                DisplayManager.Ausgabe(true);
            }
            strStartTime ="";
            m_GesamtEinschaltzeit = 0;
        }

        // haben wir was zum protokollieren weil das Aufzeichnungsintervall gewechselt hat
        if ((m_CurrentIntervall != 0) &&
            (m_CurrentIntervall != GetIntervallValue())
           )
        {
            m_CurrentIntervall = GetIntervallValue();
            // das Intervall hat gewechselt -> Temperatorlogger initialisieren damit Tageswerte neu kommen
            m_MinTemperatur = 9999;
            m_MaxTemperatur = -9999;
            m_AktLogTemperatur = -9999;
            if ((m_aktuelleTagesLaufzeit > 0) || (dash.data.aktuelleLaufzeit > 0))
            {
                // das Intervall hat gewechselt und Pumpe war an
                // wir muessen die aktuellen Werte ins Array pumpen
                DiagManager.PushDiagData(String("Intervallwechsel und Pumpe war eingeschaltet bisherige Laufzeit:")+String(m_aktuelleTagesLaufzeit) + String(" akt Laufzeit:") + String(dash.data.aktuelleLaufzeit));
                for (int i = cihistoricdatalength-1; i>0;i--)
                {
                    for (int k = 0; k<7;k++)
                        dash.historicdata[i].strTimestamp[k] = dash.historicdata[i-1].strTimestamp[k];
                    dash.historicdata[i].uiValue = dash.historicdata[i-1].uiValue;
                    m_historicLaufzeit[i] = m_historicLaufzeit[i-1];
                    m_historicLaufAnzahl[i] = m_historicLaufAnzahl[i-1];

                }
                m_historicLaufzeit[0] = m_aktuelleTagesLaufzeit;
                m_historicLaufAnzahl[0] = m_aktuelleTagesLaufanzahl;

                m_aktuelleTagesLaufzeit = 0;
                m_aktuelleTagesLaufanzahl = 0;
                m_DashStartzeit = millis() / 1000;
            }
            else
            {
                DiagManager.PushDiagData("Intervallwechsel und Pumpe war immer abgeschaltet");
                String tmpstr;
                tmpstr = "Aktuell";
                tmpstr.toCharArray(dash.historicdata[0].strTimestamp,7);
                dash.historicdata[0].uiValue = 0;
                m_historicLaufzeit[0] = 0;
                m_historicLaufAnzahl[0] = 0;
                m_aktuelleTagesLaufzeit = 0;
                m_aktuelleTagesLaufanzahl = 0;
            }

        }
        if (dash.data.aktuelleLaufzeit > 0)
        {
            // wir haben ein aktuell laufendes Intervall das wir auf dem Index 0 darstellen
            String tmpstr;
            // tmpstr = String(timeinfo->tm_hour) + ":" + String(timeinfo->tm_min);
            char buffer [80];
            strftime (buffer,80,"%d.%m",timeinfo);
            tmpstr = String(buffer);
            tmpstr.toCharArray(dash.historicdata[0].strTimestamp,7);
            m_historicLaufzeit[0] = m_aktuelleTagesLaufzeit + dash.data.aktuelleLaufzeit;
            m_historicLaufAnzahl[0] = m_aktuelleTagesLaufanzahl + 1;
        }

        for (int i = 0; i<cihistoricdatalength;i++)
        {
            if (dash.data.AnzeigeinMin)
                dash.historicdata[i].uiValue = m_historicLaufzeit[i];
            else
                dash.historicdata[i].uiValue = m_historicLaufAnzahl[i];
        }

        // Anzeige ob die Ueberwachung eingeschaltet ist oder nicht
        if (dash.data.RuntimeMonitor != bShownRuntimemonitor) 
        {
            if (dash.data.RuntimeMonitor == false)
            {
                if (dash.data.PumpenAbschaltError)
                    DiagManager.PushDiagData("Pumpe manuell wieder freigegeben, Zwangsabschaltung aufgehoben");

                dash.data.PumpenAbschaltError = false;
            }
            DisplayManager.Ausgabe(true);
            bShownRuntimemonitor = dash.data.RuntimeMonitor;
        }

        // Analogeingang
        m_PumpenStromInput = analogRead(A0);
        if (m_PumpenStromInput > configManager.data.PumpeStatusEinLevel)
            m_PumpeIsRunning = true;
        else
            m_PumpeIsRunning = false;

        dash.data.Pumpenzustand= m_PumpeIsRunning || dash.data.Testschalter;
        if ((dash.data.Pumpenzustand) && 
            (dash.data.RuntimeMonitor) &&
            (dash.data.PumpenAbschaltError == false) &&
            (m_GesamtEinschaltzeit > configManager.data.MaxOnTime)
           )
           {
                dash.data.PumpenAbschaltError = true;
                DiagManager.PushDiagData("max. Laufzeit überschritten. Pumpe abgeschaltet");
                dash.data.Pumpenzustand = 0;
                DisplayManager.Ausgabe(true);
           }
        digitalWrite(D13,dash.data.PumpenAbschaltError);

       //Temperatur auslesen
       if (xSemaphoreTake(xMutexTemp, portMAX_DELAY))
       {
            dash.data.Temperatur = m_Temperatur; 
            xSemaphoreGive(xMutexTemp);
        }
        if (m_MinTemperatur > dash.data.Temperatur)
            m_MinTemperatur = dash.data.Temperatur;
        if (m_MaxTemperatur < dash.data.Temperatur)
            m_MaxTemperatur = dash.data.Temperatur;
        // hat die sich so weit geaendert das wir das protokollieren wollen
        if (fabs(m_AktLogTemperatur-dash.data.Temperatur) >= 0.5)
        {
            FirebaseManager.m_bTempLogging = true;
            m_AktLogTemperatur=dash.data.Temperatur;
        }
    }
}

  