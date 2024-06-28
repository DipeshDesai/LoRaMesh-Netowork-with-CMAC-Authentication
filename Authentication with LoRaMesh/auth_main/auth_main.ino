#include "init.h"
#include "LoRaMesh.h"
#include "SetNodeId.h"
#include "sen.h"
#include "rec.h"

bool isSender = true;
bool verifiedNodes[N_NODES] = {false}; // Array to keep track of verified nodes
unsigned long chk = 0;
const unsigned long inter = 60000; //check new nodes

void setup()
{
  init_SetNodeId();
  init_LoRaMesh();
}

void loop()
{
  if (isSender) {
    // Run the sender loop and check verification status
    bool anyVerified = sen_loop(verifiedNodes);

    // Check if all nodes have been verified
    bool allVerified = true;
    for (uint8_t i = 1; i <= N_NODES; i++) {
      if (i != nodeId && !verifiedNodes[i - 1]) {
        allVerified = false;
        break;
      }
    }

    // If all nodes are verified, switch to receiver mode
    if (allVerified) {
      isSender = false;
      Serial.println("All nodes verified. Starting LoRaMesh...");
    }
  }
  else {
    unsigned long curr = millis();
    if (curr - chk >= inter) {
      // Temporarily switch to verification mode
      Serial.println("Checking for new nodes...");
      bool newVerified = rec_loop();

      if(newVerified) {
        Serial.println("New node verified.");
      }

      chk = curr; // Reset the timer
    }
    LoRaMesh_loop();
  } 
}