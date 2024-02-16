unsigned long prev_millis = 0;

void setup() {
  analogReadResolution(14);  // Set ADC resolution to 14 bits (for Arduino Uno)
  Serial.begin(19200);       // Start serial communication at 9600 bps
  waitForHandshake();        // Wait for the initial handshake signal
}

void loop() {
  unsigned long new_millis = millis();
  if (new_millis - prev_millis >= 1000) {
    prev_millis = new_millis;

    print_analog_readings();
  }
}


void print_analog_readings() {
  // Read and send analog readings for each analog pin
  for (int pin = A0; pin <= A5; pin++) {

    int analogValue = analogRead(pin);   // Read pin
    Serial.print(analogValue);           // Print value

    if (pin < A5) {                      // Print a comma except at end
      Serial.print(',');
    }
  }
  Serial.print('\n');             // Print newline
}


void waitForHandshake() {
  while (true) {
    if (Serial.available() > 0) {
      String handshake = Serial.readStringUntil('\n');

      // Check if the received handshake signal is "HelloArduino"
      if (handshake.equals("HelloArduino")) {
        // Send acknowledgment to the computer
        Serial.println("ArduinoReady");
        break;
      }
    }
  }


}