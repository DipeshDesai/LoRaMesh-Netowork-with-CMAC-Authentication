#include "init.h"
#include "LoRaMesh.h"
#include "SetNodeId.h"
#include "sen.h"
#include "rec.h"

bool isVerified = false;

void setup()
{  
  init_SetNodeId();
  init_LoRaMesh();
}  

void loop()
{
  if (!isVerified) {
    // Run the receiver loop and check verification status
    isVerified = rec_loop();

    if (isVerified) {
      Serial.println("Node verified. Starting LoRaMesh...");
    }
  } else {
    LoRaMesh_loop();
  }
}