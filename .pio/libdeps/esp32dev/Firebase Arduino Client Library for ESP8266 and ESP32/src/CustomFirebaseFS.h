/** This is an example of "CustomFirebaseFS.h" **/
#ifndef CustomFirebaseFS_H
#define CustomFirebaseFS_H


#if defined(ESP32)
#include <SPIFFS.h>
#endif
#undef DEFAULT_FLASH_FS // remove Flash FS defined macro
#define DEFAULT_FLASH_FS SPIFFS

// // Use SD_MMC instead of SD
// #include <SD_MMC.h>
// #undef DEFAULT_SD_FS // remove SD defined macro
// #undef CARD_TYPE_SD // remove SD defined macro
// #define DEFAULT_SD_FS SD_MMC
// #define CARD_TYPE_SD_MMC 1

// 📍 For SdFat on ESP32 and other devices except for ESP8266
#undef DEFAULT_SD_FS // remove SD defined macro
#undef CARD_TYPE_SD // remove SD defined macro
#include <SdFat.h> //https://github.com/greiman/SdFat
extern  SdFat sd_fat_fs;   //should declare as static here
#define DEFAULT_SD_FS sd_fat_fs
#define CARD_TYPE_SD 1
#define SD_FS_FILE SdFile

#endif

