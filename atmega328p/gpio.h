#ifndef GPIO_H_
#define GPIO_H_
#include <avr/io.h>

enum GpioDir { DIR_INPUT, DIR_OUTPUT };

#define GPIO_PIN_DIR_SET(port, pin, dir) do { DDR##port = (DDR##port & ~(1 << pin)) | (dir << pin); } while(0);
#define GPIO_PIN_GET(port, pin) (PIN##port & (1 << (pin)))
#define GPIO_PIN_SET(port, pin, value) (PORT##port = (PORT##port & ~(1 << (pin))) | ((value) << pin))





#endif /* GPIO_H_ */