/*! @defgroup timer Librería gestión temporizador
 */
/*@{*/

#ifndef TIMER_H
#define TIMER_H
#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>

/** Concats two numbers in a string-like fashion */
uint32_t concat(uint16_t x, uint16_t y);

/** Calculates and returns the concatenation of the expanded timer
 * with the microcontroller timer register */
uint32_t geticks();

/** Returns ticks conversion in microseconds */
uint32_t getmicros();

/** Returns ticks convertion in milliseconds */
uint32_t getmilis();

/** Generates a time-based delay, time must be milliseconds*/
void delayms(uint32_t time);

/** Generates a time-based delay, time must be microseconds*/
void delayus(uint32_t time);

/** Waits for a time-based delay, then calls a function , time must be in microseconds*/
void future_f(void (*f)(void), uint32_t time);

/** Calls a function periodically, to stop it call it again with 0 as time parameter */
void periodic_f(void (*f), uint32_t time);

/** Timer library initialization */
void initialize();

#endif

/*@}*/
