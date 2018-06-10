#include "TrinketHidCombo.h"

int irPin = 2; //Sensor pin connect to digital pin2 (ATINY85 pin7)
int ledPin = 1;
int start_bit = 2200; //Start bit threshold (Microseconds)
int bin_1 = 1000; //Binary 1 threshold (Microseconds)
int bin_0 = 400; //Binary 0 threshold (Microseconds)
const byte BIT_PER_BLOCK = 32;

void setup() {
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  TrinketHidCombo.begin();
  TrinketHidCombo.poll();
}

void loop() {
  int key = getIRKey();        //Fetch the key
  if (key != 0 && key != -1) //Ignore keys that are zero
  {
    switch (key)
    {
      case 13: TrinketHidCombo.print("0"); break;
      case 12: TrinketHidCombo.print("1"); break;
      case 24: TrinketHidCombo.print("2"); break;
      case 94: TrinketHidCombo.print("3"); break;
      case 8: TrinketHidCombo.print("4"); break;
      case 28: TrinketHidCombo.print("5"); break;
      case 90: TrinketHidCombo.print("6"); break;
      case 66: TrinketHidCombo.print("7"); break;
      case 82: TrinketHidCombo.print("8"); break;
      case 74: TrinketHidCombo.print("9"); break;
      case 71: TrinketHidCombo.pressKey(0, KEYCODE_ENTER); TrinketHidCombo.pressKey(0, 0); break;
      case 69: TrinketHidCombo.pressKey(0, KEYCODE_ESC); TrinketHidCombo.pressKey(0, 0); break;
      case 68: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT); break;
      case 67: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT); break;
      case 64: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_DOWN); break;
      case 70: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_UP); break;
      case 25: TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP); break;
      case 22: TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN); break;
      case 7: TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK); break;
      case 21: TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK); break;
      case 9: TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE); break;

    }
  }
}

/////////////////////////////////////////////////////////////
// decode infrared signal
/////////////////////////////////////////////////////////////
int getIRKey() {
  int data[BIT_PER_BLOCK];
  int i;
  while (pulseIn(irPin, HIGH) < start_bit) { //Wait for a start bit
    TrinketHidCombo.pressKey(0, 0);
//    TrinketHidCombo.poll();
  }

  digitalWrite(ledPin, HIGH);

  for (i = 0 ; i < BIT_PER_BLOCK ; i++)
    data[i] = pulseIn(irPin, HIGH); //Start measuring bits, I only want HIGH pulses

  delay(60);
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
