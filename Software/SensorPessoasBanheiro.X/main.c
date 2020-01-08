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


#define _XTAL_FREQ 8000000                      

void interrupt interrupts();

int dist = 0;

void main(void) 
{
    OSCCON = 0b01110000;

    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0b00010000;                         //ativando RA5 como saida (trigger) e RA4 como entrada(echo)
    TRISB = 0;
    TRISC = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;    
    IOCAbits.IOCA4 = 1;                         
           
    INTCON = 0b11001000;                        
    
    T1CON = 0b00010000;                         

    int i = 0;
    int j = 0;

    while(1)
    {
        for (i=0;i<=8;i++)
        {        
            TMR1 = 0;
            PORTAbits.RA5 = 1;                               //TRIGGER HIGH
            __delay_us(10);                                  
            PORTAbits.RA5 = 0;                               //TRIGGER LOW 
            __delay_ms(100);                                 //ECHO
            dist = dist + 1;                                 
            if (dist <= 100)
                j++;
            __delay_ms(200);            
        }
        
        if (j>2)
        {
            PORTCbits.RC5 = 1; 
            j = 0;
        }
        else
        {
            PORTCbits.RC5 = 0;
            j = 0;
        }
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