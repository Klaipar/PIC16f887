/*
 * File:   main.c
 * Author: Miguel
 *
 * Created on March 5, 2018, 5:33 PM
 */

#define _XTAL_FREQ 3400000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7





#include <xc.h>
#include "LCD.h";
#include <stdio.h>
#include <stdlib.h>

// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG


void main()
{
  int a;

  TRISB = 0b00010000;           //RB4 as Input PIN (ECHO)
  TRISD = 0x00; // LCD Pins as Output

  Lcd_Init();

  Lcd_Set_Cursor(1,1);
  Lcd_Write_String("First Test");
  Lcd_Set_Cursor(2,1);
  Lcd_Write_String("Hi Team 20!");

  __delay_ms(3000);
  Lcd_Clear();

  /*
  // LED blinking (first test)
  TRISDbits.TRISD0 = 0;  // Setting up port D0 as output
  
  // loop to make the led light blink for testing
  while(1){
      
      RD0 = 0;
      
      for(int countDelay = 0; countDelay <20; countDelay ++)__delay_ms(50);
      
      RD0 = 1;
      
      for(int countDelay = 0; countDelay <20; countDelay ++)__delay_ms(50);
      
  }
  
  */
  
  
  
  
  // LED activated with a switch (second test)
  
  TRISBbits.TRISB5 = 1;  // setting bit 5 of port B as input for the button
  TRISBbits.TRISB2 = 0;  // setting bit 2 of port B as output for the led
  
  while(1){
      
      if(PORTBbits.RB5 == 0)
      {
          
          // small delay
           for(int countDelay = 0; countDelay <20; countDelay ++)__delay_ms(5);
           if(PORTBbits.RB5 == 0)
           {
               // checking if the swith of the button still closed and if it is, turn on the led
               RB2 = 1;
           }
           
      }
      else
          RB2 = 0;   // switch the led off
      
  }
  
  
   
  
  
  // code for the detection of the height sensor
  
  
  T1CON = 0x10;                 //Initialize Timer Module

  while(1)
  {
    TMR1H = 0;                  //initial Value of Timer high
    TMR1L = 0;                  //initial Value of Timer low

    RB0 = 1;               //TRIGGER HIGH
    __delay_us(10);               //10uS Delay
    RB0 = 0;               //TRIGGER LOW

    while(!RB4);           //Waiting for Echo
    TMR1ON = 1;               //Timer Starts
    while(RB4);            //Waiting for Echo goes LOW
    TMR1ON = 0;               //Timer Stops

    a = (TMR1L | (TMR1H<<8));   //Reads Timer Value
    
    a = a/58.82;                // im converting the time received to distance for the height
    a = a + 1;
    
    //Distance Calibration
    if(a>=2 && a<=400)          //Check whether the result is valid or not as it might be out of range
    {
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String("Distance = ");
      
      Lcd_Set_Cursor(1,14);
      Lcd_Write_Char(a%10 + 48);

      a = a/10;
      Lcd_Set_Cursor(1,13);
      Lcd_Write_Char(a%10 + 48);

      a = a/10;
      Lcd_Set_Cursor(1,12);
      Lcd_Write_Char(a%10 + 48);

      Lcd_Set_Cursor(1,15);
      Lcd_Write_String("cm");
    }
    else
    {
      Lcd_Clear();
      Lcd_Set_Cursor(1,1);
      Lcd_Write_String("Out of Range");
    }
    __delay_ms(400);
  }
}