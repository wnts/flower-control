#include <stdio.h>
#include <avr/io.h>
#include "adc.h"

int adc_read(uint16_t * result)
{
	int ret = ADC_ERR_SUCCESS;	
	
	*result = 0;
		
	ret = adc_conversion_start();
	
	if(ret != ADC_ERR_SUCCESS)
		return ret;	
	while(!(ADCSRA & (1 << ADIF)))
		;
	// clear ADIF by writing 1 to it
	ADCSRA |= (1 << ADIF);
	// get conversion result	
	*result = ADC;

	return ret;	
}