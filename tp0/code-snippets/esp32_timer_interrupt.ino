#include <Arduino.h>

// Deux timers matériels
hw_timer_t * timerPing;
hw_timer_t * timerPong;

// Fonction appelée chaque seconde
void IRAM_ATTR ping() {
    Serial.println("ping");
}

// Fonction appelée toutes les 2 secondes
void IRAM_ATTR pong() {
    Serial.println("pong");
}

void setup() {
    Serial.begin(115200);

    // Timer à 1 MHz
    // 1 tick = 1 microseconde
    timerPing = timerBegin(1000000);

    // Appelle ping() lorsque le timer arrive à 1 seconde
    timerAttachInterrupt(timerPing, & ping);

    // timerAlarm(timer, alarm_value, autoreload, reload_count)
    // - timer: le timer matériel à configurer
    // - alarm_value: nombre de ticks avant l'interruption
    //   ici 1 000 000 ticks = 1 seconde à 1 MHz
    // - autoreload: true pour relancer automatiquement le timer après chaque alarme
    // - reload_count: limite le nombre de relances (0 = infini, boucle continue)
    timerAlarm(timerPing, 1000000, true, 0);

    // Timer à 500 kHz
    // 1 tick = 2 microsecondes
    timerPong = timerBegin(500000);

    // Appelle pong() lorsque le timer arrive à 2 secondes
    timerAttachInterrupt(timerPong, & pong);
    // 500 000 ticks × 2 µs = 1 000 000 µs = 1 seconde
    // 1 000 000 ticks = 2 secondes
    timerAlarm(timerPong, 1000000, true, 0);
}

void loop() {
    // Le programme principal continue de tourner
    Serial.println(".");
    delay(100);
}