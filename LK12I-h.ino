#include <Arduino.h>


const int pDataPin = 6;   // Connects to QH (Pin 9)
const int pClockPin = 5;  // Connects to CLK (Pin 2)
const int pLoadPin = 9;  // Connects to SH/LD (Pin 1)


const int sClockPin = 10; // Connected to the 555 timer output (Clock)
const int sDataPin = 11; // Connected to DS (Data In) on the shift register
const int sLatch = 12;


int bitIndex = 7; // Start from the most significant bit
int pClockIndex = 0;
int inA, inB, inC, inD, inE, inF, inG, inH;


// int outA = 1;
// int outB = 0;
// int outC = 1;
// int outD = 0;
// int outE = 1;
// int outF = 0;
// int outG = 1;
// int outH = 0;


int outA = 0;
int outB = 1;
int outC = 0;
int outD = 1;
int outE = 0;
int outF = 1;
int outG = 0;
int outH = 1;


int outQ[8] = {outA, outB, outC, outD, outE, outF, outG, outH};
int inQ[8];

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(pDataPin, INPUT);
  pinMode(pClockPin, OUTPUT);
  pinMode(pLoadPin, OUTPUT);


  pinMode(sClockPin, OUTPUT);
  pinMode(sDataPin, OUTPUT);
  pinMode(sLatch, OUTPUT);




}


void loop() {
    PisoShift();

  //SIPO
  //// 1. Get the current bit
  int currentBit = outQ[bitIndex];




  // 2. Put it on the data pin FIRST
  digitalWrite(sDataPin, currentBit);

  // 3. THEN pulse the clock so the 595 reads it
  clockPulse();




  // 4. Move to the next bit
  bitIndex--;
  if (bitIndex < 0) {
    bitIndex = 7;
    
    // Pulse the output latch (STCP) to send it to the LEDs
    digitalWrite(sLatch, HIGH);
    delayMicroseconds(3);
    digitalWrite(sLatch, LOW);




    flashByte();
  }
}



void PisoShift(){
  // 1. Snapshot parallel inputs into the register
  digitalWrite(pLoadPin, LOW);
  delayMicroseconds(12); //Barely overclockable
  digitalWrite(pLoadPin, HIGH);

  //Read last bit or else it is skipped due to nature of shiftIn()
  inQ[7] = digitalRead(pDataPin);
  //Read all data coming from datapin - last bit
  byte inputByte = shiftIn(pDataPin, pClockPin, MSBFIRST);

  //map inQ to bitRead
  Serial.print("Inputs (H to A): ");
   for (int i = 0; i < 7; i++) {
    inQ[i] = bitRead(inputByte, i + 1);
  }
  
  for (int i = 0; i < 8; i++)
  {
    Serial.print(inQ[i]);
  }
  
  Serial.println();
}

void flashByte(){
  //code to test serial port and overall functionality of 595
  //can be replaced with what you want your 595 to flash
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
  digitalWrite(sClockPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sClockPin,LOW);
  delayMicroseconds(10);
}