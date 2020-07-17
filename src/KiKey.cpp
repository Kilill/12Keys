// Do not remove the include below
#include <Arduino.h>
#define USE_TEENSY_KEYBOARD
#include "HID-Project.h"
#include "Keypad.h"

const uint8_t Rows=4;
const uint8_t Cols=4;
const uint8_t Keys[Rows][Cols]=
    { HID_KEYBOARD_F13, HID_KEYBOARD_F14, HID_KEYBOARD_F15, HID_KEYBOARD_F16,
      HID_KEYBOARD_F17, HID_KEYBOARD_F18, HID_KEYBOARD_F19, HID_KEYBOARD_F20,
      HID_KEYBOARD_F21, HID_KEYBOARD_F22, HID_KEYBOARD_F23, HID_KEYBOARD_F24,
      HID_KEYBOARD_F21, HID_KEYBOARD_F22, HID_KEYBOARD_F23, HID_KEYBOARD_F24
    };
/*
const uint8_t Keys[Rows][Cols]={
  { KEY_A, KEY_B, KEY_C, KEY_D },
  { KEY_E, KEY_F, KEY_G, KEY_H },
  { KEY_I, KEY_J, KEY_K, KEY_L }
 };

*/
const int DEBOUNCE_TIME=50;
enum keyState {Pressed = 0,Released =1 };
uint8_t ColPins[]={2,3,4,5};    //Outputs, Keypad uses the cols as output
uint8_t RowPins[]={6,7,8};      // Inputs

const uint8_t HBPin=10;             // Heart Beat, debug pin in loop
const uint8_t KeyDetectPin=16;      // every time a key as been detected

uint8_t hb=0;

Keypad keypad = Keypad( makeKeymap(Keys), RowPins, ColPins, Rows, Cols );

//The setup function is called once at startup of the sketch

// Taking care of some special events.
void keypadEvent(KeypadEvent key, KeyState kpadState ){
	static char format[80];
    switch (kpadState){
    case PRESSED:
    	Keyboard.press((KeyboardKeycode) key);
        digitalWrite(KeyDetectPin,1);
		sprintf(format,"Key Pressed:%0x (%c)",key,key); 
		Serial.println(format);
        break;

    case RELEASED:
    	Keyboard.release((KeyboardKeycode) key);
        digitalWrite(KeyDetectPin,0);
		sprintf(format,"Key Released:%0x (%c)",key,key); 
		Serial.println(format);
        break;

    case HOLD:
		sprintf(format,"Key Holde:%0x (%c)",key,key); 
		Serial.println(format);
    case IDLE:
        break;
    }
}

void setup()
{
	pinMode(HBPin,OUTPUT);
	pinMode(KeyDetectPin,OUTPUT);

	// Sends a clean report to the host. This is important on any Arduino type.
	Keyboard.begin();
    keypad.addStatedEventListener(keypadEvent); // Add an event listener for this keypad
	Serial.begin(38400);
	Serial.println("Starting up");

}

// The loop function is called in an endless loop
void loop()
{
	keypad.getKey();
    digitalWrite(KeyDetectPin,hb);
    hb =! hb;
}
