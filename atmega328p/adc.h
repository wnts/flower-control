#ifndef ADC_H_
#define ADC_H_

#define ADC_ERR_SUCCESS					 1
#define ADC_ERR_CONVERSION_ONGOING		-1

enum AdcVRef {AREF = 0, AVCC = 1, INTERNAL1V1 = 3};
enum AdcInput {
	ADC0,
	ADC1,
	ADC2,
	ADC3,
	ADC4,
	ADC5,
	ADC6,
	ADC7,
	ADC8,			///< ATMega Internal temperature sensor
	VBG = 14,		///< Internal bandgap voltage (1.1V)
	GND = 15		
};

enum AdcClkDiv {
	DIV2 = 0,
	DIV4 = 2,
	DIV8 = 3,
	DIV16 = 4,
	DIV32 = 5,
	DIV64 = 6,
	DIV128 = 7
};

/** 
 * Set the voltage reference for the ADC.
 *
 * @param vref	The voltage reference
 * @return		ADC_ERR_SUCCESS on success.
 *
 * @warning		Depending on the voltage connected to the AREF pin, 
 *				setting the voltage reference to AREF or AVCC might cause a short-circuit.
 *				Consult the ATMEGA datasheet for more information.
 *				Use with extreme care!
 */
inline int adc_vref_set(enum AdcVRef vref)
{
	ADMUX = (ADMUX & ~(3 << REFS0)) | vref << REFS0;
	return ADC_ERR_SUCCESS;
}

/**
 * Get the current ADC voltage reference.
 *
 * @return Currently selected ADC voltage reference.
 */
inline enum AdcVRef adc_vref_get(void)
{
	return (ADMUX & (3 << REFS0)) >> REFS0;
}

/** 
 * Set the ADC input clock divider.
 * This divider is applied to the ATMega's system clock
 * When the ADC needs to operate with a 10-bit precision, this divider should be 
 * selected so that the ADC input clock is 50khz-200khz. Values higher than
 * 200khz result in lower conversion precision.
 *
 * @param clk_div	ADC input clock divider
 *
 * @return ADC_ERR_SUCCESS on success.
 * 
 */
inline int adc_clkdiv_set(enum AdcClkDiv clk_div)
{
	ADCSRA = (ADCSRA & 0xf8) | clk_div;
	return ADC_ERR_SUCCESS;
}

/**
 * Select the ADC input channel
 * 
 * @return	ADC_ERR_SUCCESS on success.
 */ 
inline int adc_input_set(enum AdcInput adc_input)
{
	ADMUX = (ADMUX & 0xf0) | adc_input;
	return ADC_ERR_SUCCESS;
}

/**
 * Get the current ADC input channel
 * 
 * @return	The currently selected ADC input channel
 */ 
inline int adc_input_get(void)
{
	return ADMUX & 0x0f;
}

inline void adc_enable(void)
{
	ADCSRA = ADCSRA | (1 << ADEN);
}

inline void adc_disable(void)
{
	ADCSRA &= ~(1 << ADEN);
}

/**
 * Start an ADC conversion
 *
 * @return ADC_ERR_SUCCESS on success
 *		   ADC_ERR_CONVERSION_ONGOING when there already is an ongoing conversion
 */
inline int adc_conversion_start(void)
{	
	if(ADCSRA & (1 << ADSC))
		return ADC_ERR_CONVERSION_ONGOING;
	ADCSRA |= (1 << ADSC);
	return ADC_ERR_SUCCESS;
}

/**
 * Start an ADC conversion and return the result.
 * This function blocks until the ADC conversion is completed.
 *
 * @param result	Pointer to buffer that will receive the 10-bit ADC conversion result.
 * 
 * @return	ADC_ERR_SUCCESS on success
 *			ADC_ERR_CONVERSION_ONGOING when there is already an ongoing conversion
 */ 
int adc_read(uint16_t * result);





#endif /* ADC_H_ */