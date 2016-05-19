#include "hcsr04.h"

unsigned static int prev_capture = 0;

ISR(TIMER1_CAPT_vect) 
{	
	unsigned int current_capture = 0;	
	printf("foo\n");
}