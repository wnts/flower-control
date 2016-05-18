#include <stdint.h>
#include <stddef.h>
#include "nrf.h"
#include "spi.h"
#include "log.h"



#define	W_TX_PAYLOAD			0xB0
#define R_RX_PAYLOAD			0x61
#define R_RX_PL_WID				0x60
#define FLUSH_TX				0xE1
#define W_TX_PAYLOAD_NOACK		0x0B

static enum SPI s_spi;

int nrf_init(enum SPI spi)
{
	s_spi = spi;
	struct SPIConfig spi_config = { .clk_div = SPICLKDIV_64,
									.mode = MASTER,
									.cpol = SPI_CPOL0,
									.cpha = SPI_CPHA0,
									.data_order = MSB };
	if(!spi_config_set(s_spi, &spi_config))
		return 0;
	GPIO_PIN_DIR_SET(B, 4, DIR_INPUT);
	GPIO_PIN_DIR_SET(B, 3, DIR_OUTPUT);
	GPIO_PIN_DIR_SET(B, 2, DIR_OUTPUT);
	GPIO_PIN_DIR_SET(B, 5, DIR_OUTPUT);
	// set cs high
	GPIO_PIN_SET(B, 2, 1);
	spi_enable(s_spi);
	return 1;
}

int nrf_send_cmd(uint8_t cmd, uint8_t * data_recv, uint8_t * data_send, unsigned short len)
{		
	GPIO_PIN_SET(B, 2, 0);
	spi_transfer_char(s_spi, cmd);		
	for(int i = 0; i != len; i++)
	{		
		if(data_send == NULL && data_recv == NULL)
		return 0;
		if(data_send != NULL)
		{
			spi_transfer_char(s_spi, data_send[i]);
		}
		else
		{
			data_recv[i] = spi_transfer_char(s_spi, 0xff);
		}
	}
	GPIO_PIN_SET(B, 2, 1);
	return 1;
		
}

int nrf_reg_read(uint8_t reg_addr, uint8_t * data, unsigned short len)
{	
	return nrf_send_cmd(reg_addr, data, NULL, len);
}

int nrf_reg_write(uint8_t reg_addr, uint8_t * data, unsigned short len)
{
	return nrf_send_cmd(reg_addr | 0x20, NULL, data, len);
}

int nrf_address_set(uint8_t * addr)
{
	return nrf_reg_write(NRF_REG_TX_ADDR, addr, NRF_MAX_ADDR_LEN);
}

int nrf_write_tx_payload(uint8_t * data, unsigned short len)
{
	return nrf_send_cmd(W_TX_PAYLOAD, NULL, data, len);
}

int nrf_write_tx_payload_noack(uint8_t * data, unsigned short len)
{
	return nrf_send_cmd(W_TX_PAYLOAD_NOACK, NULL, data, len);
}

int nrf_flush_tx(void)
{
	return nrf_send_cmd(FLUSH_TX, NULL, NULL, 0);
}

int nrf_read_rx_payload_len(void)
{
	uint8_t len = 0;
	nrf_send_cmd(R_RX_PL_WID, &len, NULL, sizeof(len));
	return len;
}

int nrf_read_rx_payload(uint8_t * data)
{
	uint8_t len = nrf_read_rx_payload_len();
	return nrf_send_cmd(R_RX_PAYLOAD, data, NULL, len);
}

int nrf_datarate_set(enum NrfDatarate rate)
{
	nrf_reg8_bit_set(NRF_REG_RF_SETUP, RF_DR_LOW, rate & 2);
	nrf_reg8_bit_set(NRF_REG_RF_SETUP, RF_DR_HIGH, rate & 1);
}






