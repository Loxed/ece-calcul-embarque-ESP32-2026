// LED interne de l'Arduino Uno
const int LED = 13;

// Pushbutton connecté au GPIO 8
const int BUTTON = 8;

// Mémorise l'état précédent du bouton. 
// Cela permet de détecter le moment où le bouton passe de HIGH à LOW.
bool previousButtonState = HIGH;

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
    // Lecture de l'état actuel du bouton.
    bool buttonState = digitalRead(BUTTON);

    // Avec INPUT_PULLUP :
    // HIGH -> bouton relâché
    // LOW  -> bouton pressé
    
    // On vérifie ici si le bouton vient d'être pressé.
    if (previousButtonState == HIGH && buttonState == LOW) {
        // Change l'état de la LED.
        digitalWrite(LED, !digitalRead(LED));
    }

    // Mémorise l'état actuel pour le prochain tour de boucle.
    previousButtonState = buttonState;


    // La boucle principale effectue également une autre tâche.
    digitalWrite(LED, HIGH);
    delay(1000);

    digitalWrite(LED, LOW);
    delay(1000);
}