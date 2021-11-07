#ifndef _day_H
#define _day_H

#include <xc.h>

#define _XTAL_FREQ 64000000


void nextday(void);
int leap(int y);
void set_date(int d, int m, int y);
unsigned int getDay();
unsigned int getMonth();
unsigned int getYear();

#endif
