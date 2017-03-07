/* 
 * File:taki.h
 * Author: Matthew Mould and Takahiko Hasegawa
 * Comments:
 * Revision history: 0.1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_TAKI_H
#define	XC_TAKI_H

// Switches 0 1 2 are RG0 RG1 RG2
// Buttons 0 1 2 3 4 5 6 7 are RA0 RA1 RE4 RE5 RE6 RE7 RG3 RG4
// 4 Switches including power switch
// Switch 1 (0) = stereo/ mono
// Switch 2 (1) = Mode 
// Switch 3 (2) = Mute 
// Switch 4 (3) = Power, no to be programmed 
// Button 0&1 = Volume 
// Button 2&3 = Channel 
// Button 4&5, 6, 7 = Storage button 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void set_lcd(int khz); //in 100 khz
void ChangeSegment(char segNumber, int LCDSegment);
int check_button(int button);
int check_switch(int switchn);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_TAKI_H */

