//reads from multiple sensors and prints readings to terminal

// LCD setup
#include <LiquidCrystal.h> //include library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//time setup
float time;
int index;

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
  Serial.print("Pressure (MPa)"); //pressure first
  Serial.print(",");
  Serial.println("Temperature (C)");
  
  //start LCD
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  // read pins
  pressure_sensor_reading = analogRead(PRESSURE_SENSOR_PIN);
  temp_sensor_reading = analogRead(TEMP_SENSOR_PIN);

  // pressure conversion
  voltage = pressure_sensor_reading*(5.0/16382.0); //from 5V rail
  pressure = (voltage/5.0-0.1)/0.6667; //from datasheet in MPa

  // temperature conversion
  R = 16382.0/temp_sensor_reading-1; // sensor resistance using 14 bit measurement
  temp = 1.0/(log(R)/B+1/298.15)-273.15; // convert to temperature

  // calculate time
  time = index*0.1; //in seconds, currently takes measurement every 100ms
  index = index+1; //iterate each loop

  //display on LCD
  lcd.print("Pres (MPa): ");
  lcd.print(pressure);
  lcd.setCursor(0,1); //change lines
  lcd.print("Temp (C): ");
  lcd.print(temp);
  lcd.print("     ");

  //print to Serial
  Serial.print(time); Serial.print(","); Serial.print(pressure); Serial.print(","); Serial.println(temp);
  delay(100); //for readability
}
