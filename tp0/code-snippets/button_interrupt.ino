// Led interne de l'Arduino Uno
const int LED = 13;
// Pushbutton connecté au GPIO 8
const int BUTTON = 8;

// Fonction d'interruption appelée automatiquement lorsqu'un changement est détecté sur le GPIO 8.
ISR(PCINT0_vect) {
    // Avec INPUT_PULLUP, le bouton est à LOW lorsqu'il est pressé.
    if (digitalRead(BUTTON) == LOW) {
        // Change immédiatement l'état de la LED.
        digitalWrite(LED, !digitalRead(LED));
    }
}

void setup() {
    Serial.begin(9600);
    
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    // Active les interruptions de changement d'état sur le port B.
    PCICR |= (1 << PCIE0);

    // Active l'interruption pour le GPIO 8.
    PCMSK0 |= (1 << PCINT0);
}


void loop() {
    // La boucle principale effectue une autre tâche.
    // Elle peut être interrompue à tout moment si le bouton est pressé.

    Serial.println("Bonjour :)");
}