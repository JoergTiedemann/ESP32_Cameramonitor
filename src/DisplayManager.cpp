#include "DisplayManager.h"
#include "WiFiManager.h"
#include "dashboard.h"
#include "TFT_eSPI.h"

extern String strTime;
extern String strStartTime;
extern String strLastRun;
extern int m_aktuelleTagesLaufzeit;
extern float m_Temperatur;

//create global object
CDisplayManager DisplayManager;

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
// Insgesamt braucht man nur 3 Signale von der CPU SLI/SDA, SCK und A0/DC -> Reste CS und RST und Vcc und GND sind konstante pegel nur CS wird bei mehreren Teilnehmern am SDI Bus benötigt
// #define TFT_CS     -1 //D1 // Standard D1 der CS Eingang des Display kann auch dauerhaft mit GND verbunden werden dann kann aber nur ein Teilnehmer am SPI Bus betrieben werden
// #define TFT_RST    -1  // Standadrd D0 you can also connect this to the Arduino reset (RST) in which case, set this #define pin to -1!
// #define TFT_DC     D2

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)

// For 1.44" and 1.8" TFT with ST7735 use
// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

//function to call in setup
void CDisplayManager::begin()
{
        // Use this initializer if you're using a 1.8" TFT
    tft.begin();   // initialize a ST7735S chip, black tab
    tft.setRotation(2);   //1: 90 Grad drehen -> dann ist das waagerecht
    tft.fillScreen(TFT_BLUE);
    tft.setCursor(0, 30);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(2);
    tft.println("Booting...");
 
}


void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void tftPrintTest() {
  tft.setTextWrap(false);
//  tft.fillScreen(ST77XX_BLACK);
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(0, 30);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(TFT_GREEN);
  delay(2000);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN);
//   tft.print(f1, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(TFT_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(TFT_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(TFT_WHITE);
  tft.print(" seconds.");
}

void CDisplayManager::ClearScreen(int BackColor)
{
    m_BkgndColor = BackColor;
    tft.fillScreen(m_BkgndColor);
    if (m_BkgndColor == TFT_BLUE)
        m_FgndColor = TFT_WHITE;
    else
        m_FgndColor = TFT_BLACK;
    tft.setTextColor(m_FgndColor,m_BkgndColor);
}


void CDisplayManager::Ausgabe(bool bInitScreen)
{
    // nun die IP Adresse anzeigen
    if (WiFiManager.isCaptivePortal())
    {
        // Captive Portal gestarten CPU ist im Wlan AP Mode
        if (bInitScreen)
            ClearScreen(TFT_YELLOW);
        tft.setTextWrap(false);
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.println("WLAN AP-Mode !");
        tft.setTextSize(2);
        tft.println(PSTR("192.168.4.1"));
    }
    else
    {
        tft.setTextWrap(false);
        tft.setCursor(0, 0);
        if (bInitScreen)
        {
            if (dash.data.PumpenAbschaltError)
                ClearScreen(TFT_RED);
            else
            {
                if (dash.data.Pumpenzustand)
                    ClearScreen(TFT_GREEN);
                else
                    ClearScreen(TFT_BLUE);
            }
        }
        tft.setTextSize(0);
        tft.println(WiFi.SSID());
        tft.setTextSize(0);
        tft.println(WiFi.localIP());
        tft.setTextSize(0);
        tft.setTextSize(2);
        tft.println(strTime);

        tft.setTextSize(2);
        if (dash.data.Pumpenzustand)
        {
                tft.println("Ein");
                tft.println("seit:");
                tft.println(strStartTime);
                tft.println("");
        }
        else
        {
            tft.println("Aus");
            tft.setTextWrap(true);
            tft.println("Heute:");
            tft.println( String(m_aktuelleTagesLaufzeit)+"Sek");
        }

        tft.setTextSize(3);
        // tft.println("");
        tft.println( String(m_Temperatur)+" C");
        tft.setTextSize(2);

        if (dash.data.RuntimeMonitor == false)
        {
            tft.setTextColor(TFT_BLACK,TFT_YELLOW);
            tft.setTextWrap(true);
            tft.println("kein");
            tft.println("Zeitcheck");
            tft.setTextColor(m_FgndColor,m_BkgndColor);
            tft.setTextWrap(false);
        }
        else
        {
            tft.println("");
            tft.println("");
        }

    }
}