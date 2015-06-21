

// "Recv," register
int dataRecv = 2;    // `SER` or `DS` pin on 74HC595
int clockRecv = 3;   // `SRCLK` or `SH_CP` pin on 74HC595
int latchRecv = 4;   // `RCLK` or `STCP` pin on 74HC595

// "Emit," register
int dataEmit = 5;
int clockEmit = 6;
int latchEmit = 7;

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
  //set pins to Recvput so you can control the shift register
  pinMode(dataRecv, OUTPUT);
  pinMode(clockRecv, OUTPUT);
  pinMode(latchRecv, OUTPUT);
  
  pinMode(dataEmit, OUTPUT);
  pinMode(clockEmit, OUTPUT);
  pinMode(latchEmit, OUTPUT);
  
  
      digitalWrite(latchEmit, LOW);
    digitalWrite(latchRecv, LOW);
    
    // Shift on emmiter
    shiftOut(dataEmit, clockEmit, MSBFIRST, B00000000);  // All on
    
    // Shift out recv
    shiftOut(dataRecv, clockRecv, MSBFIRST, B00000000);  // All on
    
    // Turn the latches high
    digitalWrite(latchEmit, HIGH);
    digitalWrite(latchRecv, HIGH);
    
    delay(100);
}

void loop() {
  // Testing with an 1x8 matrix
  for (int i = 0; i < 8; i++) {

  }
  
/*  
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
    
    // pause before next value
    delay(100);
  }
*/
}
