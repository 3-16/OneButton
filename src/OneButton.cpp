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

OneButton::OneButton(int pin, int activeLow)
{
  _pin = pin;

  debounceTicks = 50;      // number of millisec that have to pass by before a click is assumed as safe.
  clickTicks = 600;        // number of millisec that have to pass by before a click is detected.
  pressTicks = 1000;       // number of millisec that have to pass by before a long button press is detected.
 
  _state = NotPressed; // starting with state NotPressed: waiting for button to be pressed
  _isLongPressed = false;  // Keep track of long press state

  if (activeLow) {
    // the button connects the input pin to GND when pressed.
    _buttonReleased = HIGH; // notPressed
    _buttonPressed = LOW;

    // use the given pin as input and activate internal PULLUP resistor.
    pinMode(pin, INPUT_PULLUP);

  } else {
    // the button connects the input pin to VCC when pressed.
    _buttonReleased = LOW;
    _buttonPressed = HIGH;

    // use the given pin as input
    pinMode(pin, INPUT);
  }

  // no functions attached yet: clear all function pointers.
  onClick = NULL;
  onDoubleClick = NULL;
  onLongPressStart = NULL;
  onLongPressStop = NULL;
  onDuringLongPress = NULL;
} // OneButton

// function to get the current long pressed state
bool OneButton::isLongPressed(){
  return _isLongPressed;
}

void OneButton::tick(void)
{
  // Detect the input information 
  int buttonLevel = digitalRead(_pin); // current button signal.
  unsigned long now = millis(); // current (relative) time in msecs.

  // Implementation of the state machine
  switch(_state) {
    case NotPressed: // waiting for menu pin being pressed.
      if (buttonLevel == _buttonPressed) {
        _state = Pressing; // step to state Pressing
        _startTime = now; // remember starting time
      }
      break;
    case Pressing: // waiting for menu pin being released.
      if (buttonLevel == _buttonReleased) {
        if((unsigned long)(now - _startTime) < debounceTicks) {
          // button was released too quickly so I assume some debouncing.
          // go back to state NotPressed without calling a function.
          _state = NotPressed;
        }
        else {
          _state = SingleClicked; // step to state SingleClicked
          _stopTime = now; // remember stopping time
        }
      }
	  else if ((buttonLevel == _buttonPressed) && ((unsigned long)(now - _startTime) > pressTicks)) {
        if (onLongPressStart || onDuringLongPress || onLongPressStop) {
          _isLongPressed = true;  // Keep track of long press state
          if (onLongPressStart) onLongPressStart();
          if (onDuringLongPress) onDuringLongPress();
          _state = LongPressing; // step to state LongPressing
        }
      }
      break;
    case SingleClicked: // waiting for menu pin being pressed the second time or timeout.
      if (onDoubleClick == NULL || (unsigned long)(now - _startTime) > clickTicks) {
        // this was only a single short click
        if (onClick) onClick();
        _state = NotPressed; // restart.
   
      } else if ((buttonLevel == _buttonPressed) && ((unsigned long)(now - _stopTime) > debounceTicks)) {
        _state = ClickAndPressing; // step to state ClickAndPressing
        _startTime = now; // remember starting time
      }
      break;
    case ClickAndPressing: // waiting for menu pin being released finally.
      // Stay here for at least debounceTicks because else we might end up in state 1 if the
      // button bounces for too long.
      if (buttonLevel == _buttonReleased && ((unsigned long)(now - _startTime) > debounceTicks)) {
        // this was a 2 click sequence.
        if (onDoubleClick) onDoubleClick();
        _state = NotPressed; // restart.
      }
      break;
    case LongPressing: // waiting for menu pin being release after long press.
      if (buttonLevel == _buttonReleased) {
        _isLongPressed = false;  // Keep track of long press state
        if(onLongPressStop) onLongPressStop();
        _state = NotPressed; // restart.
      } else {
        // button is being long pressed
        _isLongPressed = true; // Keep track of long press state
        if (onDuringLongPress) onDuringLongPress();
      }
      break;
  }
} // OneButton.tick()


// end.

