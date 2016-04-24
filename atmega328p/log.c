#include <stdio.h>
#include "log.h"
#include "uart.h"

static int log_putchar(char c, FILE *stream)
{
	uart_putchar(UART0, c);
	return 0;
}

int log_init(void)
{
	static FILE uart_stdout = FDEV_SETUP_STREAM(log_putchar, NULL, _FDEV_SETUP_WRITE);	
	struct UartConfig uart_config = { .baudrate = 9600,
									  .mode = ASYNC,
									  .stop_bits = STOPBITS1,
									  .char_size = UART8B,
									  .parity = DISABLED };
	if(!uart_config_set(UART0, &uart_config))
		return 0;
	stdout = &uart_stdout;
	return 1;
}

