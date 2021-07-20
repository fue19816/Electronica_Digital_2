//------------------------------------------------------------------------------
//                      Implementación de librerias                             
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "valorADC.h"
#include "segm.h"

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
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 8000000
//------------------------------------------------------------------------------
//                      Variables                             
//------------------------------------------------------------------------------

unsigned int display1, display2;
unsigned int d = 0;                       //Se declara la variable de multipl.
signed int res;                       //del display
uint8_t  valadc, var, bits_sign, resto_bits, valor1, valor2;


//------------------------------------------------------------------------------
//                      Prototipo de funciones                             
//------------------------------------------------------------------------------
void setup(void);               //Se declaran todas las funciones a usar

void __interrupt() Interrupciones(void){
    if (PIR1bits.ADIF == 1){
        PIR1bits.ADIF = 0;
        if (ADCON0bits.CHS == 0){
           var = valorADC(valadc);
           bits_sign = 0x0F & (var >> 4); //Se obtienen los bits más sign.
           resto_bits = 0x0F & var;       //Se obtienen los bits menso sign.
        }
    }
    if(INTCONbits.RBIF){            //Si se enciende la bandera del puerto B
        if (PORTBbits.RB0 == 0)     //Si se presiona el primer boton que incre.
            PORTD++;
        if (PORTBbits.RB1 == 0)     //Si se presiona el 2do boton que decr.
            PORTD--;
        INTCONbits.RBIF = 0;        //Se limpia la bandera del puerto B
    }
    if (INTCONbits.T0IF == 1){      //Si se enciende la bandera del tmr0
        INTCONbits.T0IF = 0;        //Se limpia la bandera
        TMR0 = 178;                 //Se resetea el tmr
        PORTEbits.RE0 = 0;          //Se limpian los bits del multiplexado
        PORTEbits.RE1 = 0;              
        
        if(d == 0){                 //Si el valor de la variable es 0
            segm(bits_sign);       //El puerto C tiene el valor del disp 1
            PORTEbits.RE0 = 1;      //Se enciende el display correspondiente
            PORTEbits.RE1 = 0;
            d = 1;                  //Se intercambia el valor de la variable
        }
        else if (d == 1){           //Si el valor de la variable es 1
            segm(resto_bits);         //El puerto C tiene el valor del disp 2
            PORTEbits.RE0 = 0;      //Se enciende el display correspondiente
            PORTEbits.RE1 = 1;      
            d = 0;                  //Se intercambia el valor de la variable
        }
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
        if (ADCON0bits.GO == 0){
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
       
        res = PORTD - var;      //Se realiza una resta para comparar el valor
        if(res < 0){            //En caso que var sea mayor que PORTD
            PORTEbits.RE2 = 1;      //Se enciende la alarma
        }
        else{
            PORTEbits.RE2 = 0;      //De lo contrario se apaga
        }
    }
    
}

//------------------------------------------------------------------------------
//                      Configuración                            
//------------------------------------------------------------------------------

void setup(void){
    //Configuración de puertos
    ANSEL = 0x01;           //Se configura las entradas analogicas
    ANSELH = 0x00;
    
    TRISA = 0x01;           //El puerto A, C, D y E son salida, mientras que en
    TRISB = 0x03;           //B son entradas
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;

    PORTB = 0x00;
    PORTC = 0x00;           //Se limpian todos los puertos que son salida
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configuración del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
    
    //Configuración del timer0
    OPTION_REGbits.T0CS = 0;   //Se utiliza el oscilador int. para el tmr0
    OPTION_REGbits.PSA = 0;    
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;    //Prescaler de 64
    TMR0 =  178;               //El valor del TMR0 es igual a 178
    
    //Configuración de interrupciones
    INTCONbits.T0IF = 0;       //Se habilita las interrupciones del tmr0
    INTCONbits.T0IE = 1;       //y las del ADC
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.RBIE = 1;        //Se habilita las interrupciones y del puerto B
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1;
    
    //Coniguración del puerto B para pull-up
    OPTION_REGbits.nRBPU = 0;   //Se habilitan los pull up de los puertos B
    WPUBbits.WPUB0 = 1;         //Se habilita el pull up de los dos botones
    WPUBbits.WPUB1 = 1;
    
    //Configuración interrupt on change
    IOCBbits.IOCB0 = 1;         //Se habilita la interrupcion de los botones
    IOCBbits.IOCB1 = 1;
    
    //Configuración del ADC
    ADCON0bits.ADON = 1;        //Funcione el ADC
    ADCON1bits.ADFM = 0;        //Justificado a la izquierda
    ADCON0bits.GO   = 1;        // Lectura del potenciometro
    ADCON1bits.VCFG0 = 0;       //Voltajes de referencia 
    ADCON1bits.VCFG1 = 0;       //
    ADCON0bits.CHS = 0;         // Pin AN0 para leer
    ADCON0bits.ADCS = 1;        // Clock selection
    __delay_us(50);
}

//------------------------------------------------------------------------------
//                      Funciones                           
//------------------------------------------------------------------------------

