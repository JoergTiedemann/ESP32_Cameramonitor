/** This is an example of "CustomFirebaseFS.h" **/
#pragma once

#ifndef CustomFirebaseFS_H
#define CustomFirebaseFS_H


#if defined(ESP32)
#include <SPIFFS.h>
#endif
#undef DEFAULT_FLASH_FS // remove Flash FS defined macro
#define DEFAULT_FLASH_FS SPIFFS

// Use SD_MMC instead of SD
#include <SD_MMC.h>
#undef DEFAULT_SD_FS // remove SD defined macro
#undef CARD_TYPE_SD // remove SD defined macro
#define DEFAULT_SD_FS SD_MMC
#define CARD_TYPE_SD_MMC 1

#endif
