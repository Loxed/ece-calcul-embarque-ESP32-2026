#include <Arduino.h>

// LED externe
#define LED 2

// Pushbutton connecté au GPIO 4
#define BUTTON 4

// Fonction d'interruption appelée automatiquement lorsqu'un front descendant
// est détecté sur le GPIO du bouton.
void buttonISR()
{
    // Avec INPUT_PULLUP, le bouton passe de HIGH à LOW lorsqu'il est pressé.
    // Change immédiatement l'état de la LED.
    digitalWrite(LED, !digitalRead(LED));
}

void setup()
{
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    // Active l'interruption lorsque le bouton passe de HIGH à LOW.
    attachInterrupt(
        digitalPinToInterrupt(BUTTON),
        buttonISR,
        FALLING
    );
}

void loop()
{
    // La boucle principale effectue également une autre tâche.
    // Elle peut être interrompue à tout moment si le bouton est pressé.
    Serial.println("Bonjour :)");
}