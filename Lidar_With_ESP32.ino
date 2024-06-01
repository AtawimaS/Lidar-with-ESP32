#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "2+1=4";
const char* password = "yangbenar3";

#define RX_PIN 19 // Connect the LiDAR TX pin to this pin
#define TX_PIN 18 // Connect the LiDAR RX pin to this pin

HardwareSerial sft1(1); // Use HardwareSerial port 1 (UART1)

int dist;
int strength;
int check;
int uart[9];
int i;
const int HEADER = 0x59;

void setup() {
  Serial.begin(115200);
  sft1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize the hardware serial port
  WiFi.begin(ssid, password);
  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  ArduinoOTA.setHostname("LiDAR_ESP32");
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("\nHardware serial test started");
  delay(3000);
}

void loop() {
  ArduinoOTA.handle();

  if (sft1.available()) {
    if (sft1.read() == HEADER) {
      uart[0] = HEADER;
      if (sft1.read() == HEADER) {
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) {
          uart[i] = sft1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) {
          dist = uart[2] + uart[3] * 256;
          strength = uart[4] + uart[5] * 256;
          Serial.print("Distance = ");
          Serial.print(dist);
          Serial.print("cm");
          Serial.print("\t");
          Serial.print("strength = ");
          Serial.print(strength);
          Serial.print("\n");
          delay(50);
        }
      }
    }
  }
}
