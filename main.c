/*
 * File:   main.c
 * Author: Matthew Mould
 *
 * Created on 14 February 2017, 12:16
 */

#pragma config OSC = INTIO7     // Internal osc, RA6=CLKO, RA7=I/O
#pragma config FCMEN = OFF		// Fail-Safe Clock Monitor disabled 
#pragma config IESO = OFF		// Oscillator Switchover mode disabled 
#pragma config WDT = OFF        // WDT disabled (control through SWDTEN bit)
#pragma config PWRT = OFF       // racmod  -> PWRT disabled
#pragma config MCLRE = ON       // MCLR pin enabled; RG5 input pin disabled
#pragma config XINST = OFF      // Instruction set extension disabled
#pragma config BOREN = OFF      // Brown-out controlled by software
#pragma config BORV = 3         // Brown-out voltage set for 2.0V, nominal
#pragma config STVREN = OFF		// Stack full/underflow will not cause Reset
#pragma config CP = OFF			// Program memory block not code-protected 

#include <i2c.h>
#include "taki.h"

void main(void) 
{
    initialise();
    
    return;
}

void initialise()
{
    void Init() {

	int i;

	OSCCON = 0b01110010;        	// Select 8 MHz internal oscillator
    
	LCDSE0 = 0b11111111;        	// Enable  LCD segments 07-00
	LCDSE1 = 0b11111111;        	// Enable  LCD segments 15-08
	LCDSE2 = 0b11111111;        	// Enable  LCD segments 23-16
	LCDSE3 = 0b00000000;        	// Disable LCD segments 31-24
	LCDCON = 0b10001000;         	// Enab LC controller. Static mode. INTRC clock
	LCDPS  = 0b00110110;         	// 37 Hz frame frequency
    
	ADCON1 = 0b00111111;        	// Make all ADC/IO pins digital
	TRISA = 0b00000011;             // RA0 and RA1 buttons 0 1
    TRISB = 0b00000000;
	TRISC = 0b00011000;				// RC3 and RC4 do the I2C bus
    TRISE = 0b11110000              // buttons 2 3 4 5
	TRISG = 0b11111111;				// RG0, RG1 & RG2 switches and 6 7 buttons
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
    
    INTCONbits.TMR0IF = 0;          // Clear timer flag
	//T0CON = 0b00000011;				// Prescale by 16
    T0CON = 0b00001000;             // No prescale
    TMR0H = 0;                      // Clear timer count
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // Start timer
	OpenI2C( MASTER, SLEW_OFF);
	SSPADD = 0x3F;
}
}