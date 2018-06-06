Arduino OneButton Library
===

[This fork](https://github.com/acropup/OneButton) of [Matthias Hertel's OneButton Library](https://github.com/mathertel/OneButton) is functionally similar, except the code has been refactored to use an Enum and switch statement as a state machine, and many of the trivial class members are made public, thus reducing the library's code size by ~30%. The result for users of the library is that if you previously wrote this:
```C
button.attachDoubleClick(doubleclick);
```
You should now write this:
```C
button.onDoubleClick = doubleclick;
```
---

This Arduino libary is improving the usage of a singe button for input.
It shows how to use an digital input pin with a single pushbutton attached
for detecting some of the typical button press events like single clicks, double clicks and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers the hardware invests.

This is also a sample for implementing simple finite-state machines by using the simple pattern above. 

You can find more detail on this library at
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
