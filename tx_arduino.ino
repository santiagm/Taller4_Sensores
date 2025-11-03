// Código del receptor
#include <CAN.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Receiver");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  int packetSize = CAN.parsePacket();

  if (packetSize || CAN.packetId() != -1) {
    Serial.print("Received ");
    if (CAN.packetExtended()) Serial.print("extended ");
    if (CAN.packetRtr()) Serial.print("RTR ");
    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);
      while (CAN.available()) Serial.print((char)CAN.read());
      Serial.println();
    }
    Serial.println();
  }
}