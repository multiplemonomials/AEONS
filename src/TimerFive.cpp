/*
 *  Interrupt and PWM utilities for 16 bit Timer4 and on ATmega168/328/2560
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified Oct 2009 by Dan Clemens to work with timer3 of the ATMega1280 or Arduino Mega
 *  Modified Mar 2014 by Jamie Smith to create a general-purpose Timer class
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include "Timer.h"

void Timer::initialize(long microseconds)
{
  TCCR5A = 0;                 // clear control register A 
  TCCR5B = _BV(WGM53);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}

void Timer::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS50);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS51);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS51) | _BV(CS50);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS52);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS52) | _BV(CS50);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS52) | _BV(CS50);  // request was out of bounds, set as maximum
  *icr = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  *tccrb &= ~(_BV(CS50) | _BV(CS51) | _BV(CS52));
  *tccrb |= clockSelectBits;                                                     // reset clock select register
}

void Timer::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 46) OCR5A = dutyCycle;
  if(pin == 44) OCR5B = dutyCycle;
  if(pin == 45) OCR5C = dutyCycle;
}

void Timer::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  
	// sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 46) { DDRL |= _BV(PORTL3); TCCR5A |= _BV(COM5A1); }
  if(pin == 44) { DDRL |= _BV(PORTL4); TCCR5A |= _BV(COM5B1); }
  if(pin == 45) { DDRL |= _BV(PORTL5); TCCR5A |= _BV(COM5C1); }
  setPwmDuty(pin, duty);
  start();
}

void Timer::disablePwm(char pin)
{
  if(pin == 46) TCCR5A &= ~_BV(COM5A1);   // clear the bit that enables pwm on PE3
  if(pin == 44) TCCR5A &= ~_BV(COM5B1);   // clear the bit that enables pwm on PE4
  if(pin == 45) TCCR5A &= ~_BV(COM5C1);   // clear the bit that enables pwm on PE5
}

void Timer::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK5 = _BV(TOIE5);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}

void Timer::detachInterrupt()
{
  TIMSK5 &= ~_BV(TOIE5);                                   // clears the timer overflow interrupt enable bit 
}

void Timer::start()
{
  TCCR5B |= clockSelectBits;
}

void Timer::stop()
{
  TCCR5B &= ~(_BV(CS50) | _BV(CS51) | _BV(CS52));          // clears all clock selects bits
}

void Timer::restart()
{
  TCNT5 = 0;
}
