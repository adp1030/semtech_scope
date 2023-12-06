int x;

#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

//Define LED matrices to switch between

const uint32_t happy[] = {
    0x19819,
    0x80000001,
    0x81f8000
};
const uint32_t heart[] = {
    0x3184a444,
    0x44042081,
    0x100a0040
};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  matrix.begin();
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();

  if (x == 0){
    matrix.loadFrame(heart);
  }
  if (x == 1){
    matrix.loadFrame(happy);
  }

}