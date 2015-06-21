

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

byte pattern[8][8] = {
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1},
  {0,0,1,1,0,0,1,1}
};

/*
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, byte val)
{
      int i;

      for (i = 0; i < 8; i++)  {
            if (bitOrder == LSBFIRST)
                  digitalWrite(dataPin, !!(val & (1 << i)));
            else      
                  digitalWrite(dataPin, !!(val & (1 << (7 - i))));
                  
            digitalWrite(clockPin, HIGH);
            digitalWrite(clockPin, LOW);            
      }
}
*/

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
  for (int i = 0; i < 100; i++) {
    
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchOut, LOW);
    digitalWrite(latchIn, LOW);
    
    // shift out the bits:
//    if ((i % 5) == 0)
      shiftOut(dataOut, clockOut, LSBFIRST, B11111111);
//    else
//      shiftOut(dataOut, clockOut, LSBFIRST, B00000000); 
      
    shiftOut(dataIn, clockIn, LSBFIRST, B00000000);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchOut, HIGH);
    digitalWrite(latchIn, HIGH);
    
    // pause before next value
    delay(1);
  }
}
