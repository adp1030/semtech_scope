//Arduino code for ESP32 modules to send environmental sensor data
//from a BME280 module (i2c connect) every 5 seconds to Machinechat's JEDI One software
//The serial monitor will show status
//
//Note: Program will not proceed until a valid BME sensor is detected
//
//v1.01 September 13, 2020 DRM

#include <WiFi.h>
#include <HttpClient.h>
#include <ArduinoJson.h>

// Some BME280 modules are strapped for address 0x76 - change line below as needed
#define I2C_ADDR 0x77

Adafruit_BME280 bme;

// Create a unique ID for the data from each ESP32 running this code
const char* jediID = "ESP32Sensor51";

// Wi-Fi settings - replace with your Wi-Fi SSID and password
const char* ssid     = "";
const char* password = "";

// IP address of PC or Raspberry Pi running Machinechat JEDI software
// If you changed the JEDI port number, replace 8100 with the new port
const char* host = "192.168.35.116:8100";

// This is the setup section and will only run one time
void setup() {

  // Configure serial port for debug when ESP32 board is connected
  // to computer using USB port
  Serial.begin(115200);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(500);
  }

  // Initiate Wi-Fi connection setup
  WiFi.begin(ssid, password);

  // Show status on serial monitor
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  // Wait for Wi-Fi connection and show progress on serial monitor
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Configure BME280 Sensor

  Serial.println("Checking BME280");
  unsigned status;

  status = bme.begin(I2C_ADDR);

  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address\n");
    while (1) delay(10); //***program will hang here if no valid sensor attached***
  } else {
    Serial.println("BME280 Detected");
  }
}

// This is the main section and will loop continuously
void loop() {

  String postData;

  // This is the minimum amount of time to wait before
  // reading the sensor
  delay(1000);

  // Read the sensor, convert readings and set variables
  float tempC = bme.readTemperature();
  float humi = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float tempF = (tempC * 1.8) + 32.0F;
  float pressure_inches = pressure * 0.0295300586F;

  //Following code creates the serialized JSON string to send to JEDI One
  //using ArduinoJson library
  StaticJsonDocument <200> doc;

  JsonObject context = doc.createNestedObject("context");
  context["target_id"] = String(jediID);

  JsonObject data = doc.createNestedObject("data");
  data["tempF"] = tempF;
  data["humi"] = humi;
  data["pressure"] = pressure_inches;

  serializeJson(doc, postData);

  //This prints the JSON to the serial monitor screen
  //and can be commented out
  Serial.println(postData);

  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    // Send the data to JEDI using HTTP.
    String address = String("http://") + String(host) + String("/v1/data/mc");
    http.begin(client, address);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(postData);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.print("received payload:  ");
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();  //Close connection

  } else {
    Serial.println("Error in WiFi connection");
  }
  // Wait for 5 seconds before repeating the loop
  delay(5000);
}