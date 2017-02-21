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
    int b0=0, b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0; //8 buttons
    
    
            
    
    return 0;  
}

int check_switch(int switchn){
    int Read, Mask, Store;       // 3 switches excluding the top one 
    
    Read = PORTG;                //Reading PortG
    Mask = 00000001;            //Mask off with the value
    Store = Mask & Read;        //And it to isolate 
    
    if (Store != 1)
    {
        Mask = 00000010;
        Store = Mask & Read; 
        if (Store != 1)
        {
           Mask = 00000100;
           Store = Mask & Read; 
           if (Store != 1)
           {
               Store = 0; 
           }
        }
        
    }
    return Store;
   
    
    
    
    
    
    
}
