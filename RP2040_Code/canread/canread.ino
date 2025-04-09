/*
  CANRead

  Receive and read CAN Bus messages

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/can
*/

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_CAN.h>

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  if (!CAN.begin(CanBitRate::BR_250k)) {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
}

void loop() {
  if (CAN.available()) {
    CanMsg msg = CAN.read();

    // Process only messages with ID 0x980117EF (or the correct ID for bus voltage)
    if (msg.id == 0x980117EF) {
      // Process bytes 1 & 2
      uint16_t combinedValue1 = (msg.data[1] << 8) | msg.data[0];
      float voltage1 = combinedValue1 * 0.1; // Resolution is 0.1 V/bit
      Serial.print("Voltage:");
      Serial.println(voltage1);
  

      // Process bytes 3 & 4
      uint16_t combinedValue2 = (msg.data[3] << 8) | msg.data[2];
      float voltage2 = (combinedValue2 * 0.1)-3200; // Resolution is 0.1 V/bit
      Serial.print("Bus Current:");
      Serial.println(voltage2);

      // Process bytes 5 & 6
      uint16_t combinedValue3 = (msg.data[5] << 8) | msg.data[4];
      float voltage3 = (combinedValue3 * 0.1)-3200; // Resolution is 0.1 V/bit
      Serial.print("Phase current: ");
      Serial.println(voltage3);

      // Process bytes 7 & 8
      uint16_t combinedValue4 = (msg.data[7] << 8) | msg.data[6];
      float voltage4 = combinedValue4-32000; // Resolution is 0.1 V/bit
      Serial.print("Speed:");
      Serial.println(voltage4);
    }
  }
}
