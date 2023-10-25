/**
 * Feather HUZZAH ESP8266 Jedi One Demo
 * 
 * Connect ESP8266 to BME280 through I2C and run this demo.
 * Based on: https://support.machinechat.io/hc/en-us/articles/360050302333--30-Minutes-or-Less-Build-a-Wireless-Sensor-Network-Using-NodeMCU
 */

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Pin settings
const int LED_PIN = LED_BUILTIN;

// Wait (milliseconds) between postings
const int DELAY = 5000;

// Network Settings
const char* ssid     = "<your WiFi SSID>";
const char* password = "<your WiFi password>";

// IP address of server or Raspberry Pi running Machinechat JEDI software
// If you changed the JEDI port number, replace 8100 with the new port
const char* host = "192.168.1.204:8100";

// Globals
Adafruit_BME280 bme;

void setup() {

  // Use onboard LED to convey status
  pinMode(LED_PIN, OUTPUT);

  // Configure serial port
  Serial.begin(115200);
  Serial.println();

  // Initialize BME280
  if (!bme.begin()) {
    Serial.println("Could not find BME280 sensor, check wiring.");
    while(1) {
      blinkLED(1, 100); // Status code: fast blink forever is error
    }
  }

  // Connect to WiFi
  Serial.print("Connected to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected!");
}

void loop() {

  float temp_c;
  float humd;
  float pres;
  String post_data;
  WiFiClient client;
  HTTPClient http;
  String address;
  int http_code;

  // Read BME280 sensor values
  temp_c = bme.readTemperature();
  humd = bme.readHumidity();
  pres = bme.readPressure() / 100.0F;

  // Print values to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp_c, 1);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humd, 1);
  Serial.println("%");
  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" hPa");

  // Build a string (JSON) with data to send to JEDI. Format is:
  // {
  //    "context": {
  //        "target_id" : "Sensor1"
  //    },
  //    "data": {
  //        "metric1" : metric_value,
  //        "metric2" : metric_value
  //    }
  // }
  //
  // Replace metric1 with what ever data metrics that you are
  // sending to JEDI. Replace metric_value with the value of
  // the metric. If you have more than one sensor, set the
  // target_id with the name of the sensor.
  post_data = String("{\"context\":{") + 
              String("\"target_id\":\"BMESensorNode1\"") + 
              String("}, \"data\":{") + 
              String("\"temp_c\":") + String(temp_c) + String(", ") +
              String("\"humidity\":") + String(humd) + String(", ") +
              String("\"pressure\":") + String(pres) +
              String(" }}");

  // Send out HTTP request if WiFi is still connected
  if (WiFi.status() == WL_CONNECTED) {

    // Blink LED to show we're transmitting
    digitalWrite(LED_PIN, LOW);

    // Send data to JEDI One using HTTP POST
    address = String("http://") + String(host) + String("/v1/data/mc");
    http.begin(address);
    http.addHeader("Content-Type", "application/json");
    http_code = http.POST(post_data);

    // HTTP response will be negative on error
    if (http_code > 0) {
      
      // Display HTTP response code
      Serial.printf("[HTTP] POST... code: %d\n", http_code);

      // Display response
      if (http_code == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.print("Received payload:  ");
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", 
                    http.errorToString(http_code).c_str());
      blinkLED(3, 100);   // Status code: blink LED rapidly 3 times
    }

    // Close connection
    http.end();

    // Turn off LED when transmission is done
    digitalWrite(LED_PIN, HIGH);
  } else {

    // No WiFi connection
    Serial.println("No WiFi connection");
    blinkLED(5, 100);   // Status code: blink LED rapidly 5 times
  }

  // Wait before posting again
  delay(DELAY);
}

void blinkLED(int times, int wait_ms) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, LOW);
    delay(wait_ms);
    digitalWrite(LED_PIN, HIGH);
    delay(wait_ms);
  }
}