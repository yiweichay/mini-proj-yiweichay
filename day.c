/*
 * File:   main.c
 * Author: cyiwe
 *
 * Created on October 31, 2021, 9:06 AM
 */


#include <xc.h>
#include "day.h"

static int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //number of days in each month
int day, date, month, year; //day = mon, tue etc, 

//code to calculate the leap year
int leap(int y){
    return ((y % 4 == 0 && y%  100 != 0) || y % 400 == 0);
}

//code for day increments
void nextday(void) 
{
    day += 1;
    if(day > 7){day = 1;} //if day count increases more than 7 (sunday), day = mon
    date += 1;
    if (date > days_in_month[month]){ 
        date = 1;
        month += 1;
        if (month>12){
            month = 1;
            year += 1;
            if (leap(year)){
                days_in_month[2] = 29;
            }
            else {
                days_in_month[2] = 28;
            }
        }
    }
}

//code to set the day,month,year
void set_date(int day_week, int d, int m, int y){
    if (m > 12){m = 1;}
    if (d > days_in_month[m]){d = days_in_month[m+1];}
    if (day_week > 7){day_week =1;}
    
    if (leap(y)){days_in_month[2] = 29;} //if leap year, then febuary has 29 days
    else {days_in_month[2] = 28;} //if not leap year, feb has 28 days
    
    //define the variables
    day = day_week;
    date = d;
    month = m;
    year = y;
}

//calculate the number of days until the last sunday of march/oct
int calculateDaysToTarget(int monthTarget, int recalculate){ // month target is either march(3) or oct(10)
    int numberOfDays = 0; //define the current number of days to 1st of march/oct
    if(month != monthTarget || (month == monthTarget && (date + 7-day) > days_in_month[month]) || recalculate == 1){ //nearest sunday is not in the same month
        numberOfDays = days_in_month[month] - date; 
        if(month < monthTarget){
            for(int i=month+1; i<monthTarget; i++){
                numberOfDays += days_in_month[i];
            }
        }
        else{
            for(int i=month+1; i<13; i++){ //this is until december
                numberOfDays += days_in_month[i];
            }
            for(int i=1; i<monthTarget; i++){ //from jan to march
                numberOfDays += days_in_month[i];
            }
            if(leap(year+1) && monthTarget > 2){
                numberOfDays++;
            }
            else if(leap(year)){
                numberOfDays--;
            }
        }
        numberOfDays += 1;
    }
    return numberOfDays;
}

//determine which day of the week is 1st of march/oct
int dateOfLastSunday(int monthTarget, int numberOfDays){
    int dateTarget;
    if(numberOfDays != 0){
        //firstDay is first day of monthTarget
        int firstDay = numberOfDays % 7; //this is to find out which day 1st of march falls on 
        firstDay += day;
        if(firstDay > 7){
            firstDay -= 7; //gives the day of the week for 1st march
        }
        dateTarget = 1+(7-firstDay); //gives date of first Sunday of march
    }
    else{
        dateTarget = date + (7-day);
        if(dateTarget > days_in_month[monthTarget]){
            dateTarget -= 7;
        }
    }
    while(dateTarget <= days_in_month[monthTarget]){
        dateTarget += 7;
    }
    dateTarget -= 7;
    return dateTarget;
}

unsigned int getDate(){
    return date;
}

unsigned int getMonth(){
    return month;
}

unsigned int getYear(){
    return year;
}

unsigned int getDayofWeek(){
    return day;
}