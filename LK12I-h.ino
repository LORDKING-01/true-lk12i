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
  Serial.begin(9600);    // Make sure it matches your BAUD in serial monitor
  Serial.setTimeout(10); // Used to lower input delay when sending serial data

  // Arduino pin init
  pinMode(sClockPin, OUTPUT);
  pinMode(sDataPin, OUTPUT);

  pinMode(pDataPin, INPUT);
  pinMode(pClockPin, OUTPUT);
  pinMode(pLoadPin, OUTPUT);

  // 1. Load the saved phase shift from EEPROM
  phaseOffset = EEPROM.read(PHASE_ADDR);
  if (phaseOffset > 7) {
    phaseOffset = 0; // Default to 0 if EEPROM is uninitialized (255)
    EEPROM.write(PHASE_ADDR, phaseOffset);
  }

  // 2. Apply the phase shift to the 4017 at startup
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

void SipoShift() {
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

    // Update output byte
    flashByte();
  }
}

void flashByte() {
  // Communicates over the serial port
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Shift phase forward by 1 clock cycle and save to EEPROM
    if (input == "+") {
      clockPulse();
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
    // Load new 8-bit pattern
    else if (input.length() == 8) {
      for (int i = 0; i < 8; i++) {
        outQ[i] = input[i] - '0';
      }
    }
  }
}

void clockPulse() {
  // Pulses the clock, giving it a HIGH edge
  digitalWrite(sClockPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sClockPin, LOW);
  delayMicroseconds(10);
}

void PisoShift() {
  // Save parallel inputs into the register
  digitalWrite(pLoadPin, LOW);
  delayMicroseconds(12); // Barely overclockable
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
