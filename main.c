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
#include "fm.h"
#include "switches_buttons.h"

// FM register bank defaults -
const unsigned int regDflt[18] = {
	0xFFFF,     // R0 -- the first writable register .  (disable xo_en)   
	0x5B15,     // R1.   
	0xD0B9,     // R2.   
	0xA010,     // R3   seekTHD = 16   
	0x0780,     // R4   
	0x28AB,     // R5   
	0x6400,     // R6   
	0x1EE7,     // R7   
	0x7141,     // R8   
	0x007D,     // R9   
	0x82C6,     // R10  disable wrap   
	0x4F55,     // R11. <--- (disable xo_output)   
	0x970C,     // R12.   
	0xB845,     // R13   
	0xFC2D,     // R14   
	0x8097,     // R15   
	0x04A1,     // R16   
	0xDF6A      // R17
};

unsigned int regImg[18];	// FM register bank images
unsigned char switchState[3] = {2, 2, 2}; //Previous state of switches

void initialise();
unsigned char FMread(unsigned char regAddr, unsigned int *data);
unsigned char FMvers(unsigned int *vsn);
void fm_error();
void dly(int d);
unsigned char FMinit();
unsigned char FMwrite(unsigned char adr);
unsigned char FMready(unsigned int *rdy);
void show_freq();
void check_switches();
int check_buttons();

void main(void) 
{
    unsigned int fm_version;
    int button_pressed;
    
    initialise();
    
    FMvers(&fm_version);
    if (fm_version != 0x1010)
        fm_error();     // Invalid FM receiver
    
    if (FMinit() != exit_success)
        fm_error();
    
    while(1)
    {
        check_switches();
        button_pressed = check_buttons(); // -1 if no button
        if (button_pressed != -1)
            set_lcd(button_pressed);
    }
    
    return;
}

void fm_error()
{
    // TODO: add lcd error message
    while(1)
        continue;
}

void dly(int d) {

	int i = 0;

	for ( ; d; --d) 
		for (i = 100;  i;  --i) ;
}

void initialise()
{
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

unsigned char FMread(unsigned char regAddr, unsigned int *data) {

	unsigned char firstByte;
	unsigned char secondByte;

	StartI2C();					// Begin I2C communication
	IdleI2C();					// Allow the bus to settle

	// Send address of the chip onto the bus
	if (WriteI2C(FMI2CADR) == exit_failure)
        return exit_failure;
    
	IdleI2C();
	WriteI2C(regAddr);			// Address the internal register
	IdleI2C();
	RestartI2C();				// Initiate a RESTART command
	IdleI2C();
	WriteI2C(FMI2CADR + DEVRD);	// Ask for read from FM chip
	IdleI2C();
	firstByte = ReadI2C(); 		// Returns the MSB byte
	IdleI2C();
	AckI2C();					// Send back Acknowledge
	IdleI2C();
	secondByte = ReadI2C();		// Returns the LSB
	IdleI2C();
	NotAckI2C();
	IdleI2C();
	StopI2C();
	IdleI2C();
	*data = firstByte;
	*data <<= 8;
	*data = *data | secondByte;

	return exit_success;
}

// Gets FM chip version and copies it to vsn
unsigned char FMvers(unsigned int *vsn) {
	if (FMread(FMCHIPVERSADR, vsn) != exit_success)
        return exit_failure;
	return exit_success;
}

unsigned char FMinit() {

	unsigned char ad;
	unsigned int dat;

	// Copy default FM register values to the image set -
	for(ad = 0; ad < 18; ad++) regImg[ad] = regDflt[ad];

	dat = regImg[0];
	regImg[0] &= ~1;
    
	if (FMwrite(0) != exit_success)
        return exit_failure;
    
	for(ad = 0; ad < 18; ad++)
		if (FMwrite(ad) != exit_success)
            return exit_failure;

	regImg[0] = dat | 1;
    
	if (FMwrite(0) != exit_success)
        return exit_failure;
    
	dly(20);
    
	while (FMready(&dat), !dat) // While fm radio is busy
        dly(2);
    
	show_freq();
	return exit_success;
}

unsigned char FMwrite(unsigned char adr) {

	unsigned char firstByt;
	unsigned char secndByt;

	firstByt = regImg[adr] >> 8;
	secndByt = regImg[adr];

	StartI2C();					// Begin I2C communication
	IdleI2C();

	// Send slave address of the chip onto the bus
	if (WriteI2C(FMI2CADR))
        return exit_failure;
	IdleI2C();
	WriteI2C(adr);				// Address the internal register
	IdleI2C();
	WriteI2C(firstByt);			// Ask for write to FM chip
	IdleI2C();
	WriteI2C(secndByt);
	IdleI2C();
	StopI2C();
	IdleI2C();
    
	return exit_success;
}

// Checks if radio is busy or ready
unsigned char FMready(unsigned int *rdy) {

	unsigned int sts;

	if (FMread(FMCHIPSTSADR, &sts) != exit_success)
        return exit_failure;
    
	sts &= FMASKSTATUS;
	*rdy = sts ? TRUE : FALSE;
    
	return exit_success;
}

// Reads frequency from receiver and sends it to lcd
void show_freq()
{
    unsigned int channel = FMLOWCHAN;
    
    FMread(FMASKRDCHAN, &channel);
    channel += 690;
    set_lcd(channel);
    
    return;
}

void check_switches()
{
    unsigned char switches[3];
    int i;
    
    for (i = 0; i < 3; i++)
        switches = check_switch(i);
    
    if (switches[MUTE_SWITCH] != switchState[MUTE_SWITCH])
    {
        set_mute(switches[MUTE_SWITCH]);
    }
    if (switches[STEREO_SWITCH] != switchState[STEREO_SWITCH])
    {
        // TODO: Toggle stereo
    }
    if (switches[TUNE_SEEK_SWITCH] != switchState[TUNE_SEEK_SWITCH])
    {
        // TODO: Toggle seek/tune
    }
    
    switchState = switches;
}

int check_buttons()
{
    int i;
    
    for (i = 0; i < 8; i++)
    {
        if (check_button(i))
            return i;
    }
    return -1;
}

// Mutes if value is 1, else 0.
void set_mute(int value)
{
    if (value == 1)
        regImg[1] |= 0b0000000000000010; // Set hmute
    else
        regImg[1] &= 0b1111111111111101; // Unset hmute
    
    FMwrite(1);
}
