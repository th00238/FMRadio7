/*
 * File:   taki.c
 * Author: takahikohasegawa
 *
 * Created on February 14, 2017, 12:39 PM
 */




void set_lcd(int khz){
    //if khz is zero, clear screen 
}
int check_button(int button){
    int ReadPortA,ReadPortE,ReadPortG, Mask, ButtonNumber;      //8 buttons RA0 RA1 RE4 RE5 RE6 RE7 RG3 RG4
    
    ReadPortA = PORTA; 
    ReadPortE = PORTE;
    ReadPortG = PORTG;
    
    
    switch (ButtonNumber){
        
            case 0: 
                Mask = 0b00000001; //PORTA
                ButtonNumber = Mask & ReadPortA;   //And it to isolate 
                break;
                
            case 1: 
                Mask = 0b00000010; //PORTA
                ButtonNumber = Mask & ReadPortA;   //And it to isolate 
                break;
            case 2:
                Mask = 0b00000100; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 3:
                Mask = 0b00001000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 4:
                Mask = 0b00010000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 5:
                Mask = 0b00100000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 6:
                Mask = 0b01000000; //PORTG
                ButtonNumber = Mask & ReadPortG;   //And it to isolate 
                break;
            case 7:
                Mask = 0b10000000; //PORTG
                ButtonNumber = Mask & ReadPortG;   //And it to isolate 
                break;
            default: 
                ButtonNumber = 0;
                break;
    }
                  
                
    return ButtonNumber; 
}

int check_switch(int switchn){
    int Read, Mask, Store = 0;       // 3 switches excluding the top one 
    
    Read = PORTG;  //Reading PortG
    switch (Store)
    {
        case 0:
            Mask = 0b00000001;            //Mask off with the value
            Store = Mask & Read;   //And it to isolate 
            break;
    
        case 1:    
            Mask = 0b00000010;
            Store = Mask & Read; 
            break; 
        case 2:
            Mask = 0b00000100;
            Store = Mask & Read; 
            break;
        default: 
            Store = 0;
            break; 
    }
    return Store;
 
}
