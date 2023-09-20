int PRESSURE_SENSOR_PIN = A1;
int sensor_reading;
float voltage;
float pressure; //conversion from datasheet

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(14); //change to 14-bit resolution
  long baudRate = 9600;
  Serial.begin(baudRate);
}


void loop() {
  // put your main code here, to run repeatedly:
  sensor_reading = analogRead(PRESSURE_SENSOR_PIN);
  voltage = sensor_reading*(5.0/16382.0); //from 5V rail
  pressure = (voltage/5.0-0.1)/0.6667; //from datasheet in MPa

  //Serial.print("Pressure (MPa): ");
  Serial.println(pressure);
  delay(100); //for readability
}
