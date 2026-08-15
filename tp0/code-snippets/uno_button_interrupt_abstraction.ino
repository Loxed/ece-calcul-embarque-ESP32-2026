// LED interne de l'Arduino UNO
const int LED = 13;

// Pushbutton connecté au GPIO 2
// Sur l'Arduino UNO, D2 et D3 disposent d'une interruption externe.
const int BUTTON = 2;

// Fonction d'interruption appelée automatiquement
// lorsqu'un appui est détecté sur le bouton.
void buttonISR() {
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

    // Associe la fonction buttonISR à l'interruption
    // correspondant à la broche BUTTON.
    attachInterrupt(digitalPinToInterrupt(BUTTON), buttonISR, FALLING);
}

void loop() {
    // La boucle principale effectue une autre tâche.
    // Elle peut être interrompue à tout moment lorsque le bouton est pressé.

    Serial.println("Bonjour :)");
}