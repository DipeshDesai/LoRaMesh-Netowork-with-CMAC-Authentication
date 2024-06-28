#include "sen.h"

bool sen_loop(bool *verifiedNodes)
{
  // Data to be sent
  const uint8_t data[1] = {nodeId};
  //{ 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a , 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11 };
  uint8_t mac[16];

  cmac.generateMAC(mac, key, data, sizeof(data));

  // Create a payload combining the data and MAC tag
  uint8_t payload[sizeof(data) + sizeof(mac)];
  memcpy(payload, data, sizeof(data));
  memcpy(payload + sizeof(data), mac, sizeof(mac));

  Serial.println("Sending payload");
  rf95.send(payload, sizeof(payload));
  rf95.waitPacketSent();

  uint8_t temp[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(temp);

  bool flag=false; // Variable to track if any node verified successfully
  for (uint8_t n = 1; n <= N_NODES; n++) {
    if (n != nodeId) {
      Serial.print("Waiting for reply from node ");
      Serial.println(n);
      if (rf95.waitAvailableTimeout(10000)) {
        // Reply from node   
        if (rf95.recv(temp, &len)) {
          Serial.print("Got reply from node ");
          Serial.print(n);
          Serial.print(": ");
          Serial.println((char*)temp);
          // Serial.print("RSSI: ");
          // Serial.println(rf95.lastRssi(), DEC);
          // if (strcmp((char*)temp, "ok") == 0) {
          //   Serial.println("Verification successful!");
          //   verifiedNodes[n - 1] = true;
          //   flag = true;
          // }
          if (len == 2) // Ensure packet size
          {
            uint8_t SenId = temp[0];
            uint8_t RecId = temp[1];

            // Check if the payload contains the sender's ID and the current node's ID
            bool isSenId = (SenId == nodeId);
            bool isRecId = (RecId == n);
            
            // Print the result
            Serial.print("Combined message is valid: ");
            Serial.println(isSenId && isRecId ? "Yes" : "No");

            if (isSenId && isRecId) {
              Serial.println("Verification successful!");
              verifiedNodes[n - 1] = true;
              flag = true;
            }
          }
        } else {
          Serial.println("Verification failed!");
        }
      } else {
        Serial.print("No reply from node ");
        Serial.println(n);
      }
    }
  }
  delay(10000);
  return flag;
}