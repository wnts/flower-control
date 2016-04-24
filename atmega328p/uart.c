#include <avr/io.h>
#include <string.h>
#include "uart.h"
#include "clock.h"

static struct UartConfig s_config;


int uart_config_set(enum Uart uart, struct UartConfig * uart_config)
{
	memcpy(&s_config, uart_config, sizeof(s_config));
	if(uart == UART0)
	{		
		// no double transmission speed
		UCSR0A &= ~(1 << U2X0);
		// no multiprocessor mode
		UCSR0A &= ~(1 << MPCM0);
		// enable tx and rx
		UCSR0B |= (1 << RXEN0);
		UCSR0B |= (1 << TXEN0);
		// set mode
		UCSR0C = (UCSR0C & ~(3 << UMSEL01)) | (uart_config->mode << UMSEL01);
		// set parity
		UCSR0C = (UCSR0C & ~(3 << UPM01)) | (uart_config->parity << UPM01);
		// clock polarity bit
		UCSR0C &= ~(1 << UCPOL0);		
		// set stop bits
		UCSR0C = (UCSR0C & ~(1 << USBS0)) | (uart_config->stop_bits << USBS0);
		// set character size
		UCSR0B = (UCSR0B & ~(1 << UCSZ02)) | (((uart_config->char_size >> 2) & 0xFE) << UCSZ02);
		
		// set baud rate
		uint16_t br = 0;		
		switch(uart_config->mode)		
		{			
			case ASYNC:
				br = sysclock_get() / (16 * uart_config->baudrate) - 1;
			break;
			default:
				return 0;
			break;			
		}		
		UBRR0L = br & 0xFF;
		UBRR0H = (br >> 8) & 0x0F;
	}
	else
	{
		return -1;
	}

	return 1;
};

void uart_putchar(enum Uart uart, char c)
{
	switch(uart)
	{
		case UART0:
			// wait until data register is empty
			while((UCSR0A & (1 << UDRE0)) == 0)
				;
			UDR0 = c;
			// wait for transmit to complete
			while(!(UCSR0A & (1 << TXC0)))
				;
		break;
		default:
			return;
		break;
	}
}