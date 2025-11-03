#include <SPI.h>
#include "mcp_can.h"

const int SPI_CS_PIN = 10;
const int INT_PIN = 2;
MCP_CAN CAN0(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("Inicializando MCP2515 (8 MHz) a 125 kbps..."));
  if (CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println(F("MCP2515 inicializado correctamente."));
    CAN0.setMode(MCP_NORMAL);
  } else {
    Serial.println(F("Error al iniciar MCP2515."));
    while (1);
  }
}

void loop() {
  byte data[] = { 'H','e','l','l','o' };
  byte len = 5;
  if (CAN0.sendMsgBuf(0x123, 0, len, data) == CAN_OK)
    Serial.println(F("Trama enviada -> ID=0x123, Data=Hello"));
  delay(500);
}