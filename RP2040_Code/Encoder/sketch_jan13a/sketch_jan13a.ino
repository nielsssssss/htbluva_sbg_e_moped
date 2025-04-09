int encoderPinA = 9;
int encoderPinB = 3;
int encoderPos = 0;
int encoderPinALast = LOW;
int encoderPinANow = LOW;
unsigned long lastDebounceTime = 0; // Zeit des letzten Wechsels
unsigned long debounceDelay = 2;   // Entprellzeit in Millisekunden

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int currentPinA = digitalRead(encoderPinA);

  if (currentPinA != encoderPinALast) {
    // Wenn sich der Zustand ändert, speichere die aktuelle Zeit
    if (millis() - lastDebounceTime > debounceDelay) {
      encoderPinANow = currentPinA;

      if (encoderPinANow == LOW) {
        // Zustandswechsel erkannt
        if (digitalRead(encoderPinB) == HIGH) {
          encoderPos++;
        } else {
          encoderPos--;
        }
        Serial.println(encoderPos);
      }
      lastDebounceTime = millis(); // Entprellzeit zurücksetzen
    }
  }
  encoderPinALast = currentPinA;
}
