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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

//volatile unsigned int hour = 9;
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
    Interrupts_init();
    button_init();
    
    unsigned int LDRoutput;
    unsigned int count=0;
    while(PORTFbits.RF3){
        if (!PORTFbits.RF2){
            count++;
            if (count>24) {count=0;} //reset a when it gets too big
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
    while (1){
        LDRoutput = ADC_getval();
        set_led(LDRoutput);
        //LEDarray_disp_bin(hour);
    }
        
}
