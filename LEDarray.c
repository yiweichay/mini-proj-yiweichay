#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{
    TRISGbits.TRISG0 = 0;	//set up TRIS registers for pins connected to LED array, pin G0 set to output
    TRISGbits.TRISG1 = 0;   // pin G1 set to output
    TRISAbits.TRISA2 = 0;
    TRISFbits.TRISF6 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISFbits.TRISF0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
	
    LATGbits.LATG0 = 0; //set initial output LAT values (they may have random values when powered on), output of pin G0 set high
    LATGbits.LATG1 = 0; // output of pin G1 set low
    LATAbits.LATA2 = 0;
    LATFbits.LATF6 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATFbits.LATF0 = 0;
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number)
{
	//some code to turn on/off the pins connected to the LED array
	//if statements and bit masks can be used to determine if a particular pin should be on/off
	//see Readme.md for examples
    if (number & 1){
        LATGbits.LATG0 = 1;
    }
    else{
        LATGbits.LATG0 = 0;
    }
    if (number & 2){
        LATGbits.LATG1 = 1;
    }
    else{
        LATGbits.LATG1 = 0;
    }
    if (number & 4){
        LATAbits.LATA2 = 1;
    }
    else{
        LATAbits.LATA2 = 0;
    }
    if (number & 8){
        LATFbits.LATF6 = 1;
    }
    else{
        LATFbits.LATF6 = 0;
    }
    if (number & 16){
        LATAbits.LATA4 = 1;
    }
    else{
        LATAbits.LATA4 = 0;
    }
    if (number & 32){
        LATAbits.LATA5 = 1;
    }
    else{
        LATAbits.LATA5 = 0;
    }
    if (number & 64){
        LATFbits.LATF0 = 1;
    }
    else{
        LATFbits.LATF0 = 0;
    }
    if (number & 128){
        LATBbits.LATB0 = 1;
    }
    else{
        LATBbits.LATB0 = 0;
    }
    if (number & 256){
        LATBbits.LATB1 = 1;
    }
    else{
        LATBbits.LATB1 = 0;
    }
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val = 0;
	
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    for(int i=1; i<10; i++){ // for loop
        if(number >= 10*i){ 
            disp_val += 1 << (i-1); //if number >= 20, disp_val + 1 shifted by 1 bit so the 2nd LED turns on
        }
    }

	LEDarray_disp_bin(disp_val); 	//display value on LED array
}

void LEDarray_intensity_meter(unsigned int number)
{
	unsigned int disp_val = 0;
	
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    //ambient light in room gives int tmp_val = 42 (101010)
    //covering with thumb gives int tmp_val = 8 (1000)
    unsigned int offset = 8;
    for(int i=1; i<10; i++){ // for loop
        if(number-offset >= 4*i){ 
            disp_val += 1 << (i-1); //if number >= 20, disp_val + 1 shifted by 1 bit so the 2nd LED turns on
        }
    }

	LEDarray_disp_bin(disp_val); 	//display value on LED array
}

/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
	unsigned int disp_val = 0;
	
	// some code to format the variable cur_val and max, store in disp_val for display on the LED array
	// hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
    
    //ambient light in room gives int tmp_val = 42 (101010)
    //covering with thumb gives int tmp_val = 8 (1000)
    unsigned int step_interval = 20 ; //in my room, step interval = 4
    unsigned int offset = 15; //in my room offset = 8
    for(int i=1; i<10; i++){ // for loop
        if(cur_val-offset >= step_interval*i){ 
            disp_val += 1 << (i-1); //if number >= 20, disp_val + 1 shifted by 1 bit so the 2nd LED turns on
            
        }
    }
    
    if(max != cur_val){
        unsigned int disp_max = 1 << ((max - offset)/step_interval);//converts max to most significant bit
        disp_val |= disp_max; //display value = display value OR display max
    }
    
	LEDarray_disp_bin(disp_val);	//display value on LED array
}

