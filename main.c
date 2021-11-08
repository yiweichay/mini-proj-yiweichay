/*
 * File:   main.c
 * Author: cyiwe
 *
 * Created on October 31, 2021, 9:06 AM
 */
// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)


#include <xc.h>
#include "LEDarray.h"
#include "led.h"
#include "interrupts.h"
#include "comparator.h"
#include "ADC.h"
#include "timers.h"
#include "day.h"
#include "LCD.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

static volatile unsigned int sec = 0;
static volatile unsigned int min = 0;
static volatile unsigned int hour = 0;
static volatile unsigned int offled = 0;
char *buf;
char num;
static volatile unsigned int date_daylightOn;
static volatile unsigned int date_daylightOff;
static volatile unsigned int dusk[2];
static volatile unsigned int dawn[2];
static volatile unsigned int midday[2] = {0, 12};
static volatile unsigned int solarnoon[] = {0, 12};

unsigned int LDRoutput;
static volatile unsigned int dawnOver = 1;
static volatile unsigned int duskOver = 1;

void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){
        sec++;
        if (sec > 60){
            sec = 0;
            min += 1;
            if (min > 60){
                min = 0;
                hour += 1;
                if (hour==24){
                    hour=0; //reset a when it gets too big
                    nextday();
                    if(dawnOver != 0 && duskOver != 0){
                        dawnOver = 0;
                        duskOver = 0;
                    }
                } 
        
                if (hour == 1){ //turn off led between 1am and 5am
                    offled = 1;
                    LATHbits.LATH3 = 0;
                }
                else if(hour == 5){offled = 0;}
        
                if (getMonth()==3 && getDate()== date_daylightOn && hour==1){
                    hour += 1;
                    int numberOfDays = calculateDaysToTarget(3, 1);
                    date_daylightOn = dateOfLastSunday(3, numberOfDays);
                }
        
                if (getMonth()==10 && getDate() == date_daylightOff && hour==2){
                    hour -=1;
                    int numberOfDays = calculateDaysToTarget(10, 1);
                    date_daylightOff = dateOfLastSunday(10, numberOfDays);
                }

                LEDarray_disp_bin(hour);
            }
            
            ///   at midday time calculated, reset clock to be 12pm(example))
            if(min == midday[0] && hour == midday[1]){
                if (midday[0] != solarnoon[0] || midday[1] != solarnoon[1]){
                    min = solarnoon[0];
                    hour = solarnoon[1];
                    LEDarray_disp_bin(hour);
                }
            }
        }
        TMR0H = 0b1011; //11
        TMR0L = 0b11011011;//219
        PIR0bits.TMR0IF = 0; // clearing the flag
    }
}
void button_init(void){
    //setup pin for input
    TRISFbits.TRISF2 = 1;
    ANSELFbits.ANSELF2 = 0;
    TRISFbits.TRISF3 = 1;
    ANSELFbits.ANSELF3 = 0;
}

void main(void) {
    //call initialisation functions
    led_init();
    ADC_init();
    LEDarray_init();
    Timer0_init();
    button_init();
    LCD_init();
    
    set_date(3,30,3,2022);
    int numberOfDays = calculateDaysToTarget(3,0);
    date_daylightOn = dateOfLastSunday(3, numberOfDays);
    
    numberOfDays = calculateDaysToTarget(10,0);
    date_daylightOff = dateOfLastSunday(10, numberOfDays);
    
    //unsigned int count=0;
    while(PORTFbits.RF3){
        if (!PORTFbits.RF2){
            hour++;
            if (hour==24) {
                hour=0;
                nextday();
            } //reset a when it gets too big
            
            if (hour>=1 && hour<5){ //turn off led between 1am and 5am
                offled = 1;
                LATHbits.LATH3 = 0;
            }
            else{offled = 0;}
            
            LEDarray_disp_bin(hour); //output a on the LED array in binary
            __delay_ms(500);
            
        }
    }
    for(int i=0; i<3; i++){
        LEDarray_disp_bin(hour); //output a on the LED array in binary
        __delay_ms(500);
        LEDarray_disp_bin(0); //output a on the LED array in binary
        __delay_ms(500);
    }
    LEDarray_disp_bin(hour);
    Interrupts_init();
    
    while (1){
        buf = &num;
        DATE2String(buf, date_daylightOn, getDate(), getMonth(), getYear(), sec, min, hour, midday[1], midday[0]);
        LDRoutput = ADC_getval();
        
        if ((LDRoutput >= dayLightPresent) && (dawnOver == 0)){
            dawn[0] = min;
            dawn[1] = hour;
            dawnOver = 1;
        }
        if (LDRoutput <= dayLightAbsent && duskOver == 0 && dawnOver == 1){
            dusk[0] = min;
            dusk[1] = hour;
            ///get midday time from clock here (take dusk - dawn)
            if (dusk[0] < dawn[0]){
                dusk[1] -= 1;
                dusk[0] += 60;
            }
            
            midday[0] = (dusk[0]-dawn[0])/2 + dawn[0];
            midday[1] = (dusk[1]-dawn[1])/2 + dawn[1];
            if ((dusk[1]-dawn[1])%2 != 0){
                midday[0] += 30;
            }
            duskOver = 1;
        }
        
        if (!offled){
            set_led(LDRoutput);
        }
    }
}
