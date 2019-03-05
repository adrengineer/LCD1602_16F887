/*
 * File:   LCD1602main.c
 * Author: Adrengineer
 *
 * Created on 1 March 2019, 11:30
 */

#include <xc.h>
#include <stdio.h>
#include "LCD1602.h"
#include "config_LCD1602.h"


int t;
void main (){
    
     LCD_Init();
     LCD_Command(CURSOR_ON_BLINK);
     
     LCD_Putc(0x68);
     LCD_Putc(0x68);
     LCD_Command(CURSOR_RIGHT );
     LCD_Print("ADRENGINEER");
     
     __delay_ms(200);
     
     while(1){
         LCD_Command(LCD_SECOND_ROW);
         printf("ADRENGINEER %d",t);
         t++;
         __delay_ms(200);
     }
  
}