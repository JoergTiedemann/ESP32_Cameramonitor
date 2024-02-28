#include "CameraManager.h"

#include "esp_camera.h"
// #include "FS.h"                // SD Card ESP32
// #include "SDfat.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include "DiagManager.h"
#include "configManager.h"
// Provide the RTDB payload printing info and other helper functions.
// #include <addons/RTDBHelper.h>
#include <Firebase_ESP_Client.h>
#include "sd_fat32_fs_wrapper.h"

#include <SdFat.h> //https://github.com/greiman/SdFat
SdFat sd_fat_fs;   //should declare as static here
// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

CCameraManager CameraManager;

#define SPI_CS_PIN 13
#define SPI_SCK_PIN 14
#define SPI_MISO_PIN 2
#define SPI_MOSI_PIN 15
#define SPI_CLOCK_IN_MHz 10

// https://github.com/greiman/SdFat
SdSpiConfig sdFatSPIConfig(SPI_CS_PIN,  USER_SPI_BEGIN | SHARED_SPI, SD_SCK_MHZ(SPI_CLOCK_IN_MHz),&SPI);

//configManager.data.SDSpeed/10

void CCameraManager::printCardType() {
  Serial.print("Card type: ");

  switch (DEFAULT_SD_FS.card()->type()) {
    case SD_CARD_TYPE_SD1:
      Serial.printf("SD1\n");
      break;

    case SD_CARD_TYPE_SD2:
      Serial.printf("SD2\n");
      break;

    case SD_CARD_TYPE_SDHC:
        Serial.printf("SDHC\n");
      break;

    default:
      Serial.printf("Unknown\n");
  }
}


// Initialize the micro SD card
void CCameraManager::InitMicroSDCard()
{
  // Start Micro SD card
  Serial.println("Starting SD Card");
  //  SD_Card_Mounting();
  Serial.print("\nMounting SD Card... ");
  if (!Firebase.sdBegin(&sdFatSPIConfig, SPI_CS_PIN, SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN)) // pointer to SdSpiConfig, SS, SCK,MISO, MOSI
  {
      Serial.println("failed\n");
      return;
  }
  else
  {
      Serial.println("success\n");
  }

  Serial.printf("Mounting erfolgreich Addr:%d\n",&DEFAULT_SD_FS);
  
  printCardType();
  uint32_t size = DEFAULT_SD_FS.card()->sectorCount();
  if (size == 0){
     Serial.printf("Can't determine card size\n");
  }
  uint32_t sizeMB = 0.000512 * size + 0.5;
  Serial.printf("card size [MB]:%d \n",sizeMB);
}



void CCameraManager::begin()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  config.grab_mode = CAMERA_GRAB_LATEST; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Init SD Card
  InitMicroSDCard();
  
  Serial.println("This will never be printed");
}

// Get the picture filename based on the current ime
String getPictureFilename(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "";
  }
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  Serial.println(timeString);
  String filename = "/picture_" + String(timeString) +".jpg";
  return filename; 
}

String CCameraManager::TakePicture()
{
    Serial.println("Foto machen");
    camera_fb_t * fb = NULL;
    // Take Picture with Camera
    fb = esp_camera_fb_get();  
    if(!fb) {
        Serial.println("Camera capture failed");
        return "";
    }

    // Path where new picture will be saved in SD Card
    String path = getPictureFilename();
    m_PictureName = path;
    m_PictureNameSimple = m_PictureName.substring(1);

    // InitMicroSDCard();
    // fs::FS &fs = SD; 
    Serial.printf("Picture file name: %s\n", path.c_str());
    

    SdFile file;
    file.open(path.c_str(), O_RDWR | O_CREAT);
    if(!file){
        Serial.println("Failed to open file in writing mode");
    } 
    else {
        Serial.printf("Start schreiben %d Byte\n",fb->len);
        file.write(fb->buf, fb->len); // payload (image), payload length
        Serial.printf("Saved file to path: %s\n", path.c_str());
        DiagManager.PushDiagData("Foto aufnehmen: %s",path.c_str());
    }
    file.close();
    esp_camera_fb_return(fb); 
    // DeInitMicroSDCard();

    // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
    // pinMode(4, OUTPUT);
    // digitalWrite(4, LOW);
    // rtc_gpio_hold_en(GPIO_NUM_4);
    
    // delay(2000);
    Serial.println("Bild aufgenommen");
    return path;
}

void CCameraManager::SendPicture(AsyncWebServerRequest *request)
{
        Serial.printf("Send Picture to Webserver:%s\n",CameraManager.m_PictureNameSimple.c_str());
        // CameraManager.InitMicroSDCard();
        // fs::FS fs = fs::FS(fs::FSImplPtr(new SdFat32FSImpl(DEFAULT_SD_FS)));
        // SdFat32 fat32sys;
        fs::FS datei = fs::FS(fs::FSImplPtr(new SdFat32FSImpl(DEFAULT_SD_FS)));
        if (datei.exists(CameraManager.m_PictureNameSimple.c_str()))
          Serial.printf("Datei %s existiert\n",CameraManager.m_PictureNameSimple.c_str());
        else  
          Serial.printf("Datei %s existiert nicht\n",CameraManager.m_PictureNameSimple.c_str());

        if (datei.exists(CameraManager.m_PictureName.c_str()))
          Serial.printf("Datei %s existiert\n",CameraManager.m_PictureName.c_str());
        else  
          Serial.printf("Datei %s existiert nicht\n",CameraManager.m_PictureName.c_str());

        if (DEFAULT_SD_FS.exists(CameraManager.m_PictureNameSimple.c_str()))
          Serial.printf("SD Datei %s existiert\n",CameraManager.m_PictureName.c_str());
        else  
          Serial.printf("SD Datei %s existiert nicht\n",CameraManager.m_PictureName.c_str());

        if (DEFAULT_SD_FS.exists(CameraManager.m_PictureName.c_str()))
          Serial.printf("SD Datei %s existiert\n",CameraManager.m_PictureNameSimple.c_str());
        else  
          Serial.printf("SD Datei %s existiert nicht\n",CameraManager.m_PictureNameSimple.c_str());


         request->send(datei,CameraManager.m_PictureName, "image/jpg", false);
        // myFile = DEFAULT_SD_FS.open(CameraManager.m_PictureName);
        // myFile = datei;
        // request->send(myFile,CameraManager.m_PictureName, "image/jpg", false);

          // myFile = SD.open("test.txt", FILE_WRITE); 
        // request->send(SD,CameraManager.m_PictureName, "image/jpg", false);
        // request->send(SdFs,CameraManager.m_PictureName, "image/jpg", false);
        // CameraManager.DeInitMicroSDCard();

}