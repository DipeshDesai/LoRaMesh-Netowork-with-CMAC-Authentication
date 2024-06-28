#include "init.h"

//AES
const uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

//LoRaMesh
uint8_t nodeId = 1;
uint8_t routes[N_NODES]; // full routing table for mesh
int16_t rssi[N_NODES]; // signal strength info

// Singleton instance of the radio driver
RH_RF95 rf95;

// Class to manage message delivery and receipt, using the driver declared above
RHMesh *manager;

// AES objects
AESTiny128 aes128;
AES_CMAC cmac(aes128);

// message buffer
char buf[RH_MESH_MAX_MESSAGE_LEN];

//rh95 message buffer
char temp[RH_RF95_MAX_MESSAGE_LEN];

int freeMem() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void init_SetNodeId()
{
  Serial.begin(9600);
  EEPROM.write(0, nodeId);
  Serial.print(F("set nodeId = "));
  Serial.println(nodeId);

  uint8_t readVal = EEPROM.read(0);

  Serial.print(F("read nodeId = "));
  Serial.println(readVal);
  
  if (nodeId != readVal) {
    Serial.println(F("*** FAIL ***"));
  } else {
    Serial.println(F("SUCCESS"));
  }
}

void init_LoRaMesh()
{
  randomSeed(analogRead(0));
  //pinMode(LED, OUTPUT);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available

  nodeId = EEPROM.read(0);
  if (nodeId > 10) {
    Serial.print(F("EEPROM nodeId invalid: "));
    Serial.println(nodeId);
    nodeId = 1;
  }
  Serial.print(F("initializing node "));

  manager = new RHMesh(rf95, nodeId);

  if (!manager->init()) {
    Serial.println(F("init failed"));
  } else {
    Serial.println("done");
  }

  if (!rf95.setFrequency(868.0)) {
    Serial.println(F("setFrequency failed"));
    while (1);
  }
  
  rf95.setTxPower(50, false);

  // long range configuration requires for on-air time
  boolean longRange = false;
  if (longRange) {
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
    };
    rf95.setModemRegisters(&modem_config);
    }
    else
    {
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128)) {
      Serial.println(F("set config failed"));
    }
  }

  Serial.println("RF95 ready");
  for(uint8_t n=1;n<=N_NODES;n++) {
    routes[n-1] = 0;
    rssi[n-1] = 0;
  }
  Serial.print(F("mem = "));
  Serial.println(freeMem());
}