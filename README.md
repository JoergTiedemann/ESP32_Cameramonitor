# ESP32_Cameramonitor
ESP32 Projekt fuer einen Camaramonitor mit Firebasespeicher

## HW-Platform
Das Ding läuft auf einer ESP32 CAM  PLattform, d.h. alles was ESP32 DevkitV4 o.ä. (z.B. ESP32 WROOM)
ist, es hat 4MD SPI Flash speicher fürs Progamm und 320KB Ram Size von denen aber nur 160 maximal statisch allokiert werden

In der aktuell verwendeten Partion min_spiffs.csv (siehe PlatformIo.ini) werden 1,2MB für Programm und 1,2MB für OverTheAir Updates reserviert
Weiterhin stehen dann noch 190KB SPIFFS Filesystem zur Verfuegung

## Installation
Nach dem Download bzw. Clonen des Rpos im Verzeichnis lib/ESP32 IOT Framework
einen cmd Prompt holen und npm ci eingeben damit die Node Module geladen werden
im Git selber sind die Bin Datei und die ELF Datei mit dem Befehl
```
git add -f <relativer Pfad vom Projektroot auf bin und elf Datei>

```
ins git repo aufgenommen worden

## Compilieren
Nach dem Compilieren steht im Verzeichnis .pio\Build\ES32dev die Datei firmware.bin 
zur Verfügung mit die dann geflasht werden kann
Aktuell belegt dieses Image den Speicher wie folgt:
```
RAM:   [==        ]  16.0% (used 52480 bytes from 327680 bytes)
Flash: [=======   ]  71.9% (used 1412681 bytes from 1966080 bytes)