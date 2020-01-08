/*
 * File:   main.c
 * Author: Guilherme Gotardo
 *
 * lul
 * 
 * Created on 23 de Junho de 2017, 22:04
 */


#include <xc.h>
#include <stdio.h>
#include "config.h"
#include "lcd_writing.c"

#define _XTAL_FREQ 8000000                      

void interrupt interrupts();

int dist = 0;

void main(void) 
{
    OSCCON = 0b01110000;

    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0b00010000;                         
    TRISB = 0;
    TRISC = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;    
    IOCAbits.IOCA4 = 1;                         
           
    INTCON = 0b11001000;                        
    
    T1CON = 0b00010000;                         
    char aux = 0;
    int i = 0;
    int j = 0;
            
    //lcd_init();
    //_delay_ms(100);
    //printf("lul");

    while(1)
    {    

        /*if(dist>=2 && dist<=400)                         //Limitar distancias devido ao tmr1 senão da estouro de bit
        {
          lcd_clear();
          printf("Distancia = ");
          printf("%d cm",dist);
        }
        else
        {
          lcd_clear();
          printf("No sorry, nothing");
        }*/
       
         TMR1 = 0;
         PORTAbits.RA5 = 1;                               //TRIGGER HIGH
         __delay_us(10);                                  
         PORTAbits.RA5 = 0;                               //TRIGGER LOW 
         __delay_ms(100);                                 //ECHO
         dist = dist + 1;                                 

        if (dist <= 120)
        {
              PORTCbits.RC5 = 1;
              aux = 1;          
        }
        else
            PORTCbits.RC5 = 0;
    }
}

void interrupt interrupts()
{
    PORTA = PORTA;
    
    if(INTCONbits.RABIF && INTCONbits.RABIE && IOCbits.IOCA4)                       
      {
        INTCONbits.RABIE = 0;                           
        
        if(PORTAbits.RA4)                               
            T1CONbits.TMR1ON = 1;                       
        
        if (!PORTAbits.RA4)                             
        {
          T1CONbits.TMR1ON = 0;                         
          dist = TMR1/58.82;                            
        }   
      }
      INTCONbits.RABIF = 0;                            
      INTCONbits.RABIE = 1;                            

}