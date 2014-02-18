//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int ledMap[] = {
B10000111,
B10001011,
B10001101,
B10001110,

B01000111,
B01001011,
B01001101,
B01001110,

B00100111,
B00101011,
B00101101,
B00101110,

B00010111,
B00011011,
B00011101,
B00011110,
};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int i = 0; i < 16; i++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, LSBFIRST, ledMap[i]);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(5);
  }
}
