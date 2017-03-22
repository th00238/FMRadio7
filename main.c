/*
 * File:   main.c
 * Author: Matthew Mould, Takahiko Hasegawa
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
	0x0000,     // R1.   
	0x00B9,     // R2.   
	0x8410,     // R3   seekTHD = 16   
	0x0000,     // R4   
	0x0000,     // R5   
	0x0000,     // R6   
	0x0000,     // R7   
	0x0000,     // R8   
	0x0000,     // R9   
	0x0008,     // R10  enable wrap   
	0x0000,     // R11.  
	0x0000,     // R12.   
	0xB845,     // R13   
	0xFC2D,     // R14   
	0x8097,     // R15   
	0x04A1,     // R16   
	0xDF6A      // R17
};

unsigned int regImg[18];	// FM register bank images
unsigned char switchState[3] = {2, 2, 2}; //Previous state of switches
unsigned int seek = 1; // Weather to seek of tune

void initialise(void);
unsigned char FMread(unsigned char regAddr, unsigned int *data);
unsigned char FMvers(unsigned int *vsn);
void fm_error(void);
void dly(int d);
unsigned char FMinit(void);
unsigned char FMwrite(unsigned char adr);
unsigned char FMready(unsigned int *rdy);
void show_freq(void);
void check_switches(void);
int check_buttons(void);
void set_stereo(unsigned int value);
void set_mute(unsigned int value);
void set_volume(unsigned int inc);
void tune(char dir);
void seekChan(char dir);

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
        {
            set_lcd(button_pressed); // DEBUG
            
            switch (button_pressed)
            {
                case VOLUP_BUTTON:
                    set_volume(1);
                    break;
                case VOLDOWN_BUTTON:
                    set_volume(0);
                    break;
                case TUNEUP_BUTTON:
                    if (seek)
                        seekChan(1);
                    else
                        tune(1);
                    break;
                case TUNEDOWN_BUTTON:
                    if (seek)
                        seekChan(0);
                    else
                        tune(0);
                    break;
                case PRES1_BUTTON:
                    // TODO
                    break;
                case PRES2_BUTTON:
                    // TODO
                    break;
                case PRES3_BUTTON:
                    // TODO
                    break;
                case PRES4_BUTTON:
                    // TODO
                    break;
                default:
                    break;
            }
        }
    }
    
    return;
}

void fm_error()
{
    set_lcd(9999);
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
	LCDSE3 = 0b11111111;        	// Disable LCD segments 31-24
	LCDCON = 0b10001000;         	// Enab LC controller. Static mode. INTRC clock
	LCDPS  = 0b00110110;         	// 37 Hz frame frequency
    
	ADCON1 = 0b00111111;        	// Make all ADC/IO pins digital
	TRISA = 0b00000011;             // RA0 and RA1 buttons 0 1
    TRISB = 0b00000000;
	TRISC = 0b11011000;				// RC3 and RC4 do the I2C bus, RC6 RC7 EUART
    TRISE = 0b11110000;              // buttons 2 3 4 5
	TRISG = 0b11111111;				// RG0, RG1 & RG2 switches and 6 7 buttons
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
    
    TXSTA1 = 0b10100x10;
    RCSTA1 = 0b10010000;            // Enable EUSART
    
    INTCONbits.TMR0IF = 0;          // Clear timer flag
	//T0CON = 0b00000011;				// Prescale by 16
    T0CON = 0b00001000;             // No prescale
    TMR0H = 0;                      // Clear timer count
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // Start timer
	OpenI2C( MASTER, SLEW_OFF);
	SSPADD = 0x3F;
    
    //DEBUG
    LCDDATA0 = 0b11111111;
    LCDDATA1 = 0b11111111;
    LCDDATA2 = 0b11111111;
    LCDDATA3 = 0b11111111;
    LCDDATA4 = 0b11111111;
    LCDDATA5 = 0b11111111;
    LCDDATA6 = 0b11111111;
    LCDDATA7 = 0b11111111;
    LCDDATA8 = 0b11111111;
    LCDDATA9 = 0b11111111;
    LCDDATA10 = 0b11111111;
    LCDDATA11 = 0b11111111;
    LCDDATA12 = 0b11111111;
    LCDDATA13 = 0b11111111;
    LCDDATA14 = 0b11111111;
    LCDDATA15 = 0b11111111;
    LCDDATA16 = 0b11111111;
    LCDDATA17 = 0b11111111;
    LCDDATA18 = 0b11111111;
    LCDDATA19 = 0b11111111;
    LCDDATA20 = 0b11111111;
    LCDDATA21 = 0b11111111;
    LCDDATA22 = 0b11111111;
    LCDDATA23 = 0b11111111;
    

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
    
	for(ad = 1; ad < 18; ad++)
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
    
    FMread(FMCHIPSTSADR, &channel);
    channel >>= 7;
    channel += 690;
    set_lcd(channel);
    
    return;
}

void check_switches()
{
    unsigned char switches[3];
    int i;
    
    for (i = 0; i < 3; i++)
        switches[i] = check_switch(i);
    
    if (switches[MUTE_SWITCH] != switchState[MUTE_SWITCH])
    {
        set_mute(switches[MUTE_SWITCH]);
    }
    if (switches[STEREO_SWITCH] != switchState[STEREO_SWITCH])
    {
        set_stereo(switches[STEREO_SWITCH]);
    }
    if (switches[TUNE_SEEK_SWITCH] != switchState[TUNE_SEEK_SWITCH])
    {
        seek = switches[TUNE_SEEK_SWITCH];
    }
    
    for (i = 0; i < 3; i++)
        switchState[i] = switches[i];
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
void set_mute(unsigned int value)
{
    if (value == 1)
        regImg[1] |= 0b0000000000000010; // Set hmute
    else
        regImg[1] &= 0b1111111111111101; // Unset hmute
    
    FMwrite(1);
}

void set_stereo(unsigned int value)
{
    if (value == 0)
        regImg[1] |= 0b0000000000001000; // Set Mono
    else
        regImg[1] &= 0b1111111111110111; // Unset Mono
    
    FMwrite(1);
}

// Increases volume if inc is 1, else decreases
void set_volume(unsigned int inc)
{
    unsigned int volume = 0;
    int i;
    
    // Read volume
    FMread(FMCHIPVOLADR, &volume);
    volume &= 0b0000011110000000;
    volume >>= 7;
    
    // Set and write volume
    if (inc)
        volume++;
    else
        volume--;
    
    // Set volume bits
    for (i = 0; i < 4; i++)
    {
        // Test bit value
        if ((volume >> i) & 1) // If bit i in volume is 1
            regImg[3] |= 1 << (i + 7);
        else
            regImg[3] &= ~(1 << (i + 7));
    }
    
    FMwrite(3);
    return;
}

// Increases freq by 200khz. Increases if dir is 1.
void tune(char dir)
{
    unsigned int channel = FMLOWCHAN;
    unsigned int stc = 0;
    
    // Get current channel;
    FMread(FMCHIPSTSADR, &channel);
    channel >>= 7;
    
    // Set channel
    if (dir)
        channel += 2;
    else
        channel -= 2;
    
    set_mute(1);
    
    regImg[2] &= 0b1111110111111111; // Clear tune bit
    FMwrite(2);
    
    regImg[3] &= 0b1011111111111111; // Clear seek bit
    FMwrite(3);
    
    regImg[2] = channel; // Set channel
    FMwrite(2);
    
    regImg[2] |= 0b0000001000000000; // Set tune bit
    FMwrite(2);
    
    do
    {
        dly(20);
        FMread(FMCHIPSTSADR, &stc);
        stc >>= 5;
        stc &= 1; // Isolate stc bit
    } while (!stc);
    
    if (switchState[MUTE_SWITCH] == 0)
        set_mute(0); // Unmute
    
    // set_lcd(channel + 690);
    return;
}

void seekChan(char dir)
{
    unsigned int channel = FMLOWCHAN;
    unsigned int stc = 0;
    
    // Get current channel;
    FMread(FMCHIPSTSADR, &channel);
    channel >>= 7;
    
    set_mute(1);
    
    regImg[2] &= 0b1111110111111111; // Clear tune bit
    FMwrite(2);
    
    regImg[2] = channel; // Set channel
    FMwrite(2);
    
    regImg[3] &= 0b1011111111111111; // Clear seek bit
    FMwrite(3);
    
    if (dir)
        regImg[3] |= 0b1000000000000000; // Set SEEKUP
    else
        regImg[3] &= 0b0111111111111111; // Clear SEEKUP
    FMwrite(3);
    
    regImg[3] |= 0b0100000000000000; // Set seek bit
    FMwrite(3);
    
    do
    {
        dly(20);
        FMread(FMCHIPSTSADR, &stc);
        // set_lcd((stc >> 7) + 690);
        stc >>= 5;
        stc &= 1; // Isolate stc bit
    } while (!stc);
    
    if (switchState[MUTE_SWITCH] == 0)
        set_mute(0); // Unmute
    
    // set_lcd(channel + 690);
    return;
}