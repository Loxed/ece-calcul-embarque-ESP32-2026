#include <Arduino.h>

/*
 * gpio_three_levels.ino  --  TP0 / SPOT CODE 1
 *
 * Bascule la meme broche 100 000 fois selon trois niveaux d'abstraction
 * et mesure la duree de chaque boucle.
 *
 * Cible   : ESP32-S3 (NeuroBoard-S3)
 * Mesure  : brancher un oscilloscope sur PIN_TEST pour voir la difference
 *           de frequence entre les trois rafales.
 */

#include "driver/gpio.h"
#include "soc/gpio_reg.h"

#define PIN_TEST   4          // GPIO libre, 0..31 obligatoire pour la version registre
#define N_TOGGLES  100000

// --- Niveau 1 : API Arduino -------------------------------------------------
void toggleArduino() {
  for (uint32_t i = 0; i < N_TOGGLES; i++) {
    digitalWrite(PIN_TEST, HIGH);
    digitalWrite(PIN_TEST, LOW);
  }
}

// --- Niveau 2 : driver ESP-IDF ----------------------------------------------
void toggleIdf() {
  for (uint32_t i = 0; i < N_TOGGLES; i++) {
    gpio_set_level((gpio_num_t)PIN_TEST, 1);
    gpio_set_level((gpio_num_t)PIN_TEST, 0);
  }
}

// --- Niveau 3 : ecriture directe dans les registres -------------------------
// W1TS = "Write 1 To Set"   : ecrire un 1 met la broche a l'etat haut
// W1TC = "Write 1 To Clear" : ecrire un 1 met la broche a l'etat bas
// Ces registres evitent la sequence lecture-modification-ecriture, donc les
// problemes d'acces concurrent entre les deux coeurs.
void toggleRegistres() {
  const uint32_t mask = 1UL << PIN_TEST;
  for (uint32_t i = 0; i < N_TOGGLES; i++) {
    REG_WRITE(GPIO_OUT_W1TS_REG, mask);
    REG_WRITE(GPIO_OUT_W1TC_REG, mask);
  }
}

void mesure(const char *nom, void (*f)()) {
  uint32_t t0 = micros();
  f();
  uint32_t dt = micros() - t0;
  Serial.printf("%-22s : %8lu us  (%6.1f ns / bascule)\n",
                nom, (unsigned long)dt, (dt * 1000.0) / (2.0 * N_TOGGLES));
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_TEST, OUTPUT);

  // delay of 5 seconds
  delay(5000);

  Serial.println("\n=== Comparaison des trois niveaux (GPIO) ===");


  mesure("1. digitalWrite()", toggleArduino);
  mesure("2. gpio_set_level()", toggleIdf);
  mesure("3. registres", toggleRegistres);
  Serial.println("============================================");
}

void loop() {}
