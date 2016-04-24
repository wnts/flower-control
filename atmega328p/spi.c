#include <stdint.h>
#include <string.h>
#include "spi.h"

static struct SPIConfig s_conf;

int spi_config_set(enum SPI spi, struct SPIConfig * spi_config)
{
	switch(spi)
	{
		case SPI0:				
			memcpy(&s_conf, spi_config, sizeof(s_conf));			
			// set data order
			SPCR = (SPCR & ~(1 << DORD)) | (spi_config->data_order << DORD);
			// set mode (master/slave)
			SPCR = (SPCR & ~(1 << MSTR)) | (spi_config->mode << MSTR);			
			// set cpol and cpha
			SPCR = (SPCR & ~(1 << CPOL)) | (spi_config->cpol << CPOL);
			SPCR = (SPCR & ~(1 << CPHA)) | (spi_config->cpha << CPHA);						
			// set master clock frequency
			if(spi_config->mode == MASTER)
			{
				SPCR = (SPCR & ~(3 << SPR1))  | ((spi_config->clk_div & 0x03));	
				SPSR = (SPSR & ~(1 << SPI2X)) | (((spi_config->clk_div >> 2) & 0xFE) << SPI2X);
			}			
		break;
		default:
			return 0;
		break;
	}

	return 1;
}

inline char spi_transfer_char(enum SPI spi, char c)
{
	switch(spi)
	{
		case SPI0:
			SPDR = c;
			while(!(SPSR & (1 << SPIF)))
				;
			return SPDR;
		break;
		default:
			return 0;
		break;
	}
}

inline void spi_enable(enum SPI spi)
{
	switch(spi)
	{
		case SPI0:
			SPCR |= (1 << SPE);
		break;
		default:
		break;
	}
	
}
inline void spi_disable(enum SPI spi)
{
	switch(spi)
	{
		case SPI0:
		SPCR &= ~(1 << SPE);
		break;
		default:
		break;
	}
}

