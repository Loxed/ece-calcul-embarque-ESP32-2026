// tp1_timer_arduino : alarme timer a FS_HZ avec l'API Arduino-ESP32 3.x (TP 1, E1).
// L'ISR inverse PIN_SCOPE : le signal carre observe a l'oscilloscope est a FS_HZ / 2.
// Le programme compte aussi les alarmes pendant 1 s pour verifier la frequence sans oscilloscope.

#include <Arduino.h>
#include "neuroboard.h"

#ifndef FS_HZ
#define FS_HZ 16000
#endif

hw_timer_t *timer = nullptr;
volatile uint32_t alarms = 0;
volatile bool level = false;

void IRAM_ATTR onAlarm() {
    // TODO : inverser PIN_SCOPE (variable level, digitalWrite) et incrementer alarms
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    pinMode(PIN_SCOPE, OUTPUT);

    const uint32_t resolution = 1000000;                 // 1 tick = 1 us
    const uint64_t alarm_count = 0;                      // TODO : nombre de ticks entre deux alarmes pour FS_HZ
    // TODO : timerBegin(resolution), timerAttachInterrupt(timer, &onAlarm), timerAlarm(timer, alarm_count, true, 0)

    Serial.printf("Frequence visee %d Hz, alarm_count %llu\n", FS_HZ, alarm_count);
}

void loop() {
    uint32_t a0 = alarms;
    int64_t t0 = esp_timer_get_time();
    delay(1000);
    uint32_t a1 = alarms;
    int64_t t1 = esp_timer_get_time();
    Serial.printf("alarmes mesurees : %.1f par seconde\n", (a1 - a0) * 1e6 / (double)(t1 - t0));
}
