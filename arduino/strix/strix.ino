#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "Strix";
char pass[] = "owl12345678";
const int led = LED_BUILTIN;
const int rngPins[] = {A1, A2, A3, A4}; // Array of RNG pins
const int numPins = 4;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

const int CHUNK_SIZE = 1000;  // Adjust this value as needed

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT); // Set the LED pin mode
  digitalWrite(led, HIGH); // Ensure LED is off at boot

  for (int i = 0; i < numPins; i++) {
    pinMode(rngPins[i], INPUT);  // Set the RNG pin modes
  }

  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Print the network name (SSID)
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  // Wait 10 seconds for connection
  delay(10000);

  // Start the web server on port 80
  server.begin();

  // Print WiFi status
  printWiFiStatus();
}

void loop() {
  if (status != WiFi.status()) {
    status = WiFi.status();
    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP");
    } else {
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Check if the request is a GET request and contains "length"
    if (request.indexOf("GET") >= 0 && request.indexOf("length=") >= 0) {
      int lengthIndex = request.indexOf("length=") + 7;
      int lengthEndIndex = request.indexOf(" ", lengthIndex);
      String lengthString = request.substring(lengthIndex, lengthEndIndex);
      int length = lengthString.toInt();

      // Send the response to the client in chunks
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();

      sendRandomHex(client, length);
    }
    delay(1);
    client.stop();
  }
}

void sendRandomHex(WiFiClient &client, int length) {
  int bitsNeeded = length * 8;
  int byteValue[numPins] = {0};
  int bitCount = 0;

  char hexBuffer[CHUNK_SIZE * 2 + 1];  // Buffer to hold the hex representation of the chunk
  int bufferIndex = 0;

  for (int i = 0; i < bitsNeeded; i += numPins) {
    for (int j = 0; j < numPins; j++) {
      int bit = analogRead(rngPins[j]) & 1; // Read the least significant bit from each pin
      byteValue[j] = (byteValue[j] << 1) | bit;

      // Blink LED according to the bit value from A1
      if (j == 0) { // Only for A1
        digitalWrite(led, bit ? LOW : HIGH);
      }
    }
    bitCount++;

    if (bitCount == 8) {
      for (int j = 0; j < numPins; j++) {
        snprintf(hexBuffer + bufferIndex, 3, "%02X", byteValue[j]); // Convert byte to hex string
        bufferIndex += 2;
        byteValue[j] = 0;
      }
      bitCount = 0;
    }

    // Send the data in chunks to avoid memory overflow
    if (bufferIndex >= CHUNK_SIZE * 2) {
      digitalWrite(led, HIGH);
      hexBuffer[bufferIndex] = '\0';
      client.print(hexBuffer);
      bufferIndex = 0;
    }
  }

  // Ensure any remaining data is sent
  if (bufferIndex > 0) {
    digitalWrite(led, HIGH);
    hexBuffer[bufferIndex] = '\0';
    client.print(hexBuffer);
  }

  // Ensure all data is sent
  client.flush();
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
