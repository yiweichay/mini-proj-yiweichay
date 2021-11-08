#ifndef _day_H
#define _day_H

#include <xc.h>

#define _XTAL_FREQ 64000000


void nextday(void);
int leap(int y);
void set_date(int day_week, int d, int m, int y);
unsigned int getDate();
unsigned int getMonth();
unsigned int getYear();
unsigned int getDayofWeek();

int dateOfLastSunday(int monthTarget, int numberOfDays);
int calculateDaysToTarget(int monthTarget, int recalculate);

#endif
