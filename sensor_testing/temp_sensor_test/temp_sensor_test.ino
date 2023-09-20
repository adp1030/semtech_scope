int TEMP_SENSOR_PIN = A3;
int sensor_reading; //14 bit reading
int R0 = 100000; //zero power resistance reference
int B = 4275; //sensor constant
float R; //sensor resistance conversion
float temp; //temp conversion in C

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(14); //change to 14-bit resolution
  long baudRate = 9600;
  Serial.begin(baudRate);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor_reading = analogRead(TEMP_SENSOR_PIN);

  R = 16382.0/sensor_reading-1; // sensor resistance using 14 bit measurement
  temp = 1.0/(log(R)/B+1/298.15)-273.15; // convert to temperature

  //Serial.print("Temperature (C): "); //report temp
  Serial.println(temp);
  delay(100); //for readability
}
