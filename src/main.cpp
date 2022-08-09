#include <Arduino.h>

// MuteMe needs to send 0x01 - 0x02 to the PC depending on button state
// We _could_ create a new keyboard layout and mapping, but it is easier
// to just reuse the unused mappings already defined here: 
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L300-L304
// Additional information: https://github.com/NicoHood/HID/wiki/Keyboard-API#keyboard-layouts
// HID_KEYBOARD_ERROR_ROLLOVER = 0x01,
// HID_KEYBOARD_POST_FAIL  = 0x02,
// HID_KEYBOARD_ERROR_UNDEFINED    = 0x03,
// HID_KEYBOARD_A_AND_A    = 0x04,
// HID_KEYBOARD_B_AND_B    = 0x05,

#include "HID-Project.h"
#include <HID-Settings.h>
#include <Button.h>
#include <RGBLed.h>

// Pin Definitions
#define PUSHBUTTON_PIN_2	2
#define RGBLED_PIN_B	3
#define RGBLED_PIN_G	5
#define RGBLED_PIN_R	6

// Global variables and defines
#define rgbLed_TYPE COMMON_ANODE
// object initialization
Button pushButton(PUSHBUTTON_PIN_2);
RGBLed rgbLed(RGBLED_PIN_R,RGBLED_PIN_G,RGBLED_PIN_B,rgbLed_TYPE);

// The size of the buffer for the HID device, 64 byte max
// https://github.com/NicoHood/HID/issues/133#issuecomment-533946021
// ************************ Note this doesn't work ****************
// Able to receive data, but not send it back to the PC https://github.com/NicoHood/HID/issues/133
// https://arduino.stackexchange.com/questions/78880/send-custom-hid-values-instead-of-keys
// https://learn.sparkfun.com/tutorials/hid-control-of-a-web-page/all

uint8_t rawhidData[255];

void setup()
{
  // fill rawhdidData with 0s
  // for (int i = 0; i < 255; i++) {
  //   rawhidData[i] = 0;
  // }
  pushButton.init();
  rgbLed.turnOff();
  // pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(9600);
  Keyboard.begin();
  RawHID.begin(rawhidData, sizeof(rawhidData));
}
void loop()
{
  // if button.onpress == true
  if(pushButton.onPress())
  {
    // rgbLed.setRGB(50,50,50);
    // rawhidData[0] = 0x00; 
    // rawhidData[1] = 0x04;
    // Serial.print("uploading data: ");
    // Serial.print(rawhidData[0]);
    // Serial.print(",");
    // Serial.print(rawhidData[1]);
    // RawHID.write(rawhidData, sizeof(rawhidData));
    // Serial.println("Sending 0x04 to pc");
    // Keyboard.write(HID_CONSUMER_MICROPHONE_CA);
    // delay(1000);
    // Keyboard.write(HID_KEYBOARD_POST_FAIL);
    // // Keyboard.write(0x00);
    // Keyboard.releaseAll();

    // https://arduino.stackexchange.com/questions/78880/send-custom-hid-values-instead-of-keys
    // uint8_t megabuff[1];
    // megabuff[0] = 0x04;
    // RawHID.write(megabuff, 1);
    uint8_t megabuff[64];
    for (uint8_t i = 0; i < sizeof(megabuff); i++) {
        megabuff[i] = 0x00;
    }
    megabuff[3] = 0x04;
    RawHID.write(megabuff, sizeof(megabuff));
    delay(100);
    megabuff[3] = 0x01;
    RawHID.write(megabuff, sizeof(megabuff));
    delay(100);
    megabuff[3] = 0x02;
    RawHID.write(megabuff, sizeof(megabuff));
    megabuff[3] = 0x00; // reset back to 0

  }

  // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    // Mirror data via Serial
    while (bytesAvailable--) {
      auto data = RawHID.read();
      // Serial.println(data);
      // red = 0x01
      if (data == 1)
      {
        rgbLed.setRGB(255,0,0);
      }
      // green = 0x02
      if (data == 2)
      {
        rgbLed.setRGB(0,255,0);
      }
      // yellow = 0x03
      if (data == 3)
      {
        rgbLed.setRGB(255,255,0);
      }
      // blue = 0x04
      if (data == 4)
      {
        rgbLed.setRGB(0,0,255);
      }
      // purple = 0x05
      if (data == 5)
      {
        rgbLed.setRGB(255,0,255);
      }
      // cyan = 0x06
      if (data == 6)
      {
        rgbLed.setRGB(0,255,255);
      }
      // white = 0x07
      if (data == 7) 
      {
        rgbLed.setRGB(255,245,255);
      }
      // noColor = 0x00
      // TODO: Clear is actually 2 bytes 0x00 0x00, not just 1
      if (data == 0)
      {
        rgbLed.setRGB(0,0,0);
        rgbLed.turnOff();
      }
    }
    // digitalWrite(LED_BUILTIN, LOW);
  }
}
