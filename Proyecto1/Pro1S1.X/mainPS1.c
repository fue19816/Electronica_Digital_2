/* 
 * File:   mainPS1.c
 * Author: Oscar Fuentes
 *
 * Created on 2 de septiembre de 2021, 03:12 PM
 */

//------------------------------------------------------------------------------
//                      Implementación de librerias                             
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "I2C.h"

//------------------------------------------------------------------------------
//                     Configuración del PIC                            
//------------------------------------------------------------------------------

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 8000000
//------------------------------------------------------------------------------
//                      Variables                             
//------------------------------------------------------------------------------
uint8_t valor, z, dato;

//------------------------------------------------------------------------------
//                      Prototipo de funciones                             
//------------------------------------------------------------------------------
void setup(void);               //Se declaran todas las funciones a usar

void __interrupt() Interrupciones(void){
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            //PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = dato;              //Se envía la variable dato por I2C
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

//------------------------------------------------------------------------------
//                      Ciclo principal                            
//------------------------------------------------------------------------------
void main(void) {
    setup();    //Se llama a las configuraciones del pic
    //--------------------------------------------------------------------------
    //                      Loop principal                                      
    //--------------------------------------------------------------------------
    while(1){
        if(PORTAbits.RA0 == 0){
            valor = 250;
            dato = 1;
        }
        else{
            valor = 128;
            dato = 0;
        }
        
        CCPR1L = valor;        //El registro del PWM empieza 
                                              //desde 128 a 256
        //CCP1CONbits.DC1B1 = valor & 0b01; //Se colocan los bits menos sign.
        //CCP1CONbits.DC1B0 = valor>>7;
        
        
    }
    
}

//------------------------------------------------------------------------------
//                      Configuración                            
//------------------------------------------------------------------------------

void setup(void){
    //Configuración de puertos
    ANSEL = 0x00;           //Se configura las entradas analogicas
    ANSELH = 0x00;
    
    TRISA = 0x01;           //El puerto B, C, D y E son salida, mientras que en
    TRISB = 0x00;           //B son entradas
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;

    PORTC = 0x00;           //Se limpian todos los puertos que son salida
    PORTD = 0x00;
    PORTE = 0x00;
    
    I2C_Slave_Init(0x60);
    
    //Configuración del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
    
    //Configuración de interrupciones
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1;
    
    //-------------------Configuración del PWM----------------------------------
    TRISCbits.TRISC2 = 1;            //Se habilitan el pin el servo
    PR2 = 250;                      //Se coloca el registro de PR2 para 2ms
    
    //CCP1
    CCP1CONbits.P1M = 0;            //Se utiliza una salida individual
    CCP1CONbits.CCP1M = 0b00001100; //Se utiliza la función de PWM
    CCPR1L = 0x0F;                  //El registro empieza en 15
    CCP1CONbits.DC1B = 0;           //Los bits menos sign se colocan en 0
    
    PIR1bits.TMR2IF = 0;            //Se habilita la interr. del tmr2
    T2CONbits.T2CKPS = 0b11;        //El prescaler es de 16
    T2CONbits.TMR2ON = 1;           //Se enciende el TMR2
    
    while (!PIR1bits.TMR2IF);       //Cuando se cumple un ciclo del TMR2
    PIR1bits.TMR2IF = 0;            //Se limpia la bandera
    TRISCbits.TRISC2 = 0;           //Los pines del puerto C se colocan en 0
    
    //--------------------------------------------------------------------------
}
