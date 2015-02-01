/* Build led control using the thermal sensor */

const int ledOne = 2;
const int ledTwo = 3;
const int ledThree = 4;
const int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0) * 5.0;
  float temperature = voltage * 100;

  loveMeterLed(temperature);

  delay(1);
}

void loveMeterLed(float temperature) {
  if (temperature <= 22.0) {
    digitalWrite(ledOne, LOW);
    digitalWrite(ledTwo, LOW);
    digitalWrite(ledThree, LOW);  
  }
  
  if (temperature > 22.0) {
    digitalWrite(ledOne, HIGH);
    digitalWrite(ledTwo, LOW);
    digitalWrite(ledThree, LOW);  
  }
  
  if (temperature > 24.0) {
    digitalWrite(ledOne, HIGH);
    digitalWrite(ledTwo, HIGH);
    digitalWrite(ledThree, LOW);  
  }
  
  if (temperature > 26.0) {
    digitalWrite(ledOne, HIGH);
    digitalWrite(ledTwo, HIGH);
    digitalWrite(ledThree, HIGH);  
  }
}
