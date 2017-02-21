/* 
 * File:   fm.h
 * Author: Matthew Mould
 *
 * Created on 21 February 2017, 10:28
 */

#ifndef FM_H
#define	FM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define exit_success	0			// Exit success
#define exit_failure	1			// Exit fail

#define FMI2CADR		0x20		// Address (for writes) of FM module on I2C bus


#define DEVRD			0x01		// Read not write an I2C device 
#define FMCHIPVERSADR	0x1C		// Address of FM chip version
#define FMCHIPIDADR		0x1B		// Address of FM chip ID  
#define FMCHIPSTSADR	0x13		// Address of FM chip status
#define FMCHIPVOLADR    0x03

#define FMHIGHCHAN		(1080-690)	// Highest FM channel number
#define FMLOWCHAN		(875-690)
#define FALSE			0
#define TRUE			1

enum {							// Global error numbers
	GERNONE, 					// No error
	GERWCOL,					// I2C write collision
	GERFINT,					// Could not initialize FM module
	GERFMID						// Could not read chip ID (0x1010)
};



#ifdef	__cplusplus
}
#endif

#endif	/* FM_H */

