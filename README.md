# hc12-pract2

Embedded Systems project for the Motorola 68H12 microcontroller.

## Features

Current features include:

* Seven-segment display support, allowing to display integer numeric values in it.

* Potentiometer support with up to 16 bit precision.

* Numeric keyboard support.

The main program reads a number from keyboard-based input and displays it in a seven-segment array, causing a small fan to rotate with a speed value proportional to the keyboard input.

## Compiling from sources

In order to compile this software, the following programs are needed:

* `gcc-m68hc1x`

* `binutils-m68hc1x`

Run `make` in the root folder to compile both the SimpleGEL libraries necessary for the project and the source code located in `src/`.

## Documentation

This documentation can be generated with Doxygen running `make doc` in the root folder of the project.
