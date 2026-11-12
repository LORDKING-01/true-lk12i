#include <Arduino.h>
const int clockPin = 10; // Connected to the 555 timer output (Clock)
const int dataPin = 11; // Connected to DS (Data In) on the shift register
const int clockReset = 2; //manually resets the clock at the start of run
const int counterQ0 = 4; //reads when 4017 hits Q0
const int loadPin = 5;
const int clockInh = 7;
const int qhPin = 6;
const int sipoLatch = 12;
//Pattern to transmit
//RMB byte pattern is in reverse order

int bitIndex = 7; // Start from the most significant bit
int pClockIndex = 0;
int inA, inB, inC, inD, inE, inF, inG, inH;
int outA = 0;
int outB = 1;
int outC = 0;
int outD = 1;
int outE = 0;
int outF = 1;
int outG = 0;
int outH = 1;

int outQ[8] = {outA, outB, outC, outD, outE, outF, outG, outH};

void setup() {
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(clockReset, OUTPUT);
pinMode(counterQ0, INPUT);
pinMode(loadPin, OUTPUT);
pinMode(qhPin, INPUT);
pinMode(clockInh, OUTPUT);
digitalWrite(clockReset, LOW);
digitalWrite(clockReset, HIGH);
pinMode(sipoLatch, OUTPUT);

}

void loop() {
clockPulse();

int currentBit = outQ[bitIndex];

// Send that bit to the shift register's data pin
digitalWrite(dataPin, currentBit);

// Move to the next bit for the next clock tick (looping back from 7 to 0)
bitIndex--;
if (bitIndex < 0) {
bitIndex = 7;
digitalWrite(sipoLatch, HIGH);
delayMicroseconds(30);
digitalWrite(sipoLatch, LOW);


updateOutQ();


// Optional: You could change bytePattern here to display a new frame!
}




}

void PisoShift() {
  switch (pClockIndex) {
    case 0:
      // Load parallel data into the register
      digitalWrite(clockInh, HIGH); // Inhibit clock during load
      digitalWrite(loadPin, LOW);   // Assert load
      break;

    case 1:
      // Release load and prepare to shift out first bit
      digitalWrite(clockInh, LOW);  // Enable clock
      digitalWrite(loadPin, HIGH);  // Deassert load
      inH = digitalRead(qhPin);     // Read first bit (H)
      break;

    case 2:
      inG = digitalRead(qhPin);
      break;

    case 3:
      inF = digitalRead(qhPin);
      break;

    case 4: 
      inE = digitalRead(qhPin);
      break;

    case 5: 
      inD = digitalRead(qhPin);
      break;

    case 6:
      inC = digitalRead(qhPin);
      break;

    case 7: 
      inB = digitalRead(qhPin);
      break;

    case 8:
      inA = digitalRead(qhPin);
      break;

    case 9:
      // 10th pulse (Index 9): Extra pulse or frame boundary cleanup
      digitalWrite(clockInh, HIGH); // Inhibit clock for the end of cycle
      break;
      
    default:
      pClockIndex = -1; // Safety fallback
      break;
  }

  // Increment and wrap around every 10 pulses (0 to 9)
  pClockIndex++;
  if (pClockIndex >= 10) {
    pClockIndex = 0;
  }
}

void updateOutQ(){
int outQ[8] = {outA, outB, outC, outD, outE, outF, outG, outH};
}

void clockPulse(){
  digitalWrite(clockPin, HIGH);
  delay(20);
  digitalWrite(clockPin,LOW);
  delay(20);
}