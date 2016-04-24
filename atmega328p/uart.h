#ifndef UART_H_
#define UART_H_
#include <stdbool.h>

enum Uart {UART0, UART1};
enum UartMode { ASYNC = 0, SYNC = 1, SPI = 3 };
enum UartStopBits { STOPBITS1, STOPBITS2 };
enum UartCharSize { UART5B = 0, UART6B = 1, UART7B = 2, UART8B = 3, UART9B = 7};
enum UartParity { DISABLED = 0, EVEN = 2, ODD = 3};

struct UartConfig {
	uint32_t baudrate;
	enum UartMode mode;
	enum UartStopBits stop_bits;
	enum UartCharSize char_size;
	enum UartParity parity;
};

int uart_config_set(enum Uart uart, struct UartConfig * uart_config);
const struct UartConfig * uart_config_get(enum Uart uart);
void uart_putchar(enum Uart uart, char c);




#endif /* UART_H_ */