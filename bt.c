#include <p18cxxx.h>

// Setup EUSART
void serial_init()
{
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TX9 = 0;
    TXSTA1bits.TXEN = 1;
    
    RCSTA1bits.RX9 = 0;
    RCSTA1bits.CREN = 1;
    RCSTA1bits.SPEN = 1;
    
    BAUDCON1bits.BRG16 = 0;
    TXSTA1bits.BRGH = 0;
    SPBRG1 = 12;                    // Baud 9600
    
    return;
}

void bt_write_char(char ch)
{
    while (TXSTA1bits.TRMT == 0); // Wait unit ready to TX.
    
    TXREG1 = ch;
    return;
}

void bt_write_string(char *string)
{
    int i;
    
    for (i = 0; string[i]; i++)
        bt_write_char(string[i]);
    
    return;
}

// For printf
void putch(char ch)
{
    bt_write_char(ch);
}

// Check received data
void bt_receive()
{
    char data = 0;
    if (PIR1bits.RC1IF)
    {
        data = RCREG1;
    }
}