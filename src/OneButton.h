// -----
// OneButton.h - Library for detecting button clicks, doubleclicks and long press pattern on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 transformed into a library
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// 23.03.2014 Enhanced long press functionalities by adding longPressStart and longPressStop callbacks
// 21.09.2015 A simple way for debounce detection added.
// 14.05.2017 Debouncing improvements.
// -----

#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}


class OneButton
{
public:
  int debounceMs; // number of milliseconds delayed for debounce times
  int clickMs;    // maximum number of milliseconds between double-clicks (and delay before single click confirmed)
  int pressMs;    // minimum number of milliseconds to count as a long button press
  
  // Assignable callback functions for each kind of button event
  callbackFunction onClick;
  callbackFunction onDoubleClick;
  callbackFunction onLongPressStart;
  callbackFunction onLongPressStop;
  callbackFunction onDuringLongPress;
  
  // ----- Constructor -----
  // pin - pin number that the button is connected to
  // inactivePinState - whether pin is HIGH or LOW when button is not pressed
  OneButton(int pin, int inactivePinState);
  
  // ----- State machine functions -----
  // call tick() function every some milliseconds for handling button events.
  void tick(void);
  inline bool isLongPressed();

private:
  int _pin;              // hardware pin number. 
  int _inactivePinState; // whether pin is HIGH or LOW when button is not pressed

  // Button state machine states
  enum ButtonState {
    NotPressed,
    Pressing,
    SingleClicked,
    ClickAndPressing,
    LongPressing
  };
  // These variables hold data for button state machine
  ButtonState _state;
  unsigned long _startTime;
  unsigned long _stopTime;
};

#endif


