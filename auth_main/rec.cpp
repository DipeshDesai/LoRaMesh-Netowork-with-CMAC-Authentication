#include "rec.h"

bool rec_loop()
{
  if(rf95.available())
  {  
    uint8_t temp[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(temp);
    if (rf95.recv(temp, &len))
    {
      //RH_RF95::printBuffer("request: ", temp, len);
      Serial.print("got request: ");
      //Serial.println((char*)temp);

      if (len >= 1 + 16)
      {
        uint8_t receivedData[1];
        uint8_t receivedMAC[16];

        // Separate the data and MAC tag from the received buffer
        memcpy(receivedData, temp, 1);
        memcpy(receivedMAC, temp + 1, 16);

        // Generate CMAC tag for the received data
        uint8_t computedMAC[16];
        cmac.generateMAC(computedMAC, key, receivedData, sizeof(receivedData));

        // Compare the received MAC with the computed MAC
        bool isValid = (memcmp(receivedMAC, computedMAC, sizeof(computedMAC)) == 0);

        // Print the result
        Serial.print("Message is valid: ");
        Serial.println(isValid ? "Yes" : "No");

        if(isValid)
        {
          // Send a verification reply
          // uint8_t reply[] = "ok";
          // rf95.send(reply, sizeof(reply));
          
          uint8_t newPayload[2];
          newPayload[0] = receivedData[0];
          newPayload[1] = nodeId;

          // Send back the new payload
          rf95.send(newPayload, sizeof(newPayload));
          rf95.waitPacketSent();
          Serial.println("Sent verification reply");
          return true;
        }
      }
      else
      {
        Serial.println("Invalid packet size");
      }
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  return false;    
}