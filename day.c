/*
 * File:   main.c
 * Author: cyiwe
 *
 * Created on October 31, 2021, 9:06 AM
 */


#include <xc.h>
#include "day.h"

static int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int day, month, year;

int leap(int y){
    return ((y % 4 == 0 && y%  100 != 0) || y % 400 == 0);
}

void nextday(void) 
{
    day += 1;
    if (day > days_in_month[month]){
        day = 1;
        month += 1;
        year += 1;
        if (leap(year)){
            days_in_month[2] = 29;
        }
        else {
            days_in_month[2] = 28;
        }
    }
}

void set_date(int d, int m, int y){
    if (m > 12){m = 1;}
    if (d > days_in_month[m]){d = days_in_month[m+1];}
    
    if (leap(y)){days_in_month[2] = 29;}
    else {days_in_month[2] = 28;}
    
    day = d;
    month = m;
    year = y;
}
    
unsigned int getDay(){
    return day;
}

unsigned int getMonth(){
    return month;
}

unsigned int getYear(){
    return year;
}