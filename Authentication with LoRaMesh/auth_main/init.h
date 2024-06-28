#ifndef init_h
#define init_h

#include <AES.h>
#include <AES_CMAC.h>
#include <EEPROM.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>

#define RH_HAVE_SERIAL
//#define LED 13
#define N_NODES 2

//LoRaMesh
extern uint8_t nodeId;
extern uint8_t routes[N_NODES]; // full routing table for mesh
extern int16_t rssi[N_NODES]; // signal strength info

// Singleton instance of the radio driver
extern RH_RF95 rf95;

// Class to manage message delivery and receipt, using the driver declared above
extern RHMesh *manager;

// message buffer
extern char buf[RH_MESH_MAX_MESSAGE_LEN];
extern char temp[RH_RF95_MAX_MESSAGE_LEN];

//SetNodeID
extern uint8_t nodeId;

// AES Key
extern const uint8_t key[16];

//AES
extern AESTiny128 aes128;
extern AES_CMAC cmac;

void init_SetNodeId();
void init_LoRaMesh();

#endif