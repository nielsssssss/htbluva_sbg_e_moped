#include <Arduino_CAN.h>

static uint32_t const CAN_ID1 = 0x20; // CAN ID for the first message
static uint32_t const CAN_ID2 = 0x21; // CAN ID for the second message

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  if (!CAN.begin(CanBitRate::BR_250k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
}

void loop()
{
  // Werte einlesen (analog von A0 bis A7)
  uint16_t analogValue1 = 50;
  uint16_t analogValue2 = 40;
  uint16_t analogValue3 = 40;
  uint16_t analogValue4 = 40;
  uint16_t analogValue5 = 20;
  uint16_t analogValue6 = 30;
  uint16_t analogValue7 = 50;
  uint16_t analogValue8 = 45;
  



  // Nachrichtendaten für die erste Nachricht (4 x 16-Bit-Werte)
  uint8_t msg_data1[8];
  msg_data1[0] = (analogValue1 >> 8) & 0xFF; // High byte Wert 1
  msg_data1[1] = analogValue1 & 0xFF;        // Low byte Wert 1
  msg_data1[2] = (analogValue2 >> 8) & 0xFF; // High byte Wert 2
  msg_data1[3] = analogValue2 & 0xFF;        // Low byte Wert 2
  msg_data1[4] = (analogValue3 >> 8) & 0xFF; // High byte Wert 3
  msg_data1[5] = analogValue3 & 0xFF;        // Low byte Wert 3
  msg_data1[6] = (analogValue4 >> 8) & 0xFF; // High byte Wert 4
  msg_data1[7] = analogValue4 & 0xFF;        // Low byte Wert 4

  CanMsg const msg1(CanStandardId(CAN_ID1), sizeof(msg_data1), msg_data1);

  if (int const rc = CAN.write(msg1); rc < 0)
  {
    Serial.print("CAN.write(...) failed with error code ");
    Serial.println(rc);
    for (;;) { }
  }
  delay(2);

  // Nachrichtendaten für die zweite Nachricht (4 x 16-Bit-Werte)
  uint8_t msg_data2[8];
  msg_data2[0] = (analogValue5 >> 8) & 0xFF; // High byte Wert 5
  msg_data2[1] = analogValue5 & 0xFF;        // Low byte Wert 5
  msg_data2[2] = (analogValue6 >> 8) & 0xFF; // High byte Wert 6
  msg_data2[3] = analogValue6 & 0xFF;        // Low byte Wert 6
  msg_data2[4] = (analogValue7 >> 8) & 0xFF; // High byte Wert 7
  msg_data2[5] = analogValue7 & 0xFF;        // Low byte Wert 7
  msg_data2[6] = (analogValue8 >> 8) & 0xFF; // High byte Wert 8
  msg_data2[7] = analogValue8 & 0xFF;        // Low byte Wert 8

  CanMsg const msg2(CanStandardId(CAN_ID2), sizeof(msg_data2), msg_data2);

  if (int const rc = CAN.write(msg2); rc < 0)
  {
    Serial.print("CAN.write(...) failed with error code ");
    Serial.println(rc);
    for (;;) { }
  }
  delay(2);
  /*
  Serial.print("Sent values (Msg1): ");
  Serial.print(analogValue1);
  Serial.print(", ");
  Serial.print(analogValue2);
  Serial.print(", ");
  Serial.print(analogValue3);
  Serial.print(", ");
  Serial.println(analogValue4);

  Serial.print("Sent values (Msg2): ");
  Serial.print(analogValue5);
  Serial.print(", ");
  Serial.print(analogValue6);
  Serial.print(", ");
  Serial.print(analogValue7);
  Serial.print(", ");
  Serial.println(analogValue8);
  */
}
