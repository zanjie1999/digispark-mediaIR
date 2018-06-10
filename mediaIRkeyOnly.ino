#include "DigiKeyboard.h"

// not all keys are mapped in the DigiKeyboard.h file.
// you have to map it here
#define KEY_HOME   0x4A
#define KEY_PAGE_UP   0x4B
#define KEY_PAGE_DOWN   0x4E
#define KEY_ESCAPE   0x29
#define KEY_UP_ARROW   0x52
#define KEY_DOWN_ARROW   0x51
#define KEY_LEFT_ARROW   0x50
#define KEY_RIGHT_ARROW   0x4F

//defined by tingpan
#define KEY_ESC   0x1B
#define KEY_UP   0x26
#define KEY_DOWN   0x28
#define KEY_LEFT  0x25
#define KEY_RIGHT   0x27
#define KEY_LEFT_CLIK  0x01
#define KEY_RIGHT_CLIk   0x02

int irPin = 2; //Sensor pin connect to digital pin2 (ATINY85 pin7)
int ledPin = 1;
int start_bit = 2200; //Start bit threshold (Microseconds)
int bin_1 = 1000; //Binary 1 threshold (Microseconds)
int bin_0 = 400; //Binary 0 threshold (Microseconds)
const byte BIT_PER_BLOCK = 32;

void setup() {
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  DigiKeyboard.update(); // keep updating the keyboard
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:

  DigiKeyboard.sendKeyStroke(0);

  int key = getIRKey();        //Fetch the key
  if (key != 0 && key != -1) //Ignore keys that are zero
  {
    //    DigiKeyboard.println(key);
    switch (key)
    {
      case 13: DigiKeyboard.print("0"); break;
      case 12: DigiKeyboard.print("1"); break;
      case 24: DigiKeyboard.print("2"); break;
      case 94: DigiKeyboard.print("3"); break;
      case 8: DigiKeyboard.print("4"); break;
      case 28: DigiKeyboard.print("5"); break;
      case 90: DigiKeyboard.print("6"); break;
      case 66: DigiKeyboard.print("7"); break;
      case 82: DigiKeyboard.print("8"); break;
      case 74: DigiKeyboard.print("9"); break;
      case 71: DigiKeyboard.sendKeyStroke(KEY_ENTER); break;
      case 69: DigiKeyboard.sendKeyStroke(KEY_ESCAPE); break;
      case 68: DigiKeyboard.sendKeyStroke(KEY_LEFT_ARROW); break;
      case 67: DigiKeyboard.sendKeyStroke(KEY_RIGHT_ARROW); break;
      case 64: DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); break;
      case 70: DigiKeyboard.sendKeyStroke(KEY_UP_ARROW); break;

    }
  }
}

/////////////////////////////////////////////////////////////
// decode infrared signal
/////////////////////////////////////////////////////////////
int getIRKey() {
  int data[BIT_PER_BLOCK];
  int i;
  while (pulseIn(irPin, HIGH) < start_bit); //Wait for a start bit

  digitalWrite(ledPin, HIGH);

  for (i = 0 ; i < BIT_PER_BLOCK ; i++)
    data[i] = pulseIn(irPin, HIGH); //Start measuring bits, I only want HIGH pulses

  delay(50);
  for (i = 0 ; i < BIT_PER_BLOCK ; i++) //Parse them
  {
    if (data[i] > bin_1) //is it a 1?
      data[i] = 1;
    else if (data[i] > bin_0) //is it a 0?
      data[i] = 0;
    else {
      digitalWrite(ledPin, LOW);
      return -1; //Flag the data as invalid; Return -1 on invalid data
    }
  }

  //based on NEC protocol, command data started from bit 16
  //and end with bit 24 (8 bits long)
  int result = 0;
  for (i = 16 ; i < 24; i++) {
    //DigiKeyboard.print(data[i]); //print out the value of button in binary form
    if (data[i] == 1) result |= (1 << i - 16); // convert="" data="" bits="" to="" integer
  }

  digitalWrite(ledPin, LOW);

  return result; //Return key number
}
