/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

 #include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Prototypes */

#define TIMER1_PRESCALE_64 2
 
#define HIGH 0
#define LOW 1

unsigned char col = 0;
unsigned char row = 0;
volatile unsigned char counter = 0;
volatile unsigned char counterPrescaler = 1;
volatile unsigned char counterP = 2;

#define numberOfCols  6
unsigned char  cols[numberOfCols] = { 5,4, 3, 2, 1, 0 };//depending on your connections

#define numberOfRows 6
unsigned char  rows[numberOfRows] = {  8, 9, 10, 11, 14, 13  };//depending on your connections


unsigned char leds[8] ;

int counterSign = 1;
unsigned char ccc = 0;

void delay_ms( uint16_t milliseconds);

void setPattern();
void digitalWrite(unsigned char pinNum, unsigned char highLow) ;

void digitalWrite(unsigned char pinNum, unsigned char highLow) {
	if ( highLow == 0)
	{
		 switch ( pinNum ) {
case 14: PORTD &= ~_BV(PD0); break;
case 13: PORTD &= ~_BV(PD1); break;
case 12: PORTD &= ~_BV(PD2); break;//
case 11: PORTD &= ~_BV(PD3); break;
case 10: PORTD &= ~_BV(PD4); break;
			case 9: PORTD &= ~_BV(PD5); break;
			case 8: PORTD &= ~_BV(PD6); break;

			case 7: PORTB &= ~_BV(PB0); break;
			case 6: PORTB &= ~_BV(PB1); break;
			case 5: PORTB &= ~_BV(PB2); break;
			case 4: PORTB &= ~_BV(PB3); break;
			case 3: PORTB &= ~_BV(PB4); break;
			case 2: PORTB &= ~_BV(PB5); break;
			case 1: PORTB &= ~_BV(PB6); break;
			case 0: PORTB &= ~_BV(PB7); break;
 
		 }
	 }
	 else
	 {
		 switch ( pinNum ) {
case 14: PORTD |= (1<<PD0); break;
case 13: PORTD |= (1<<PD1); break;
case 12: PORTD |= (1<<PD2); break;
case 11: PORTD |= (1<<PD3); break;
case 10: PORTD |= (1<<PD4); break;
			case 9: PORTD |= (1<<PD5); break;
			case 8: PORTD |= (1<<PD6); break;

			case 7: PORTB |= (1<<PB0); break;
			case 6: PORTB |= (1<<PB1); break;
			case 5: PORTB |= (1<<PB2); break;
			case 4: PORTB |= (1<<PB3); break;
			case 3: PORTB |= (1<<PB4); break;
			case 2: PORTB |= (1<<PB5); break;
			case 1: PORTB |= (1<<PB6); break;
			case 0: PORTB |= (1<<PB7); break;

		 }
	 }
}

void setPattern() {

	/*************************
	*
	*   for matrix manipulation of LED's, patterns
	*
	**************************/

counter+= 1;

//char cu = 1<<(counter/8);

uint8_t patternArray[6][8] = { 
		
{ 0,0,0,0,0,0,0,0}, //fancy leds small
{ 0,0,0,0,0,0,0,0}, //leds big
{ 0,0,0,0,0,0,0,0}, //long row 1
{ 0,0,0,0,0,0,0,0}, //diagonals in between long rows
{ 0,0,0,0,0,0,0,0}, //long row 2
{ 0,0,0,0,0,0,0,0},

};



unsigned char i, j, k;
/* //all lights on for the foot shoot :-)
for ( i = 0; i < 6; i++)
for ( j = 0; j < 6; j++)
	patternArray[i][j] = 1;
*/
 
if ( counter == 255 ) ccc++;

if ( ccc%7 > 0 ){

  k = counter/37;
unsigned char l = counter%5;

for ( i = l; i < l+1; i++)
for ( j = k; j < k+1; j++)
	patternArray[i][j] = 1;


}else
{

unsigned char k = counter%7;
unsigned char l = counter%5;

for ( i = l; i < l+1; i++)
for ( j = k; j < k+1; j++)
	patternArray[i][j] = 1;
}

 

 
 if ( counter > 200 ){v//hand pattern: storytelling
if ( counter%13 == 0 ) patternArray[5][2] = 1;
if ( counter%13 == 1 ) patternArray[5][4] = 1;
if ( counter%13 == 2 ) patternArray[5][5] = 1;
 if ( counter%13 == 3 ) patternArray[5][1] = 1;
 if ( counter%13 == 5 ) patternArray[5][1] = 1;
 if ( counter%13 == 7 ) patternArray[5][1] = 1;
}
 
	/*************************
	*
	*   using the double array for a pattern
	*
	**************************/

//unsigned char i,j;

	for ( i = 0; i < numberOfRows; i++)
		leds[i] = 0xff;
 
	for ( j = 0; j < numberOfRows; j++)
		for (  i = 0; i < numberOfCols; i++)
			if ( patternArray[j][i] == 1){
				leds[j] -= (1 << ((i % numberOfRows)));
}
}


 


void delay_ms( uint16_t milliseconds)
{
   for( ; milliseconds > 0; milliseconds--)
   {
      _delay_ms( 1);
   }
}

int main (void)
{

	DDRB = 0xFF;			// Set output.
	DDRD = 0xFF;
 

	PORTD = 0x00;
	PORTB = 0x00;
 

  TCCR0B = (1 << WGM02) | counterP; //WGNM12 bit3 of tccr1b timer control register
  OCR0A = (uint16_t)200;//output compare register comined with WGM02 set
  TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
  
  	sei();
 

	 while (1)
    {
 
	counterPrescaler++;//making the flashing more variable
	setPattern();

        delay_ms( (counter % 100)*2  + 10); //creating more variation in the timing
       // delay_ms(250);
 
	 if ( counterPrescaler%20 == 0  )
	 	{
			cli();
			TCCR0B = (1 << WGM02) | counterP; //WGNM12 bit3 of tccr1b timer control register
			OCR0A = (uint16_t)200;//output compare register combined with WGM02 set
			TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
			counterP++;
  			if ( counterP > 4 ) counterP = 2;
			sei();
			delay_ms(200);
	 }
 
	}
}
 

// Interrupt routine
SIGNAL( TIMER0_COMPA_vect   )  {

	digitalWrite( rows[row], LOW );

  	row++;
  	if (row == numberOfRows)
    	row = 0;

	unsigned char col = 0;
  	for (  col = 0; col < numberOfCols; col++) {

	    if (  (leds[row]  & (1 << col )) == HIGH )
	     	digitalWrite( cols[col], LOW);  // Turn on this led
	    else
	     	digitalWrite( cols[col], HIGH); // Turn off this led
	  	}
 
	digitalWrite( rows[row],  HIGH);
}

 

