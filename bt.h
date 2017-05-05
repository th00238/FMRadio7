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

void serial_init(void);
int bt_tx_ready(void);
void bt_write_char(char ch);
void bt_write_string(char *string);
void putch(char ch);

#ifdef	__cplusplus
}
#endif

#endif	/* BT_H */

