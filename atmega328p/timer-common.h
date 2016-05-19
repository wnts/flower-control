#ifndef TIMER-COMMON_H_
#define TIMER-COMMON_H_

#define T1_R16_READ(reg, var)		do {								\
										unsigned char sreg;			\
										sreg = SREG;				\
										_CLI();						\
										(var) = (reg);				\
										SREG = sreg;				\
								while(0)

enum TimerClkSrc { TCS_NONE,
	TCS_IO_CLK,
	TCS_IO_CLK_DIV8,
	TCS_IO_CLK_DIV64,
	TCS_IO_CLK_DIV256,
	TCS_IO_CLK_DIV1024,
	TCS_EXT_CLK_FALLING,
    TCS_EXT_CLK_RISING };


#endif /* TIMER-COMMON_H_ */