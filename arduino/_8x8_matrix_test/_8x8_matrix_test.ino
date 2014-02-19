//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

// "Out," register
int dataOut = 2;    // `SER` or `DS` pin on 74HC595
int clockOut = 3;   // `SRCLK` or `SH_CP` pin on 74HC595
int latchOut = 4;   // `RCLK` or `STCP` pin on 74HC595

// "In," register
int dataIn = 5;
int clockIn = 6;
int latchIn = 7;

int ledMapRow[] = {
B10000000,
B01000000,
B00100000,
B00010000,
B00001000,
B00000100,
B00000010,
B00000001,
};

int ledMapColumn[] = {
B01111111,
B10111111,
B11011111,
B11101111,
B11110111,
B11111011,
B11111101,
B11111110,
};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(dataOut, OUTPUT);
  pinMode(clockOut, OUTPUT);
  pinMode(latchOut, OUTPUT);
  
  pinMode(dataIn, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(latchIn, OUTPUT);
  
}

void loop() {
  for (int i = 0; i < 64; i++) {
    
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchOut, LOW);
    digitalWrite(latchIn, LOW);
    
    // shift out the bits:
    shiftOut(dataOut, clockOut, LSBFIRST, ledMapRow[i / 8]);  
    shiftOut(dataIn, clockIn, LSBFIRST, ledMapColumn[i % 8]);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchOut, HIGH);
    digitalWrite(latchIn, HIGH);
    
    // pause before next value:
    delay(5);
  }
}
