/*
 *  Interrupt and PWM utilities for 16 bit Timer3 and on ATmega168/328/2560
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified Oct 2009 by Dan Clemens to work with timer3 of the ATMega1280 or Arduino Mega
 *  Modified Feb 2014 by Davide "Deid" Ferrero to port the code from Timer3 to Timer5 on ATMega2560
 *  Modified Mar 2014 by Jamie Smith to create a general-purpose Timer class
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // 16-bit timers

//WARNING: DO NOT use this class for the 8-bit timers (on the atmega2560) 0 and 2!!!
class Timer
{
  public:

	typedef volatile uint16_t * avr_port_ptr_large;
	typedef volatile uint8_t * avr_port_ptr_small;

	avr_port_ptr_large _icr;
	avr_port_ptr_small _tccra;
	avr_port_ptr_small _tccrb;

	avr_port_ptr_large _ocra;
	avr_port_ptr_large _ocrb;
	avr_port_ptr_large _ocrc;

    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
    void (*isrCallback)();



    // methods

    //example: Timer(&ICR5, &TCCR5A, &TCCR5B, &OCR5A, &OCR5B, &OCR5C)
    explicit Timer(avr_port_ptr_large icr, avr_port_ptr_small tccra, avr_port_ptr_small tccrb, avr_port_ptr_large ocra, avr_port_ptr_large ocrb, avr_port_ptr_large ocrc);

    void initialize(long microseconds=1000000);
    void start();
    void stop();
    void restart();
    void pwm(char pin, int duty, long microseconds=-1);
    void disablePwm(char pin);
    void attachInterrupt(void (*isr)(), long microseconds=-1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void setPwmDuty(char pin, int duty);
};

extern Timer Timer5;