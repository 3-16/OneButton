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
  int debounceTicks; // number of ticks for debounce times.
  int clickTicks; // number of ticks that have to pass by before a click is detected
  int pressTicks; // number of ticks that have to pass by before a long button press is detected
  
  // These variables will hold functions acting as event source.
  callbackFunction onClick;
  callbackFunction onDoubleClick;
  callbackFunction onLongPressStart;
  callbackFunction onLongPressStop;
  callbackFunction onDuringLongPress;
  
  // ----- Constructor -----
  OneButton(int pin, int active);
  
  // ----- State machine functions -----

  // call this function every some milliseconds for handling button events.
  void tick(void);
  bool isLongPressed();

private:
  int _pin;        // hardware pin number. 
  int _buttonReleased;
  int _buttonPressed;

  bool _isLongPressed;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the tick function is called.
  int _state;
  unsigned long _startTime; // will be set in state 1
  unsigned long _stopTime; // will be set in state 2
};

#endif


