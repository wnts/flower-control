#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "uart.h"
#include "spi.h"
#include "log.h"
#include "nrf.h"
#include "interrupt.h"

volatile int g_int;

ISR(INT0_vect)
{		
	uint8_t irq = GPIO_PIN_GET(B, 0);
	GPIO_PIN_SET(B, 1, 0);			
	printf("IRQ = %d\n", irq);	
	uint8_t status = nrf_reg8_read(NRF_REG_STATUS);
	printf("status = %02x\n", status);	
	if(status & (1 << MAX_RT))
	{
		printf("max rt\n");
		nrf_reg8_bit_set(NRF_REG_STATUS, MAX_RT, 1);
	}
	if (status & (1 << RX_DR))
	{
		printf("rx\n");
		nrf_reg8_bit_set(NRF_REG_STATUS, RX_DR, 1);
	}
	if (status & (1 << TX_DS))
	{
		printf("tx\n");
		nrf_reg8_bit_set(NRF_REG_STATUS, TX_DS, 1);
	}
	g_int = 1;		
}



ISR(BADISR_vect)
{
	printf("default int vector!\n");
}

int main(void)
{
	uint16_t result = 0;

	log_init();	
	adc_enable();	
	adc_clkdiv_set(DIV128);
	adc_vref_set(AVCC);
	adc_input_set(ADC0);		
	
	while(1)
	{
		int ret = adc_read(&result);
		if(ret != ADC_ERR_SUCCESS)
			printf("Error!\n");
		printf("result = %d\n", result);
		_delay_ms(250);
	}
}
/*
int main() {
	uint8_t payload[4];
	uint8_t i = 0;
	GPIO_PIN_DIR_SET(B, 1, DIR_OUTPUT);
	GPIO_PIN_DIR_SET(D, 2, DIR_INPUT);
	
	log_init();	
	interrupt_global_enable();	
	EICRA = (EICRA & ~3) | ICS_LOW;
	EIMSK |= 1;
	nrf_init(SPI0);
	nrf_pwr_up();
	nrf_set_ptx();	
	nrf_datarate_set(NRF_DR_250KBPS);	
	while(1)
	{
		nrf_flush_tx();
		i++;
		sprintf((char *)payload, "%d", i);
		nrf_write_tx_payload(payload, sizeof(payload));
		printf("sending\n");
		printf("status = 0x%02x\n", nrf_reg8_read(NRF_REG_STATUS));
		GPIO_PIN_SET(B, 1, 1);
		while(!g_int)
			;		
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		g_int = 0;
	}
}
*/
/*
int main(void)
{
	log_init();
	GPIO_PIN_DIR_SET(B, 1, DIR_OUTPUT);
	GPIO_PIN_DIR_SET(B, 0, DIR_INPUT);
	nrf_init(SPI0);
	nrf_pwr_up();
	nrf_set_prx();
	nrf_datapipe_width_set(0, 4);
	while(1)
	{		
		GPIO_PIN_SET(B, 1, 1);
		while(GPIO_PIN_GET(B, 0))
			;		
		uint8_t status = nrf_reg8_read(NRF_REG_STATUS);		
		if(status & ~(1 << MAX_RT))
		{
			printf("max rt\n");
			nrf_reg8_bit_set(NRF_REG_STATUS, MAX_RT, 1);
		}
		else if (status & ~(1 << RX_DR))
		{
			printf("rx\n");
			nrf_reg8_bit_set(NRF_REG_STATUS, RX_DR, 1);
		}
		else if (status & ~(1 << TX_DS))
		{
			printf("tx\n");
			nrf_reg8_bit_set(NRF_REG_STATUS, TX_DS, 1);
		}
		int len = nrf_read_rx_payload_len();		
		uint8_t data[len];
		nrf_read_rx_payload(data);
		data[len] = '\0';
		printf("RX payload = '%s'\n", data);
		GPIO_PIN_SET(B, 1, 0);
	}
	
}*/

