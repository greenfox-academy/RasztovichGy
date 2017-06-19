#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "freq_meas.h"

// TODO:
// Write the interrupt handlers

void freq_meas_init()
{
	/**************
	 * TC1 CONFIG *
	 **************/
	// TODO:
	// Configure the TC1 timer properly :)
/* DS p170 Input capture unit
feladat: TC1 timert be�ll�tani
hint: tegnapi fPWM feladatban a TC0 timert �ll�tottuk be!
TC1 timer regisztere: TCR1A �s TCR1B, ezekbe be kell �ll�tani a 
compare output mode-ot, a Waveform  generator mode-ot �s a clock prescalert

*/
//Compare output mode @ Fast PWM (table 21-4) -> Clear OC1A/OC1B on Compare Match, 
// set OC1A/OC1B at BOTTOM (non-inverting mode)
	TCCR1A |= ( 1 << COM1A1 ) | ( 1 << COM1B1 );

//Waveform generation mode set. (table 21-6)
/* When a change of the logic level (an event) occurs on the Input Capture pin (ICP1), (..) a capture
will be triggered: the 16-bit value of the counter (TCNT1) is written to the Input Capture Register (ICR1).
Azaz, a 21-6 table legyen Fast PWM �s TOP = ICR1 m�d, ami a 14-es */
	
	TCCR1A |= ( 1 << WGM11 );
	TCCR1B |= ( 1 << WGM13 ) | ( 1 << WGM12 );
*/

// clock set to clkio/1024, mint a PWM-es feladatban, nem tudom mi�rt �pp /1024, 
// gondolom hogy min�l ink�bb lass�tsuk az �rajelet
	TCCR1B |= 1 << CS12;
	TCCR1B |= 1 << CS10;

// kell egy input capture kimenet - ez a PB0, ez default
// kell egy kimenet a PD6-on
	DDRD |= 1 << DDRD6;
}

// TODO:
// Write this function. It returns the measured frequency in Hz

//interruptok
//megszak�t�s: timer ovrflow (h�nyszor �ri el a 65535-�t a TCNT sz�ml�l�
// m�g egy megszak�t�s: timer capture event, azaz amikor ICP1 elindul felfele
// megszak�t�s: week 2 day 3 - Functions, interrupts and timers t�mak�r. 


uint16_t prev; // ez a TCNT �rt�ke amikor ICP1 1-re v�lt
uint16_t last; // ez TCNT �rt�ke amikor ICP1 miut�n lement null�ra �jra 1-re v�lt.
uint16_t cntr = 0; // Create a variable which will be a counter variable, initialize it with 0.
const uint16_t cntr_max = 65535; // This will be the maximal value of our counter. 
uint16_t counter_ovf; //ez a v�ltoz� sz�molja majd azt, h�nyszor �ri el a TCNT sz�ml�l� a plafon 65535-�t	

ISR(TIMER1_OVF_vect)
{
	// sz�moljuk meg, h�ny overflow t�rt�nik
	// �s azt t�roljuk el egy sz�mba, k�s�bb abb�l sz�moljuk a T peri�dusd�t
	if (cntr < cntr_max) { //nem kellene egy for ciklus ahhoz hogy a cntr 1-t�l elinduljon?
		cntr++;
		} 
	counter_ovf = cntr; 
}

ISR(TIMER1_CAPT_vect)
{
	// sz�moljuk meg, h�ny overflow t�rt�nik
	// �s azt t�roljuk el egy sz�mba, k�s�bb abb�l sz�moljuk a T peri�dusd�t
	if (cntr < cntr_max) { //nem kellene egy for ciklus ahhoz hogy a cntr 1-t�l elinduljon?
		cntr++;
	}
uint16_t counter_ovf = cntr; //ez a v�ltoz� sz�molja majd azt, h�nyszor �ri el a TCNT sz�ml�l� a plafon 65535-�t	}
}





float get_freq()
{
	
/*
3 glob�lis v�ltoz� kell, az interruptokat sz�molja kett�,
a 3. 
*/	
	
	uint16_t temp = ICR1;
	
}
