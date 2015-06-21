// Mega Matrix: A 24x24 LED Matrix
//
// Our matrices are Command cathode; a 3x3 array of 8x8 matrices.
// The row controllers will act as emitters, and the columns as
// receivers.  (Using 74HC595 shift registers)
//
// To denote patterns for ourselves (not the device) we'll have
//   0 = LED Off
//   1 = LED On
// 
// Though the machine will flip the bits for us to turn the correct
// LEDs on.
//
// Say we want to light up a row like this:  1 1 0 1
// We Have to do this:
//           Cols:|   1   2   3   4 
//  Rows:         |
// ---------------+-----------------
// (n-1):     0   |   x   x   x   x
// (n):       1   |   0   0   1   0
// (n+1):     0   |   x   x   x   x

// x = Don't care
// 0 = LOW
// 1 = HIGH


// Row: Acts as the emmiter, all pins on port D of atmega
int rData = 2;      // PD2
int rClock = 3;     // PD3
int rLatch = 4;     // PD4
int rEnable = 5;    // PD5
int rReset = 6;     // PD6

// Colunm: Acts as the Reicver, all pins on port B of atmega
int cData = 8;      // PB0
int cClock = 9;     // PB1
int cLatch = 10;    // PB2
int cEnable = 11;   // PB3
int cReset = 12;    // PB4

// The need for speed:
// For the Row controller
#define R_DATA_HIGH PORTD |= _BV(PD2)
#define R_DATA_LOW PORTD &= ~_BV(PD2)
#define R_CLOCK_HIGH PORTD |= _BV(PD3)
#define R_CLOCK_LOW PORTD &= ~_BV(PD3)
#define R_LATCH_HIGH PORTD |= _BV(PD4)
#define R_LATCH_LOW PORTD &= ~_BV(PD4)
#define R_ENABLE_HIGH PORTD |= _BV(PD5)
#define R_ENABLE_LOW PORTD &= ~_BV(PD5)
#define R_RESET_HIGH PORTD |= _BV(PD6)
#define R_RESET_LOW PORTD &= ~_BV(PD6)

// For the Column controller
#define C_DATA_HIGH PORTB |= _BV(PB0)
#define C_DATA_LOW PORTB &= ~_BV(PB0)
#define C_CLOCK_HIGH PORTB |= _BV(PB1)
#define C_CLOCK_LOW PORTB &= ~_BV(PB1)
#define C_LATCH_HIGH PORTB |= _BV(PB2)
#define C_LATCH_LOW PORTB &= ~_BV(PB2)
#define C_ENABLE_HIGH PORTB |= _BV(PB3)
#define C_ENABLE_LOW PORTB &= ~_BV(PB3)
#define C_RESET_HIGH PORTB |= _BV(PB4)
#define C_RESET_LOW PORTB &= ~_BV(PB4)

// NOTE Could consolidate the Reset and enable pins possibly
//      as well as the latch.  Test this later.





#define NUM_BYTES 72  // 24 * 3
#define NUM_ROWS 24
#define NUM_COLS 24
boolean bufferReady = false;
byte bytesRead = 0;
byte image[NUM_BYTES];           // Image that's currently being displayed
byte image_buffer[NUM_BYTES];    // Buffer for next image


// Some patterns
byte CSH_logo[NUM_BYTES] = {
   B00000000, B00000000, B00000000,
   B00111111, B11111111, B10011110,
   B01111111, B11111111, B11011110,
   B01111111, B11111111, B11011110,
   B01111111, B11111111, B11011110,
   B01111000, B00000011, B11011110,
   B01111011, B11111011, B11011110,
   B01111011, B11111000, B00011110,
   B01111011, B11111011, B11011110,
   B01111011, B10111011, B11011110,
   B01111011, B10000011, B11111110,
   B01111011, B11111011, B11111110,
   B01111011, B11111011, B11111110,
   B01111000, B00111011, B11111110,
   B01111011, B10111011, B11011110,
   B01111011, B11111011, B11011110,
   B01111011, B11111000, B00011110,
   B01111011, B11111011, B11011110,
   B01111000, B00000011, B11011110,
   B01111111, B11111111, B11011110,
   B01111111, B11111111, B11011110,
   B01111111, B11111111, B11011110,
   B00111111, B11111111, B10011110,
   B00000000, B00000000, B00000000,
};

byte test_pattern[NUM_BYTES] = {
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
  B01010101, B01010101, B01010101,
  B10101010, B10101010, B10101010,
};


byte buffer[24] = {0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0};
byte curVal = 0;


void clsCols() {
  // Clears out all of the columns (Turns them off by setting their values to 1)
  C_DATA_HIGH;
  for (int c = 0; c < NUM_COLS; c++) {
    C_CLOCK_HIGH;
    C_CLOCK_LOW;
  }
  C_DATA_LOW;
    
  C_LATCH_HIGH;
  C_LATCH_LOW;
}


void clsRows() {
  // Clear out the shift register
  R_RESET_LOW;
  R_RESET_HIGH;
}


void setup() {
  Serial.begin(115200);
  
  // Set everything to output
  pinMode(rData, OUTPUT);
  pinMode(rClock, OUTPUT);
  pinMode(rLatch, OUTPUT);
  pinMode(rEnable, OUTPUT);
  pinMode(rReset, OUTPUT);
  pinMode(cData, OUTPUT);
  pinMode(cClock, OUTPUT);
  pinMode(cLatch, OUTPUT);
  pinMode(cEnable, OUTPUT);
  pinMode(cReset, OUTPUT);
  
  // Turn on output, disable reset
  R_ENABLE_LOW;
  C_ENABLE_LOW;
  R_RESET_HIGH;
  C_RESET_HIGH;

  // Start with a clean slate
  R_ENABLE_HIGH;
  C_ENABLE_HIGH;
//  clsRows();
//  clsCols();
  C_ENABLE_LOW;
  R_ENABLE_LOW;
}


void loop() {  
//  unsigned long start = micros();
  
  // This is not using buffered IO like it should, will figure that out later
  byte bytesReady = Serial.available();
  
  while (bytesReady > 0) {
    byte val = Serial.read();
  
    if (val == '0') {
      curVal = 0;
      bufferReady = true;
      Serial.println("OFF");
    } else if (val == '1') {
      curVal = 1;
      bufferReady = true;
      Serial.println("ON");
    }
    
    bytesReady = Serial.available();
  }

  
  if (bufferReady) {
    if (curVal == 1)
      C_DATA_HIGH;
    else if (curVal == 0)
      C_DATA_LOW;
    
    C_CLOCK_HIGH;
    C_CLOCK_LOW;
    
    
    C_ENABLE_HIGH;
    
    C_LATCH_LOW;
    C_LATCH_HIGH;
    
    C_ENABLE_LOW;
      
    bufferReady = false; 
  }
  
}
