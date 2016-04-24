#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include <avr/io.h>

#define ICS_LOW				0
#define ICS_EDGE_ANY		1
#define ICS_EDGE_FALLING	2
#define ICS_EDGE_RISING		3

inline void interrupt_global_enable(void)
{
	SREG |= 1 << 7;
}

inline void interrupt_global_disable(void)
{
	SREG &= ~(1 << 7);
}



#endif /* INTERRUPT_H_ */