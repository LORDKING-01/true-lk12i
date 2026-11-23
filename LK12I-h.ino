#include <Arduino.h>

//PISO (74x165 to Arduino)
const int pDataPin = 6;   // Connects to QH (Pin 9)
const int pClockPin = 5;  // Connects to CLK (Pin 2)
const int pLoadPin = 9;   // Connects to SH/LD (Pin 1)

////SIPO (74x595 to Arduino)
const int sClockPin = 10;  // Connected to the 555 timer output (Clock)
const int sDataPin = 11;   // Connected to DS (Data In) on the shift register
const int sLatch = 12;

int bitIndex = 7;  // Used for 595 SIPO Shift Indexing
int pClockIndex = 0; // Used for 165 Shift Indexing
int inA, inB, inC, inD, inE, inF, inG, inH; // Input Values set by PISO

//Values set to SIPO
int outA = 0;
int outB = 0;
int outC = 0;
int outD = 0;
int outE = 0;
int outF = 0;
int outG = 0;
int outH = 0;



//Arrays for in/out
int outQ[8] = { outA, outB, outC, outD, outE, outF, outG, outH };
int inQ[8];

void setup() {

  Serial.begin(9600); //Used to establish a serial connection, Make sure it matces your BAUD in serial monitor
  Serial.setTimeout(10); //Used to lower input delay when sending serial data

  //Arduino pin init
  pinMode(sClockPin, OUTPUT);
  pinMode(sDataPin, OUTPUT);
  pinMode(sLatch, OUTPUT);

  pinMode(pDataPin, INPUT);
  pinMode(pClockPin, OUTPUT);
  pinMode(pLoadPin, OUTPUT);
}


void loop() {
  //Main loops, calls PisoShift and SipoShft constantly
  PisoShift();
  SipoShift();

  //Can add a small delay here (delayMicroSeconds(10)) for stability
}

void SipoShift() {
  //SIPO

  //Calls the current bit from SIPO array
  int currentBit = outQ[bitIndex];

  //Maps that bit to the data pin
  digitalWrite(sDataPin, currentBit);

  //Pulses the clock to move onto the next bit
  clockPulse();

  //Move to the next bit on software-side
  bitIndex--;
  
  //After running all 8 bits
  if (bitIndex < 0) {
    bitIndex = 7; //Reset bitIndex

    // Pulse the output latch (STCP) to send it to the LEDs
    digitalWrite(sLatch, HIGH);
    delayMicroseconds(3);
    digitalWrite(sLatch, LOW);

    //update output byte
    flashByte();
  }
}

void flashByte() {
  //Default code for the purpose of testing output functionality with a keyboard
  //Communicates over the serial port

  //Code marked with an x can be replaced with however you want to change your byte
  if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n'); //x
    input.trim();                                //x
    if (input.length() == 8) {                   //x

      for (int i = 0; i < 8; i++) {

        outQ[i] = input[i] - '0';
      }
    }                                            //x
  }                                              //x
}

void clockPulse() {
  //Pulses the clock, giving it a HIGH edge
  digitalWrite(sClockPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sClockPin, LOW);
  delayMicroseconds(10);
}

void PisoShift() {
  // Save parallel inputs into the register
  digitalWrite(pLoadPin, LOW);
  delayMicroseconds(12);  //Barely overclockable
  digitalWrite(pLoadPin, HIGH);

  //Read last bit manually or else it is skipped due to nature of shiftIn()
  inQ[7] = digitalRead(pDataPin);
  //Read all data coming from datapin - last bit
  byte inputByte = shiftIn(pDataPin, pClockPin, MSBFIRST);

  //map inQ to bitRead
  Serial.print("Inputs (H to A): ");
  for (int i = 0; i < 7; i++) {
    inQ[i] = bitRead(inputByte, i + 1);
  }

  //Print the inputs for debugging purposes
  for (int i = 0; i < 8; i++) {
    Serial.print(inQ[i]);
  }
    Serial.print("\n");

}



