#include <Arduino_CAN.h>

// CAN IDs für die Kommunikation
#define MCU_MESSAGE_ID_1 0x1801D0EF  // Nachricht Teil I
#define MCU_MESSAGE_ID_2 0x1802D0EF  // Nachricht Teil II

// Variablen für die extrahierten Werte
float busVoltage = 0.0;
float busCurrent = 0.0;
float phaseCurrent = 0.0;
int speedRPM = 0;
int controllerTemp = 0;
int motorTemp = 0;
uint8_t status = 0;
uint8_t error = 0;

void setup() {
  // Initialisiere serielle Schnittstelle
  Serial.begin(115200);
  while (!Serial);

  // Initialisiere CAN
  if (!CAN.begin(250E3)) {  // 250 Kbps
    Serial.println("CAN-Initialisierung fehlgeschlagen!");
    while (1);
  }

  Serial.println("CAN-Initialisierung erfolgreich. Lausche auf Nachrichten...");
}

void loop() {
  // Wenn eine Nachricht empfangen wird
  if (CAN.parsePacket()) {
    // Empfange Nachrichten ID
    uint32_t id = CAN.packetId();

    // Prüfen, ob die ID den erwarteten MCU-Nachrichten entspricht
    if (id == MCU_MESSAGE_ID_1) {
      // Nachricht Teil I: Spannung, Strom, Drehzahl
      if (CAN.available() >= 8) {
        busVoltage = CAN.read() * 0.1;                          // Byte 0
        busCurrent = (int16_t)(CAN.read() | (CAN.read() << 8)) * 0.1; // Byte 1-2
        phaseCurrent = (int16_t)(CAN.read() | (CAN.read() << 8)) * 0.1; // Byte 3-4
        speedRPM = (int16_t)(CAN.read() | (CAN.read() << 8));   // Byte 5-6

        // Ausgabe
        Serial.print("Bus Voltage: "); Serial.print(busVoltage); Serial.println(" V");
        Serial.print("Bus Current: "); Serial.print(busCurrent); Serial.println(" A");
        Serial.print("Phase Current: "); Serial.print(phaseCurrent); Serial.println(" A");
        Serial.print("Speed RPM: "); Serial.println(speedRPM);
      }
    } else if (id == MCU_MESSAGE_ID_2) {
      // Nachricht Teil II: Temperaturen, Status, Fehler
      if (CAN.available() >= 8) {
        controllerTemp = (int8_t)CAN.read() - 40;               // Byte 0
        motorTemp = (int8_t)CAN.read() - 40;                   // Byte 1
        status = CAN.read();                                   // Byte 2
        error = CAN.read();                                    // Byte 3

        // Ausgabe
        Serial.print("Controller Temp: "); Serial.print(controllerTemp); Serial.println(" °C");
        Serial.print("Motor Temp: "); Serial.print(motorTemp); Serial.println(" °C");
        Serial.print("Status: 0x"); Serial.println(status, HEX);
        Serial.print("Error: 0x"); Serial.println(error, HEX);
      }
    }
  }
}
