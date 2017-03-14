/*
 * File:   taki.c
 * Author: takahikohasegawa
 *
 * Created on February 14, 2017, 12:39 PM
 */
#include <p18cxxx.h>

void Segment0(char segNumber);
void Segment1(char segNumber);
void Segment2(char segNumber);
void Segment3(char segNumber);

void ChangeSegment(char segNumber, int LCDSegment)
{
   /* int LCDDATA0 = 0b11111111; // turn on LCD Screen
    int LCDDATA1 = 0b11111111;
    int LCDDATA2 = 0b11111111;
    int LCDDATA3 = 0b11111111;*/

            //This function called to switch between sub-functions
        switch (LCDSegment)
        {
            case 0:
                Segment0(segNumber);
                break;
            case 1:
                Segment1(segNumber);
                break;
            case 2:
                Segment2(segNumber);
                break;
            case 3: 
                Segment3(segNumber);
                break;
                
        }
         
}
void set_lcd(int khz){
    
    int i;
    for (i=0; i<=3; i++)
    {
        if (khz != 0 )
        {
            ChangeSegment((khz%10), i);
             // divide by 10 and get the remainder 
            khz /= 10;
        }
        else
            ChangeSegment(0, i);
        
    }

}
void Segment0(char segNumber)
{
    //switch between 9 numbers 
    switch (segNumber)
    {
        case 0:
            LCDDATA0 = 0b10111111;
            break;
            
        case 1:
            LCDDATA0 = 0b10001100;
            break;
        case 2:
            LCDDATA0 = 0b11011011;
            break;
        case 3:
            LCDDATA0 = 0b11011110;
            break;
        case 4:
            LCDDATA0 = 0b11011110;
            break; 
        case 5:
            LCDDATA0 = 0b11110110;
            break; 
        case 6:
            LCDDATA0 = 0b11110111;
            break;
        case 7:
            LCDDATA0 = 0b10011100;
            break;
        case 8:
            LCDDATA0 = 0b11111111;
            break;
        case 9:
            LCDDATA0 = 0b11111100;
            break;

    }
            
}
void Segment1(char segNumber)
{
    switch (segNumber){
        case 0:
            LCDDATA1 = 0b10111111;
            break;
            
        case 1:
            LCDDATA1 = 0b10001100;
            break;
        case 2:
            LCDDATA1 = 0b11011011;
            break;
        case 3:
            LCDDATA1 = 0b11011110;
            break;
        case 4:
            LCDDATA1 = 0b11011110;
            break; 
        case 5:
            LCDDATA1 = 0b11110110;
            break; 
        case 6:
            LCDDATA1 = 0b11110111;
            break;
        case 7:
            LCDDATA1 = 0b10011100;
            break;
        case 8:
            LCDDATA1 = 0b11111111;
            break;
        case 9:
            LCDDATA1 = 0b11111100;
            break;
    }
}

void Segment2(char segNumber)
{
    switch (segNumber){
        case 0:
            LCDDATA2 = 0b10111111;
            break;
            
        case 1:
            LCDDATA2 = 0b10001100;
            break;
        case 2:
            LCDDATA2 = 0b11011011;
            break;
        case 3:
            LCDDATA2 = 0b11011110;
            break;
        case 4:
            LCDDATA2 = 0b11011110;
            break; 
        case 5:
            LCDDATA2 = 0b11110110;
            break; 
        case 6:
            LCDDATA2 = 0b11110111;
            break;
        case 7:
            LCDDATA2 = 0b10011100;
            break;
        case 8:
            LCDDATA2 = 0b11111111;
            break;
        case 9:
            LCDDATA2 = 0b11111100;
            break;
    }
}

void Segment3(char segNumber)
{
    switch (segNumber){
        case 0:
            LCDDATA3 = 0b10111111;
            break;
            
        case 1:
            LCDDATA3 = 0b10001100;
            break;
        case 2:
            LCDDATA3 = 0b11011011;
            break;
        case 3:
            LCDDATA3 = 0b11011110;
            break;
        case 4:
            LCDDATA3 = 0b11011110;
            break; 
        case 5:
            LCDDATA3 = 0b11110110;
            break; 
        case 6:
            LCDDATA3 = 0b11110111;
            break;
        case 7:
            LCDDATA3 = 0b10011100;
            break;
        case 8:
            LCDDATA3 = 0b11111111;
            break;
        case 9:
            LCDDATA3 = 0b11111100;
            break;
        /*case 10:
            LCDDATA3 = 0b00000001;
            break;*/
            
    }
}
int check_button(int button){
    int ReadPortA,ReadPortE,ReadPortG, Mask, ButtonNumber;      //8 buttons RA0 RA1 RE4 RE5 RE6 RE7 RG3 RG4
    
    ReadPortA = PORTA; 
    ReadPortE = PORTE;
    ReadPortG = PORTG;
    
    
    switch (button){
        
            case 0: 
                Mask = 0b00000001; //PORTA
                ButtonNumber = Mask & ReadPortA;   //And it to isolate 
                break;
                
            case 1: 
                Mask = 0b00000010; //PORTA
                ButtonNumber = Mask & ReadPortA;   //And it to isolate 
                break;
            case 2:
                Mask = 0b00010000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 3:
                Mask = 0b00100000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 4:
                Mask = 0b01000000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 5:
                Mask = 0b10000000; //PORTE
                ButtonNumber = Mask & ReadPortE;   //And it to isolate 
                break;
            case 6:
                Mask = 0b00001000; //PORTG
                ButtonNumber = Mask & ReadPortG;   //And it to isolate 
                break;
            case 7:
                Mask = 0b00010000; //PORTG
                ButtonNumber = Mask & ReadPortG;   //And it to isolate 
                break;
            default: 
                ButtonNumber = 0;
                break;
    }
                  
                
    return !ButtonNumber; 
}

int check_switch(int switchn){
    int Read, Mask, Store = 0;       // 3 switches excluding the top one 
    
    Read = PORTG;  //Reading PortG
    switch (switchn)
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
