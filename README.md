# Star Tracker

A "barn door" star tracker implementation, using a stepper motor to match Earth's rotation speed.

It is based on an Arduino board, a 28BYJ-48 stepper motor with a ULN2003 driver, and a joystick module to set the mode of operation.

## Operation modes

Three operation modes are available:
  1. Tracking mode: this mode uses the constant speed, calculated using the inserted parameters in the code, to match Earth's rotation speed and maintain stars fixed in the camera objective.
  2. Rewind mode: resets the mechanism's position using the max allowed speed configured for the stepper motor.
  3. Stop mode: it maintains the mechanism fixed.
