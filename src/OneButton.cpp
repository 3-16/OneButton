// -----
// OneButton.cpp - Library for detecting button clicks, doubleclicks and long press pattern on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// Changelog: see OneButton.h
// -----

#include "OneButton.h"

// ----- Initialization and Default Values -----

OneButton::OneButton(int pin, int inactivePinState)
{
  _pin = pin;                           // pin that the button is connected to
  _inactivePinState = inactivePinState; // whether pin is HIGH or LOW when button is not pressed
  
  debounceMs = 50;     // number of milliseconds delayed for debounce times
  clickMs = 600;       // maximum number of milliseconds between double-clicks (and delay before single click confirmed)
  pressMs = 1000;      // minimum number of milliseconds to count as a long button press
 
  _state = NotPressed; // starting with state NotPressed: waiting for button to be pressed

  if (inactivePinState) {
    // pin is normally HIGH
    // the button connects the input pin to GND (LOW) when pressed
    // use the given pin as input and activate internal PULLUP resistor
    pinMode(pin, INPUT_PULLUP);
  } else {
    // pin is normally LOW
    // the button connects the input pin to VCC (HIGH) when pressed
    // use the given pin as input
    pinMode(pin, INPUT);
  }

  // no functions attached yet
  onClick = NULL;
  onDoubleClick = NULL;
  onLongPressStart = NULL;
  onLongPressStop = NULL;
  onDuringLongPress = NULL;
} // OneButton

// function to get the current long pressed state
inline bool OneButton::isLongPressed(){
  return _state == LongPressing;
}

void OneButton::tick(void)
{
  int buttonPressed = digitalRead(_pin) ^ _inactivePinState; // current button state
  unsigned long now = millis(); // current (relative) time in msecs

  // Implementation of the state machine
  switch(_state) {
    case NotPressed: // waiting for menu pin being pressed.
      if (buttonPressed) {
        _state = Pressing; // step to state Pressing
        _startTime = now; // remember starting time
      }
      break;
    case Pressing: // waiting for menu pin being released.
      if (!buttonPressed) {
        if((unsigned long)(now - _startTime) < debounceMs) {
          // button was released too quickly so I assume some debouncing.
          // go back to state NotPressed without calling a function.
          _state = NotPressed;
        }
        else {
          _state = SingleClicked; // step to state SingleClicked
          _stopTime = now; // remember stopping time
        }
      }
      else if (buttonPressed && ((unsigned long)(now - _startTime) > pressMs)) {
        _state = LongPressing; // step to state LongPressing
        if (onLongPressStart) onLongPressStart();
        if (onDuringLongPress) onDuringLongPress();
      }
      break;
    case SingleClicked: // waiting for second press or timeout
      if (onDoubleClick && (unsigned long)(now - _startTime) < clickMs) {
        if (buttonPressed && ((unsigned long)(now - _stopTime) > debounceMs)) {
          _state = ClickAndPressing; // step to state ClickAndPressing
          _startTime = now; // remember starting time
        } else {
          // this was only a single short click
          if (onClick) onClick();
          _state = NotPressed; // restart
        }
      }
      break;
    case ClickAndPressing: // waiting for menu pin being released finally.
      // Stay here for at least debounceMs because else we might end up in state 1 if the
      // button bounces for too long.
      if (!buttonPressed && ((unsigned long)(now - _startTime) > debounceMs)) {
        // this was a 2 click sequence.
        if (onDoubleClick) onDoubleClick();
        _state = NotPressed; // restart.
      }
      break;
    case LongPressing: // waiting for menu pin being release after long press.
      if (!buttonPressed) {
        _state = NotPressed; // restart.
        if (onLongPressStop) onLongPressStop();
        else if (onClick && onLongPressStart == NULL && onDuringLongPress == NULL) {
          //There was no onLongPress event registered, so treat it as a single click
          onClick();
        }
      } else {
        // button is being long pressed
        if (onDuringLongPress) onDuringLongPress();
      }
      break;
  }
} // OneButton.tick()


// end.

