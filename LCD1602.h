/* 
 * File:   LCD1602.h
 * Author: Adrengineer
 *
 * Created on 1 March 2019, 11:30
 */

#ifndef LCD1602_H
#define	LCD1602_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "config_LCD1602.h"


//#define _8bits //************************************************************

#ifdef  _8bits
    #define lcd_rs          PORTCbits.RC0
    #define lcd_rw          PORTCbits.RC1
    #define lcd_enable      PORTCbits.RC2
    #define lcd_data        PORTD

    #define DB0             PORTDbits.RD0
    #define DB1             PORTDbits.RD1
    #define DB2             PORTDbits.RD2
    #define DB3             PORTDbits.RD3
#endif
// As defined in the following structure the pin connection is as follows:
#define lcd_rs          PORTDbits.RD0
#define lcd_rw          PORTDbits.RD1
#define lcd_enable      PORTDbits.RD2
#define lcd_data        PORTD
//   LCD pins D0-D3 are not used and PIC B3 is not used.   
#define DB4             PORTDbits.RD4
#define DB5             PORTDbits.RD5
#define DB6             PORTDbits.RD6
#define DB7             PORTDbits.RD7

/* The following are used for setting the I/O port direction register.*/

#define WR_MASK   0b00000100 //LCD_enable
#define LCD_WRITE 0b00000000//struct lcd_pin_map const LCD_WRITE = {0,0,0,0,0}; // For write mode all pins are out
#define LCD_READ 0b00001111 //struct lcd_pin_map const LCD_READ = {0,0,0,0,15}; // For read mode data pins are in

char lcdline;

    

#define     LCD_FIRST_ROW           128 //0x80
#define     LCD_SECOND_ROW          192 //0xc0
#define     LCD_THIRD_ROW           148
#define     LCD_FOURTH_ROW          212
    
#define     LCD_TURN_OFF            0x00 //0
#define     LCD_TURN_ON             0x08 //8
    
#define     CLEAR                   0x01 //1.53 ms
#define     RETURN_HOME             0x02 //1.53 ms
    
#define     DISPLAY_OFF             0x08 //Display off, but data remain in DDRAM
#define     CURSOR_OFF              0x0C //Display on, but cursor off
#define     CURSOR_ON               0x0E //Display on, Cursor On
#define     CURSOR_ON_BLINK         0x0F //Display on, Cursor On Blinking
    
#define     CURSOR_LEFT             0x10 //Shift cursor to left and decrease
#define     CURSOR_RIGHT            0x14 //Shift cursor to right and increase
#define     SHIFT_LEFT              0x18 //Shift all LCD to left
#define     SHIFT_RIGHT             0x1C //Shift all LCD to right
    
#define     _8b_1L_5x8              0x30 //8 bits mode, 1 line, 5x8 dots
#define     _8b_2L_5x8              0x38 //8 bits mode, 2 line, 5x8 dots 
#define     _8b_1L_5x11             0x34 //8 bits mode, 1 line, 5x8 dots 
#define     _8b_2L_5x11             0x3C //8 bits mode, 2 line, 5x8 dots 
#define     _4b_1L_5x8              0x20 //4 bits mode, 1 line, 5x11 dots 
#define     _4b_2L_5x8              0x28 //4 bits mode, 2 line, 5x11 dots 
#define     _4b_1L_5x11             0x24 //4 bits mode, 1 line, 5x11 dots 
#define     _4b_2L_5x11             0x2C //4 bits mode, 2 line, 5x11 dots 
   


void LCD_Init();
void LCD_Bus(char n);
void LCD_Parameter(char data);
void LCD_Command(char cmd);
void LCD_Gotoxy( unsigned char x, unsigned char y);
void LCD_Putc( unsigned char c);
void LCD_Print(unsigned char *dato);
char LCD_Getc( unsigned char x, unsigned char y);//This function is not checked
char lcd_read_byte();//This function is not checked
void putch(char p);





/** ****************************************************************************
 * @Brief: This function initilaizes ports and lcd in mode 4&8 bits
 * @Param
 * @Returm
 * ************************************************************************* **/
void LCD_Init(){
    
    #ifdef _8bits
        TRISC = 0x00;
        PORTC = 0x00;
    #endif
    TRISD = LCD_WRITE;         
    PORTD = 0x00;
    lcd_rs = 0;         
    lcd_rw = 0;
    lcd_enable = 0;          
    
    __delay_ms(15);        
    
    LCD_Command(0x30);    
    __delay_ms(5);         
    LCD_Command(0x30);
    __delay_us(100);
    LCD_Command(0x30);
    __delay_us(40);
    
    LCD_Command(0x02);              
    __delay_us(40);
    
    #ifdef _8bits    //8 bits data bus
        LCD_Command(_8b_2L_5x8);      
        __delay_us(40);
    #else            //4 bits data bus
        LCD_Command(_4b_2L_5x8);      
        __delay_us(40);
    #endif

    
    LCD_Command(0x01);              
    __delay_ms(2);
    LCD_Command(0x0C);      
    __delay_us(40);
    LCD_Command(0x06);      
    __delay_us(40);
}

/** ****************************************************************************
 * @Brief: This function writes data in the LCD bus
 * @Param n: data to write
 * @Returm
 * ************************************************************************* **/
void LCD_Bus(char n){
    #ifdef _8bits
        PORTD = n;
        PORTC &= ~WR_MASK;
        PORTC |=  WR_MASK;
        __delay_us(100); 
        return;
    #endif

    PORTD = (PORTD & 0B00001111) | ((n) & 0b11110000);
    PORTD &= ~WR_MASK;
    PORTD |=  WR_MASK;
    __delay_us(100); 
    
    PORTD = (PORTD & 0B00001111) | (n<<4);
    PORTD &= ~WR_MASK;
    PORTD |=  WR_MASK;
    __delay_us(100); 

}

/** ****************************************************************************
 * @Brief: This function writes a command in the LCD bus
 * @Param cmd: command to write
 * @Returm
 * ************************************************************************* **/
void LCD_Command(char cmd){
     lcd_rs = 0;
     __delay_us(100); //Delay needed to avoid send a data whilst controller is busy
     lcd_rw = 0;
     
     LCD_Bus(cmd);
}

/** ****************************************************************************
 * @Brief: This function writes a parameter in the bus
 * @Param data: parameter to write
 * @Returm
 * ************************************************************************* **/
void LCD_Parameter(char data){
     lcd_rs = 1;//rs
     __delay_us(100); //Delay needed to avoid send a data whilst contoller is busy
     lcd_rw = 0;
    
     LCD_Bus(data);
}

/** ****************************************************************************
 * @Brief: This function sets the pointer a specific location
 * @Param x: x location
 *        y: y location
 * @Returm
 * ************************************************************************* **/
void LCD_Gotoxy( unsigned char x, unsigned char y) {
    char address;

    switch(y) {
        case 1 : address=0x80;break;
        case 2 : address=0xc0;break;
        case 3 : address=0x94;break;
        case 4 : address=0xd4;break;
    }
    address+=x-1;
    LCD_Command(address);
}

/** ****************************************************************************
 * @Brief: This function sends a character to LCD. will display c on the next position 
 *      of the LCD. The following have special meaning:            
 *      \f  Clear display                             
 *      \n  Go to start of second line              
 *      \b  Move back one position 
 * @Param c: character to send
 * @Returm
 * ************************************************************************* **/
void LCD_Putc(char c) {
   switch (c) {
     case '\f': LCD_Command(1);
        lcdline=1;
        __delay_ms(2);
        break;
     case '\n': LCD_Gotoxy(1,++lcdline);
        break;
     case '\b': LCD_Command(0x10);       
        break;
     default: LCD_Parameter(c);        
        break;
   }
}

/** ****************************************************************************
 * @Brief: This function print a string
 * @Param *dato: string to print on the LCD
 * @Returm
 * ************************************************************************* **/
void LCD_Print(unsigned char *dato){
    while (*dato) // Whilst it's not null
    {
        LCD_Parameter(*dato); 
        dato++; 
    }
}

/** ****************************************************************************
 * @Brief:  WITH THIS FUNCTION AND STDIO LIBRARY IS POSIBLE USE PRINTF
 * @Param p: parameter to write
 * @Returm
 * ************************************************************************* **/
void putch(char p){
    
    LCD_Parameter(p);
}


/** ****************************************************************************
 * @Brief: This function get character from LCD memory
 * @Param x: x location of the desirable character
 *        y: y location of the desirable character
 * @Returm the desirable character
 * ************************************************************************* **/
//next functions are not checked
char LCD_Getc( unsigned char x, unsigned char y) {
   char value;

    LCD_Gotoxy(x,y);
    lcd_rs=1;
    value = lcd_read_byte();
    lcd_rs=0;
    return(value);
}
//next functions are not checked
char lcd_read_byte() {
      char low,high;

      TRISD = LCD_READ;
      lcd_rw = 1;
      Delay1TCYx(1);
      lcd_enable = 1;
      Delay1TCYx(1);
      high = lcd_data;
      lcd_enable = 0;
      Delay1TCYx(1);
      lcd_enable = 1;
      __delay_us(1);
      low = lcd_data;
      lcd_enable = 0;
      TRISD=LCD_WRITE;
      return( (high<<4) | low);
}


#ifdef	__cplusplus
}
#endif

#endif	/* LCD1602_H*/

