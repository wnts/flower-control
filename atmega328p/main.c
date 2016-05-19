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
#include "timer8.h"
#include "timer16.h"
#include "hcsr04.h"

volatile int g_int;
#define INT_NONE		0
#define INT_SEC			1
#define INT_NRF			2

ISR(INT0_vect)
{	
	g_int = INT_NRF;
	GPIO_PIN_SET(B, 1, 0);
}

ISR(TIMER0_COMPA_vect)
{
	static unsigned char ctr = 0;
	ctr++;
	if(ctr == 125)
	{
		ctr = 0;
		g_int = INT_SEC;
	}
}

void setup_t0(void)
{
	t0_clk_src_set(TCS_IO_CLK_DIV1024);
	t0_opmode_set(TOM8_CTC);
	OCR0A = 125;
	TIMSK0 |= 1 << OCIE0A;
}

void setup_nrf(void)
{
	GPIO_PIN_DIR_SET(B, 1, DIR_OUTPUT);
	GPIO_PIN_DIR_SET(D, 2, DIR_INPUT);
	EICRA = (EICRA & ~3) | ICS_LOW;
	EIMSK |= 1;
	nrf_init(SPI0);
	nrf_pwr_up();
	nrf_set_ptx();
	//nrf_datarate_set(NRF_DR_250KBPS);
}

void setup_adc(void)
{
	adc_enable();
	adc_clkdiv_set(DIV128);
	adc_vref_set(AVCC);
	adc_input_set(ADC0);
}

int main(void)
{
	log_init();
	interrupt_global_enable();	
	hcsr04_get_us();		
}

/*
int main(void)
{
	unsigned char seconds = 0;
	

	log_init();
	interrupt_global_enable();
	setup_t0();
	setup_nrf();
	setup_adc();		
	while(1)
	{
		switch(g_int)
		{
			uint16_t adc = 0;
			uint8_t status = 0;
			case INT_SEC:								
				nrf_flush_tx();						
				printf("%02x\n", nrf_reg8_read(NRF_REG_STATUS));
				adc_read(&adc);			
				printf("sending sensor data (%u)\n", adc);						
				nrf_write_tx_payload((uint8_t*)&adc, sizeof(adc));
				GPIO_PIN_SET(B, 1, 1);
				g_int = INT_NONE;
			break;
			case INT_NRF:					
				printf("%02x\n", nrf_reg8_read(NRF_REG_STATUS));
				status = nrf_reg8_read(NRF_REG_STATUS);
				if(status & (1 << TX_DS))
				{
					printf("transmit completed\n");					
					nrf_reg8_bit_set(NRF_REG_STATUS, TX_DS, 1);
				}
				if(status & (1 << MAX_RT))
				{
					printf("max rt\n");
					nrf_reg8_bit_set(NRF_REG_STATUS, MAX_RT, 1);
				}
				g_int = INT_NONE;				
			break;
			default:
			break;
		}		
		/*
		nrf_flush_tx();
		i++;
		sprintf((char *)payload, "%d", i);
		nrf_write_tx_payload(payload, sizeof(payload));
		printf("sending\n");
		printf("status = 0x%02x\n", nrf_reg8_read(NRF_REG_STATUS));
		GPIO_PIN_SET(B, 1, 1);
		while(!g_int)
			;
		g_int = 0;
		while(!g_sec)
			;		
		g_sec = 0;						
	}	
}
*/
/*
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

