// Check the TRMT register
int bt_tx_ready()
{
    int trmt = TXSTA1;
    trmt >>= 1;
    trmt &= 1;
    return trmt;
}

void bt_write_char(char ch)
{
    while (!bt_tx_ready)
        continue; // Wait unit ready to TX.
    
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