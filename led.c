/*
 * File:   main.c
 * Author: cyiwe
 *
 * Created on October 31, 2021, 9:06 AM
 */


#include <xc.h>
#include "led.h"

void led_init(void) 
{
    LATHbits.LATH3 = 0; //set initial output state
    TRISHbits.TRISH3 = 0; //set TRIS value for pin (output)
    
}

void set_led(unsigned int value){
    if(value >= dayLightPresent){
        LATHbits.LATH3 = 0;
    }
    else if(value <= dayLightAbsent){
        LATHbits.LATH3 = 1;
    }
}
