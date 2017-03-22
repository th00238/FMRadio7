/* 
 * File:   bt.h
 * Author: matthew
 *
 * Created on 22 March 2017, 14:45
 */

#ifndef BT_H
#define	BT_H

#ifdef	__cplusplus
extern "C" {
#endif


int bt_tx_ready();
void bt_write_char(char ch);
void bt_write_string(char *string);

#ifdef	__cplusplus
}
#endif

#endif	/* BT_H */

