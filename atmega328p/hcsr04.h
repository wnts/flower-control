#ifndef HCSR04_H_
#define HCSR04_H_

#include "gpio.h"
#include "timer16.h"

inline int hcsr04_get_us(void)
{
	t1_clk_src_set(TCS_IO_CLK);
	t1_opmode_set(TOM16_NORMAL);
	t1_input_capture_edge_set(CPE_RISING);
	t1_input_capture_int_enable();
	t1_enable();
	GPIO_PIN_DIR_SET(D, 7, DIR_OUTPUT);
	GPIO_PIN_SET(D, 7, 1);
	_delay_us(10);
	GPIO_PIN_SET(D, 7, 0);
}


#endif /* HCSR04_H_ */