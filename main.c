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

static volatile unsigned int count = 0;
static volatile unsigned int offled = 0;
char *buf;
char num;

void __interrupt(high_priority) HighISR()
{
	//add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if(PIR0bits.TMR0IF){
        count++;
        if (count==24){
            count=0;
            nextday();
            buf = &num;
            DATE2String(buf, getDay(), getMonth(), getYear());
            
        } //reset a when it gets too big
        
        if (count == 1){ //turn off led between 1am and 5am
            offled = 1;
            LATHbits.LATH3 = 0;
        }
        else if(count == 5){offled = 0;}

        LEDarray_disp_bin(count);
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
    
    set_date(7,11,2021);
    unsigned int LDRoutput;
    //unsigned int count=0;
    while(PORTFbits.RF3){
        if (!PORTFbits.RF2){
            count++;
            if (count==24) {
                count=0;
                nextday();
            } //reset a when it gets too big
            
            
            if (count>=1 && count<5){ //turn off led between 1am and 5am
                offled = 1;
                LATHbits.LATH3 = 0;
            }
            else{offled = 0;}
            
            LEDarray_disp_bin(count); //output a on the LED array in binary
            __delay_ms(500);
            
        }
    }
    for(int i=0; i<3; i++){
        LEDarray_disp_bin(count); //output a on the LED array in binary
        __delay_ms(500);
        LEDarray_disp_bin(0); //output a on the LED array in binary
        __delay_ms(500);
    }
    LEDarray_disp_bin(count);
    Interrupts_init();
    while (1){
        buf = &num;
        DATE2String(buf, getDay(), getMonth(), getYear());
        if (!offled){
            LDRoutput = ADC_getval();
            set_led(LDRoutput);
            //LEDarray_disp_bin(hour);
        }    
    }
}
