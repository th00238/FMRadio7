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

void set_lcd(int khz);
int check_button(int button);
int check_switch(int switchn);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_TAKI_H */

