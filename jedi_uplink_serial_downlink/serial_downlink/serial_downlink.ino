/*
Serial input key:
Shutoff low: 0
Shutoff high: 1
Drip low: 2
Drip high: 3

Write digital pins, delay for 5 seconds potentially to actuate valve, then set low
*/

int x; //serial input

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  //Write enable pins high
  digitalWrite(2,HIGH);
  digitalWrite(10,HIGH);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();

  if (x == 0){ //Shutoff low
    digitalWrite(3, HIGH);
    delay(6000);
    digitalWrite(3, LOW);
  }
  if (x == 1){ //Shutoff high
    digitalWrite(4, HIGH);
    delay(6000);
    digitalWrite(4, LOW);
  }
  if (x == 2){ //Drip low
    digitalWrite(11, HIGH);
    delay(6000);
    digitalWrite(11, LOW);
  }
  if (x == 3){ //Drip high
    digitalWrite(12, HIGH);
    delay(6000);
    digitalWrite(12, LOW);
  }

}