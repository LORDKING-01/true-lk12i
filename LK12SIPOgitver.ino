#include <Arduino.h>
const int clockPin = 10; // Connected to the 555 timer output (Clock)
const int dataPin = 11; // Connected to DS (Data In) on the shift register
const int sipoLatch = 12;


int bitIndex = 7; // Start from the most significant bit
int pClockIndex = 0;
int inA, inB, inC, inD, inE, inF, inG, inH;
int tempCycle = 0;


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


    updateOutQ();
  }
}

void tempFlash(){
    //temp code
  if(tempCycle == 0){
    outA = 1;
    outB = 0;
    outC = 1;
    outD = 0;
    outE = 1;
    outF = 0;
    outG = 1;
    outH = 0;
    tempCycle = 1;
  }
  else if (tempCycle == 1){
    outA = 0;
    outB = 1;
    outC = 0;
    outD = 1;
    outE = 0;
    outF = 1;
    outG = 0;
    outH = 1;
    tempCycle = 0;
  }
}
void updateOutQ(){
  tempFlash();
  outQ[0] = outA;
  outQ[1] = outB;
  outQ[2] = outC;
  outQ[3] = outD;
  outQ[4] = outE;
  outQ[5] = outF;
  outQ[6] = outG;
  outQ[7] = outH;
}


void clockPulse(){
  digitalWrite(clockPin, HIGH);
  delay(30);
  digitalWrite(clockPin,LOW);
  delay(30);
}