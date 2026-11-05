// tp1_plotter : acquisition par buffers pour le traceur serie (TP 1, partie III).
// Remplit un buffer de 1 024 echantillons a FS_HZ (timer + analogRead sur PIN_ADC), puis l'envoie en
// entier au format du traceur serie Arduino ("adc:<valeur>"), et recommence.
// Mesure aussi la duree d'acquisition et la duree d'envoi d'un buffer.

#include <Arduino.h>
#include "neuroboard.h"

#ifndef FS_HZ
#define FS_HZ 8000
#endif

static const int N = 1024;
static hw_timer_t *timer = nullptr;
static volatile bool tick = false;
static uint16_t buf[N];

void IRAM_ATTR onAlarm() {
    tick = true;
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    analogReadResolution(12);
    timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &onAlarm);
    timerAlarm(timer, (1000000 + FS_HZ / 2) / FS_HZ, true, 0);
}

void loop() {
    int64_t t0 = esp_timer_get_time();
    // TODO : remplir buf[0..N-1], un echantillon par alarme (attendre tick, le remettre a false, analogRead)
    int64_t t1 = esp_timer_get_time();
    // TODO : envoyer le buffer au traceur serie, une ligne "adc:<valeur>" par echantillon
    Serial.flush();
    int64_t t2 = esp_timer_get_time();
    Serial.printf("# acquisition %lld us, envoi %lld us, fs %d Hz\n", t1 - t0, t2 - t1, FS_HZ);
}
