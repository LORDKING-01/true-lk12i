#include <Arduino.h>
#include <EEPROM.h>


// PISO (74x165 to Arduino)
const int pDataPin = 6;   // Connects to QH (Pin 9)
const int pClockPin = 5;  // Connects to CLK (Pin 2)
const int pLoadPin = 9;   // Connects to SH/LD (Pin 1)


// SIPO (74x595 to Arduino)
const int sClockPin = 10; // Connected to the 4017 & 595 Clock
const int sDataPin = 11;  // Connected to DS (Data In) on the shift register


int bitIndex = 7;    // Used for 595 SIPO Shift Indexing
int pClockIndex = 0; // Used for 165 Shift Indexing
int inA, inB, inC, inD, inE, inF, inG, inH; // Input Values set by PISO


// EEPROM
const int PHASE_ADDR = 0; // EEPROM address to store the phase shift (0-7)
int phaseOffset = 0;


// Values set for SIPO
int outA = 0;
int outB = 0;
int outC = 0;
int outD = 0;
int outE = 0;
int outF = 0;
int outG = 0;
int outH = 0;


// Arrays for in/out
int outQ[8] = {outA, outB, outC, outD, outE, outF, outG, outH};
int inQ[8];


void setup() {
  Serial.begin(500000);    // Make sure it matches your BAUD in serial monitor
  Serial.setTimeout(10); // Used to lower input delay when sending serial data, may need to be edited if sending large serial data


  // Arduino pin init
  pinMode(sClockPin, OUTPUT);
  pinMode(sDataPin, OUTPUT);


  pinMode(pDataPin, INPUT);
  pinMode(pClockPin, OUTPUT);
  pinMode(pLoadPin, OUTPUT);


  // Loads the phaseoffset of EEPROM on startup
  phaseOffset = EEPROM.read(PHASE_ADDR);
  if (phaseOffset > 7) {
    phaseOffset = 0; // Default to 0 if EEPROM is uninitialized (255)
    EEPROM.write(PHASE_ADDR, phaseOffset);
  }


  // Applys the phase shift by manually pulsing the clock to line up the byte
  digitalWrite(sDataPin, LOW);
  for (int i = 0; i < phaseOffset; i++) {
    clockPulse();
  }


  Serial.print("Loaded phase offset: ");
  Serial.println(phaseOffset);
}


void loop() {
  // Main loop, calls PisoShift and SipoShift constantly
  PisoShift();
  SipoShift();
}


void SipoShift() { // Encompasses shifting in the SIPO and sending bits based on the SIPO array
  // Calls the current bit from SIPO array
  int currentBit = outQ[bitIndex];


  // Maps that bit to the data pin
  digitalWrite(sDataPin, currentBit);


  // Pulses the clock to move onto the next bit
  clockPulse();


  // Move to the next bit on software-side
  bitIndex--;


  // After running all 8 bits
  if (bitIndex < 0) {
    bitIndex = 7; // Reset bitIndex


    // Check for serial input once and route it to the appropriate function
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();


      //Runs phaseShift and flashByte
      phaseShift(input);
      flashByte(input);
    }
  }
}


void phaseShift(String input) { // Handles shifting the clock phase and saving to EEPROM
  // Shift phase forward by 1 clock cycle and save to EEPROM
  if (input == "+") {
    clockPulse();  // manually pulses the clock to start fresh
    phaseOffset = (phaseOffset + 1) % 8;
    EEPROM.update(PHASE_ADDR, phaseOffset);
    Serial.print("Phase shifted (+1). Saved offset: ");
    Serial.println(phaseOffset);
  }
  // Shift phase backward by 1 clock cycle (7 forward) and save to EEPROM
  else if (input == "-") {
    for (int i = 0; i < 7; i++) {
      clockPulse();
    }
    phaseOffset = (phaseOffset + 7) % 8;
    EEPROM.update(PHASE_ADDR, phaseOffset);
    Serial.print("Phase shifted (-1). Saved offset: ");
    Serial.println(phaseOffset);
  }
}


void flashByte(String input) { // Encompasses updating the byte 
  // Communicates over the serial port
  // This is the code that you will use to update the byte sent to the 595
  // By default, there is code to manually send and test bytes
  // Code marked with an X can be deleted and replaced with your new code
  
  // Load new 8-bit pattern 
  if (input.length() == 8) {       //x
    for (int i = 0; i < 8; i++) {  //x
      outQ[i] = input[i] - '0';    //x
    }                              //x
  }                                
}


void clockPulse() {    // Pulses the clock, giving it a HIGH edge
  digitalWrite(sClockPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sClockPin, LOW);
  delayMicroseconds(5);
}


void PisoShift() { // Encompasses everything related to the 165
  // Save parallel inputs into the register
  digitalWrite(pLoadPin, LOW);
  delayMicroseconds(9); // Barely overclockable
  digitalWrite(pLoadPin, HIGH);


  // Read last bit manually or else it is skipped due to nature of shiftIn()
  inQ[7] = digitalRead(pDataPin);
  // Read all data coming from datapin - last bit
  byte inputByte = shiftIn(pDataPin, pClockPin, MSBFIRST);


  // Map inQ to bitRead
  Serial.print("Inputs (A-H): ");
  for (int i = 0; i < 7; i++) {
    inQ[i] = bitRead(inputByte, i + 1);
  }


  // Print the inputs for debugging purposes
  for (int i = 0; i < 8; i++) {
    Serial.print(inQ[i]);
  }


  Serial.print("\n");
}