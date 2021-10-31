#ifndef _led_H
#define _led_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void led_init(void);
void led_on(void);
void led_off(void);

#endif
