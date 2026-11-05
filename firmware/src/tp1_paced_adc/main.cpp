// tp1_paced_adc : acquisition avec analogRead() cadencee par un timer (TP 1, E8 a E11).
// 1. duree moyenne d'un appel a analogRead() sur 10 000 appels ;
// 2. pour 8, 16 et 48 kHz : l'ISR leve un drapeau, loop() convertit et horodate 1 000 echantillons ;
//    affichage de la moyenne et de l'ecart type de l'intervalle, et du nombre d'alarmes manquees.

#include <Arduino.h>
#include <math.h>
#include "neuroboard.h"

static const int N = 1000;
static hw_timer_t *timer = nullptr;
static volatile uint32_t pending = 0;                    // alarmes non encore traitees
static int64_t stamps[N];
static uint16_t values[N];

void IRAM_ATTR onAlarm() {
    // TODO : l'ISR ne fait qu'une chose, signaler l'alarme a loop()
}

static void measure_analogread_duration() {
    // TODO : chronometrer 10 000 appels a analogRead(PIN_ADC) avec esp_timer_get_time(),
    //        afficher la duree moyenne d'un appel et la frequence maximale correspondante
}

static void run(uint32_t fs) {
    // TODO : regler l'alarme du timer pour fs (timerAlarm), puis pour i de 0 a N-1 :
    //        attendre une alarme, compter celles manquees, convertir (analogRead) et horodater
    //        (esp_timer_get_time) dans values[i] et stamps[i]
    uint32_t missed = 0;
    double mean = 0, var = 0;
    // TODO : moyenne et variance de l'intervalle stamps[i] - stamps[i - 1]
    Serial.printf("fs %6lu Hz : intervalle moyen %8.2f us (attendu %8.2f), ecart type %6.2f us, alarmes manquees %lu\n",
                  (unsigned long)fs, mean, 1e6 / fs, sqrt(var), (unsigned long)missed);
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    analogReadResolution(12);
    timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &onAlarm);
}

void loop() {
    Serial.println();
    measure_analogread_duration();
    for (uint32_t fs : {8000u, 16000u, 48000u}) {
        run(fs);
    }
    delay(5000);
}
