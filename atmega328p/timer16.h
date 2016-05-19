#ifndef TIMER16_H_
#define TIMER16_H_
#include <avr/io.h>
#include "common.h"
#include "timer-common.h"

static enum TimerClkSrc t1_saved_clk_src = TCS_NONE;

enum T1InputCapEdge {
	CPE_FALLING,
	CPE_RISING	
};

enum Timer16Opmode {
	TOM16_NORMAL
};

inline int t1_clk_src_set(enum TimerClkSrc clk_src)
{
	TCCR1B = (TCCR1B & ~(7 << CS10)) | clk_src;
	return ERR_SUCCESS;
}

inline enum TimerClkSrc t1_clk_src_get(void)
{
	return TCCR1B & (7 << CS10);
}

inline int t1_opmode_set(enum Timer16Opmode opmode)
{
	TCCR1A = (TCCR1A & ~(3 << WGM10)) | (opmode & 3);
	TCCR1B = TCCR1B & ~(3 << WGM02) | opmode >> 2;
	return ERR_SUCCESS;
}

inline void t1_enable(void)
{
	t0_clk_src_set(t1_saved_clk_src);
}

inline void t1_disable(void)
{
	t1_saved_clk_src = t1_clk_src_get();
	t1_clk_src_set(TCS_NONE);
}

inline void t1_input_capture_edge_set(enum T1InputCapEdge edge)
{	
	TCCR1B = (TCCR1B & ~(1 << ICES1)) | edge;
}

inline void t1_input_capture_int_enable(void)
{	
	TIMSK1 |= 1 << ICIE1;	
}

inline void t1_input_capture_int_disable(void)
{
	TIMSK1 = TIMSK1 & ~(1 << ICIE1);
}




#endif /* TIMER16_H_ */