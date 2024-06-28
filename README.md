# LoRaMesh-Netowork-with-CMAC-Authentication
Fully functional LoRaMesh Network working in Arduino Uno Enabled with CMAC Authentication

### Setup Instructions

#### Node Setup

1. **Set Node ID:**
   - Upload `SetNodeId.ino` sketch to the first node.
   - Set the Node ID to 1.

2. **Upload LoRaMesh Sketch:**
   - Upload `LoRaMesh.ino` sketch to the same Arduino Uno device.
   
3. **Repeat for Additional Nodes:**
   - Repeat the above steps for up to 255 devices.
   - Change the Node ID for each device accordingly.

#### Authentication Setup

1. **First Node Setup:**
   - Upload `auth_main.ino` to the first device (Arduino Mega).
   - Ensure the Node ID is set to 1 in the `init.cpp` file.

2. **Other Nodes Setup:**
   - Upload `auth.ino` to each additional node.
   - Change the Node ID in the `init.cpp` file for each node.

### Notes

- **Authentication Code Compatibility:**
  - The authentication code will only run on Arduino Mega devices.

 ### Libraries
 - RadioHead : https://github.com/sparkfun/SparkFun_RadioHead_Arduino_Library
 - AES_CMAC : https://github.com/Industrial-Shields/arduino-AES_CMAC
 - Crypto : https://github.com/OperatorFoundation/Crypto 
 
