//reads from multiple sensors and prints readings to terminal

//network setup
#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h" 
const char* ssid = SECRET_SSID;        // wifi SSID
const char* password = SECRET_PASS;    // wifi password

// Device ID in JEDI
const char* jediID = "Arduino_UNO_WiFi_Rev4";

// IP address of server or Raspberry Pi running Machinechat JEDI software
const char* serverAddress = "192.168.35.112";
int port = 8100; //Default port
const char* server_name = "Jedi_One";

//setup wifi
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

//time setup
float seconds; //can't name something time in Arduino IoT cloud
int index;
int time;

//pressure sensor setup
int PRESSURE_SENSOR_PIN = A1;
int pressure_sensor_reading;
float voltage;
float pressure; //conversion from datasheet

//temperature sensor setup
int TEMP_SENSOR_PIN = A3;
int temp_sensor_reading; //14 bit reading
int R0 = 100000; //zero power resistance reference
int B = 4275; //sensor constant
float R; //sensor resistance conversion
float temp; //temp conversion in C

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(14); //change to 14-bit resolution
  long baudRate = 9600;
  index = 0;
  Serial.begin(baudRate);

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
  
}

void loop() {
  // put your main code here, to run repeatedly:
  String postData;

  // read pins
  pressure_sensor_reading = analogRead(PRESSURE_SENSOR_PIN);
  temp_sensor_reading = analogRead(TEMP_SENSOR_PIN);

  // pressure conversion
  voltage = pressure_sensor_reading*(5.0/16382.0); //from 5V rail
  pressure = (((voltage/5.0-0.1)/0.6667))*145.038; //from datasheet in MPa, then convert to psi

  // temperature conversion
  R = 16382.0/temp_sensor_reading-1; // sensor resistance using 14 bit measurement
  temp = 1.0/(log(R)/B+1/298.15)-273.15; // convert to temperature

  // calculate time
  time = index*5; //in seconds, currently takes measurement every 5s
  index = index+1; //iterate each loop
  
  //send over WiFi
  StaticJsonDocument <200> doc; //using ArduinoJson library
  
  JsonObject context = doc.createNestedObject("context");
  context["target_id"] = String(jediID);

  JsonObject data = doc.createNestedObject("data");
  data["pressure"] = pressure;
  data["temperature"] = temp;

  serializeJson(doc, postData);

  //This prints the JSON to the serial monitor screen
  Serial.println(postData);
  
  //make POST request
  Serial.println("making POST request");
  String contentType = "application/json";

  client.post("/v1/data/mc", contentType, postData);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  delay(5000);
}
