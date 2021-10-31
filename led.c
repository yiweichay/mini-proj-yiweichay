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

void led_on(void)
{
    LATHbits.LATH3 = 1; //LED RH3 turns on
}

void led_off(void)
{
    LATHbits.LATH3 = 0; //LED RH3 turns off
}