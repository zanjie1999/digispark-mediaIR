#include "TrinketHidCombo.h"

const int irPin = 2; //Sensor pin connect to digital pin2 (ATINY85 pin7)
const int ledPin = 1;
const int start_bit = 2200; //Start bit threshold (Microseconds)
const int bin_1 = 1000; //Binary 1 threshold (Microseconds)
const int bin_0 = 400; //Binary 0 threshold (Microseconds)
const byte BIT_PER_BLOCK = 32;
int flag = 1;  //flag of media key

void setup() {
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  TrinketHidCombo.begin();
  TrinketHidCombo.pressKey(0, 0);
  TrinketHidCombo.pressKey(0, 0);
}

void loop() {
  int key = getIRKey();        //Fetch the key
  if (key > 0 ) {          //Ignore keys that are zero
    sendKey(key);
    //    delay(10);
    if (pulseIn(irPin, HIGH) > start_bit) {
      int num = 0;
      while (pulseIn(irPin, HIGH) > start_bit) {
        if (num > 5) {
//          digitalWrite(ledPin, LOW);
          if (flag) {
            sendKey(key);
          }
          digitalWrite(ledPin, HIGH);
        }
        num++;
      }
      digitalWrite(ledPin, LOW);
    }
    if (!flag) {
      TrinketHidCombo.pressKey(0, 0);
    }
  }

}

void sendKey(int key) {
  flag = 0;
  switch (key)
  {
    case 13: TrinketHidCombo.pressKey(0, KEYCODE_0); break;
    case 12: TrinketHidCombo.pressKey(0, KEYCODE_1); break;
    case 24: TrinketHidCombo.pressKey(0, KEYCODE_2); break;
    case 94: TrinketHidCombo.pressKey(0, KEYCODE_3); break;
    case 8: TrinketHidCombo.pressKey(0, KEYCODE_4); break;
    case 28: TrinketHidCombo.pressKey(0, KEYCODE_5); break;
    case 90: TrinketHidCombo.pressKey(0, KEYCODE_6); break;
    case 66: TrinketHidCombo.pressKey(0, KEYCODE_7); break;
    case 82: TrinketHidCombo.pressKey(0, KEYCODE_8); break;
    case 74: TrinketHidCombo.pressKey(0, KEYCODE_9); break;
    case 71: TrinketHidCombo.pressKey(0, KEYCODE_ENTER); break;
    case 69: TrinketHidCombo.pressKey(0, KEYCODE_ESC); break;
    case 68: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_LEFT); break;
    case 67: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_RIGHT); break;
    case 64: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_DOWN); break;
    case 70: TrinketHidCombo.pressKey(0, KEYCODE_ARROW_UP); break;
    case 25: TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP); flag=1; break;
    case 22: TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN); flag=1; break;
    case 7: TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK); flag=1; break;
    case 21: TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK); flag=1; break;
    case 9: TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE); flag=1; break;
  }
  if (flag) {
    TrinketHidCombo.pressKey(0, 0);
  }
}

/////////////////////////////////////////////////////////////
// decode infrared signal
/////////////////////////////////////////////////////////////
int getIRKey() {
  int data[BIT_PER_BLOCK];
  int i;
  while (pulseIn(irPin, HIGH) < start_bit) { //Wait for a start bit
    //    TrinketHidCombo.pressKey(0, 0);
    TrinketHidCombo.poll();
  }

  digitalWrite(ledPin, HIGH);

  for (i = 0 ; i < BIT_PER_BLOCK ; i++) //Parse them
  {
    int tmp = pulseIn(irPin, HIGH);
    if (tmp > bin_1) //is it a 1?
      data[i] = 1;
    else if (tmp > bin_0) //is it a 0?
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
