#ifndef _led_H
#define _led_H

#include <xc.h>

#define _XTAL_FREQ 64000000

#define dayLightPresent 0b1111
#define dayLightAbsent 0b1100

void led_init(void);
void set_led(unsigned int value);

#endif
