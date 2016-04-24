#ifndef SPI_H_
#define SPI_H_
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include "gpio.h"

enum SPI { SPI0 };
enum SPIDataOrder { MSB, LSB };
enum SPIMode { SLAVE = 0, MASTER = 1};
enum SPIClkDiv { SPICLKDIV_4,
				 SPICLKDIV_16, 
				 SPICLKDIV_64,
				 SPICLKDIV_128,
				 SPICLKDIV_2,
				 SPICLKDIV_8,
				 SPICLKDIV_32 };
// using enums for this to restrict values to 0 and 1, which
// allows for easy bit manipulation in implementation code
enum SPICpol { SPI_CPOL0, SPI_CPOL1 };
enum SPICpha { SPI_CPHA0, SPI_CPHA1 };

struct SPIConfig {
	enum SPIClkDiv clk_div;
	enum SPIMode mode;
	enum SPICpol cpol;
	enum SPICpha cpha;
	enum SPIDataOrder data_order;	
};

int spi_config_set(enum SPI spi, struct SPIConfig * spi_config);
struct SPIConfig * spi_config_get(enum SPI spi);
void spi_enable(enum SPI spi);
void spi_disable(enum SPI spi);
char spi_transfer_char(enum SPI spi, char c);
#endif /* SPI_H_ */