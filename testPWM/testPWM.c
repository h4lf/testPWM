/*
 * testPWM.c
 *
 * Created: 04.08.2016 0:45:20
 *  Author: UB4LAG Vasily Afanasyev
 
 The MIT License (MIT)

 Copyright (c) 2016 Vasily Afanasyev

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#include "testPWM.h"
#include "pin_macros.h"

volatile struct
{
	unsigned tmr1ovf	: 1;
	} IsrFlag;

ISR (TIMER1_OVF_vect)
{
	IsrFlag.tmr1ovf = 1;
}

void initial_periphery(void)
{
	//------Disable comparator
	ACSR = 0x80;
	
	//------Init GPIO
	DRIVER(PinLed,OUT);
	DRIVER(PinPWMa,OUT);
	DRIVER(PinPWMb,OUT);
	
	//------Init TIM1
	TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // non-inverting mode; mode = 14 (Fast PWM)
	TCCR1C = 0;
	TCNT1 = 0;
	ICR1 = 30000U;
	OCR1A = 0;
	OCR1B = UINT16_MAX;
	TIMSK1 = (1 << TOIE1);
	TCCR1B = (1 << CS11) | (1 << WGM13) | (1 << WGM12); // start with clk/8; mode = 14 (Fast PWM)
}

int main(void)
{
	
	initial_periphery();
	sei();
	
    while(1)
    {
        if (IsrFlag.tmr1ovf)
        {
			static uint16_t DutyCycle1A = 0, DutyCycle1B = UINT16_MAX;
			
			cli();
			IsrFlag.tmr1ovf = 0;
			sei();
			DutyCycle1A += 1024U;
			DutyCycle1B -= 1024U;
			OCR1A = DutyCycle1A;
			OCR1B = DutyCycle1B;
			TOGGLE(PinLed);
        }
    }
}