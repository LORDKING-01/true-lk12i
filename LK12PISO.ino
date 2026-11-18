#include <Arduino.h>

const int dataPin = 6;   // Connects to QH (Pin 9)
const int clockPin = 10;  // Connects to CLK (Pin 2)
const int latchPin = 9;  // Connects to SH/LD (Pin 1)

void setup() {
  Serial.begin(9600);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void loop() {
  // 1. Snapshot parallel inputs into the register
  digitalWrite(latchPin, LOW);
  delayMicroseconds(5);
  digitalWrite(latchPin, HIGH);

  // 2. Shift out 8 bits serially (MSB first)
  Serial.print("Inputs (H to A): ");
  for (int i = 0; i < 8; i++) {
    int bitVal = digitalRead(dataPin);
    Serial.print(bitVal);
    
    // Pulse clock to advance to the next bit
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(clockPin, LOW);
  }
  Serial.println();
  delay(500);
}
