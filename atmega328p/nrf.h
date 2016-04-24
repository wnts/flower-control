#ifndef NRF_H_
#define NRF_H_
#include <stdio.h>
#include <stdint.h>
#include "spi.h"

#define NRF_MAX_ADDR_LEN	5

#define NRF_REG_CONFIG		0x00
#define PRIM_RX				0
#define PWR_UP				1
#define MASK_MAX_RT			4
#define MASK_TX_DS			5
#define MASK_RX_DR			6


#define NRF_REG_EN_RXADDR	0x02
#define NRF_REG_SETUP_RETR	0x04

#define NRF_REG_RF_SETUP	0x06
#define RF_DR_LOW			5
#define RF_DR_HIGH			3

#define NRF_REG_STATUS		0x07
#define MAX_RT				6
#define TX_DS				5
#define RX_DR				4

#define NRF_REG_TX_ADDR		0x10
#define NRF_REG_PW_P0		0x11



#define NRF_REG_FEATURE		0x1D
#define EN_DYN_ACK			0
#define EN_DPL				2



enum NrfMode { PTX, RTX };

enum NrfDatarate {
	NRF_DR_1MBPS = 0,
	NRF_DR_2MBPS = 1,
	NRF_DR_250KBPS = 2	
};


int nrf_init(enum SPI spi);
int nrf_reg_read(uint8_t reg_addr, uint8_t * data, unsigned short len);
int nrf_reg_write(uint8_t reg_addr, uint8_t * data, unsigned short len);
int nrf_write_tx_payload(uint8_t * data, unsigned short len);
int nrf_write_tx_payload_noack(uint8_t * data, unsigned short len);
int nrf_flush_tx(void);
int nrf_reg_read(uint8_t reg_addr, uint8_t * data, unsigned short len);
int nrf_reg_write(uint8_t reg_addr, uint8_t * data, unsigned short len);

inline uint8_t nrf_reg8_read(uint8_t reg_addr)
{
	uint8_t reg8 = 0;		
	nrf_reg_read(reg_addr, &reg8, sizeof(reg8));
	return reg8;
}

inline int nrf_reg8_write(uint8_t reg_addr, uint8_t value)
{
	return nrf_reg_write(reg_addr, &value, sizeof(value));
}

inline int nrf_reg8_bit_set(uint8_t reg_addr, uint8_t bit, uint8_t value)
{
	uint8_t reg_config = nrf_reg8_read(reg_addr);
	reg_config = (reg_config & ~(1 << bit)) | (value << bit);
	return nrf_reg8_write(reg_addr, reg_config);
}


inline int nrf_set_ptx(void)
{
	return nrf_reg8_bit_set(NRF_REG_CONFIG, PRIM_RX, 0);
}

inline int nrf_set_prx(void)
{
	return nrf_reg8_bit_set(NRF_REG_CONFIG, PRIM_RX, 1);
}



inline int nrf_pwr_up(void)
{
	return nrf_reg8_bit_set(NRF_REG_CONFIG, 1, 1);
}
inline int nrf_datapipe_enable(unsigned short pipe)
{
	return nrf_reg8_bit_set(NRF_REG_EN_RXADDR, pipe, 1);
}

inline int nrf_datapipe_disable(unsigned short pipe)
{
	return nrf_reg8_bit_set(NRF_REG_EN_RXADDR, pipe, 0);
}

inline int nrf_datapipe_width_set(unsigned short pipe, unsigned short width)
{
	return nrf_reg8_write(NRF_REG_PW_P0 + pipe, width);
}

inline int nrf_dynack_enable(void)
{
	return nrf_reg8_bit_set(NRF_REG_FEATURE, EN_DYN_ACK, 1);
}

inline int nrf_auto_retransmit_disable(void)
{
	uint8_t setup_retr = nrf_reg8_read(NRF_REG_SETUP_RETR);
	setup_retr &= 0xf0;
	return nrf_reg8_write(NRF_REG_SETUP_RETR, setup_retr);
}


int nrf_write_tx_payload(uint8_t * data, unsigned short len);
int nrf_read_rx_payload_len(void);
int nrf_read_rx_payload(uint8_t * data);
int nrf_flush_tx(void);
int nrf_datarate_set(enum NrfDatarate rate);

	



#endif /* NRF_H_ */