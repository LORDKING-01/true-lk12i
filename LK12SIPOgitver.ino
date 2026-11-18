#include <Arduino.h>
const int clockPin = 10; // Connected to the 555 timer output (Clock)
const int dataPin = 11; // Connected to DS (Data In) on the shift register
const int sipoLatch = 12;


int bitIndex = 7; // Start from the most significant bit
int pClockIndex = 0;
int inA, inB, inC, inD, inE, inF, inG, inH;


int outA = 1;
int outB = 0;
int outC = 1;
int outD = 0;
int outE = 1;
int outF = 0;
int outG = 1;
int outH = 0;


// int outA = 0;
// int outB = 1;
// int outC = 0;
// int outD = 1;
// int outE = 0;
// int outF = 1;
// int outG = 0;
// int outH = 1;


int outQ[8] = {outA, outB, outC, outD, outE, outF, outG, outH};


void setup() {
Serial.begin(9600);
Serial.setTimeout(50);
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(sipoLatch, OUTPUT);


}


void loop() {
  // 1. Get the current bit
  int currentBit = outQ[bitIndex];


  // 2. Put it on the data pin FIRST
  digitalWrite(dataPin, currentBit);


  // 3. THEN pulse the clock so the 595 reads it
  clockPulse();


  // 4. Move to the next bit
  bitIndex--;
  if (bitIndex < 0) {
    bitIndex = 7;
    
    // Pulse the output latch (STCP) to send it to the LEDs
    digitalWrite(sipoLatch, HIGH);
    delayMicroseconds(30);
    digitalWrite(sipoLatch, LOW);

    flashByte();
    updateOutQ();
  }

}


void updateOutQ(){

  outQ[0] = outA;
  outQ[1] = outB;
  outQ[2] = outC;
  outQ[3] = outD;
  outQ[4] = outE;
  outQ[5] = outF;
  outQ[6] = outG;
  outQ[7] = outH;
}

void flashByte(){
  if(Serial.available() > 0)
  {
    
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 8)
    {
      for(int i = 0; i < 8; i++)
      {
        outQ[i] = input[i] - '0';
      }
    }
    
  }
}

void clockPulse(){
  digitalWrite(clockPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(clockPin,LOW);
  delayMicroseconds(30);
}
