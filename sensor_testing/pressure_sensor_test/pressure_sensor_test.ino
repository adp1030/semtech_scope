int PRESSURE_SENSOR_PIN = A1;
int sensor_reading;

void setup() {
  // put your setup code here, to run once:
  long baudRate = 9600;
  Serial.begin(baudRate);
}


void loop() {
  // put your main code here, to run repeatedly:
  sensor_reading = analogRead(PRESSURE_SENSOR_PIN);
  Serial.print("Sensor Reading:");
  Serial.print(sensor_reading);
  delay(100); //for readability
}
