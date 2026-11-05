// check_board : verifie flash, PSRAM et LED de la NeuroBoard-S3.
// Sortie attendue : 16 Mo de flash, 8 Mo de PSRAM, puis la LED change de couleur chaque seconde.

#include <Arduino.h>
#include "neuroboard.h"

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.printf("Puce       : %s rev %d, %d coeurs, %lu MHz\n", ESP.getChipModel(), ESP.getChipRevision(),
                  ESP.getChipCores(), (unsigned long)ESP.getCpuFreqMHz());
    Serial.printf("Flash      : %lu Mo\n", (unsigned long)(ESP.getFlashChipSize() / (1024 * 1024)));
    Serial.printf("PSRAM      : %lu Mo\n", (unsigned long)(ESP.getPsramSize() / (1024 * 1024)));
    Serial.printf("SRAM libre : %lu o\n", (unsigned long)ESP.getFreeHeap());
    Serial.printf("Core Arduino-ESP32 %d.%d.%d, ESP-IDF %s\n", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR,
                  ESP_ARDUINO_VERSION_PATCH, esp_get_idf_version());
}

void loop() {
    static uint8_t step = 0;
    const uint8_t c[3][3] = {{16, 0, 0}, {0, 16, 0}, {0, 0, 16}};
    rgbLedWrite(NB_PIN_RGB_LED, c[step][0], c[step][1], c[step][2]);
    step = (step + 1) % 3;
    delay(1000);
}
