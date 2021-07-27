/* 
 * File:   main.c
 * Author: Oscar Fuentes
 *
 * Created on 22 de julio de 2021, 09:14 PM
 */

//------------------------------------------------------------------------------
//                      Implementación de librerias                             
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include <string.h>
#include "LCD.h"
#include "ADC.h"
#include "USART.h"

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

double c;                       
uint8_t valadc, valor1, valor2;
char dato;
double  vol1, vol2;
char v1[10];
char cadena1[30];
char cadena2[30];
char cadena3[30];

//------------------------------------------------------------------------------
//                      Prototipo de funciones                             
//------------------------------------------------------------------------------
void setup(void);               //Se declaran todas las funciones a usar
double conversion(uint8_t var);


void __interrupt() Interrupciones(void){
    if(PIR1bits.RCIF){
        dato = RCREG;   //Valor del teclado igual a variable
    }
}

//------------------------------------------------------------------------------
//                      Ciclo principal                            
//------------------------------------------------------------------------------
void main(void) {
    setup();    //Se llama a las configuraciones del pic
    Lcd_Init();
    Lcd_Clear();
    //--------------------------------------------------------------------------
    //                      Loop principal                                      
    //--------------------------------------------------------------------------
    while(1){
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:   S2:   S3:");
        vol1 = conversion(valor1);
        vol2 = conversion(valor2);
        
        //Mostrar el valor del pot 1
        Lcd_Set_Cursor(2,1);
        sprintf(v1, "%3.2fV",vol1);
        Lcd_Write_String(v1);
        
        //Mostrar el valor del pot 2
        Lcd_Set_Cursor(2,7);
        sprintf(v1, "%3.2fV",vol2);
        Lcd_Write_String(v1);
        
        //Mostrar el valor del contador
        Lcd_Set_Cursor(2,14);
        sprintf(v1, "%3.0f",c);
        Lcd_Write_String(v1);
        
        //Cadena para mostrar el valor en la terminal
        sprintf(cadena1,"Voltaje 1: %3.2fV",vol1);
        sprintf(cadena2,"Voltaje 2: %3.2fV",vol2);
        
        //Condición para saber el valor ingresado por el teclado
        if(dato == '+'){
            c++;
            dato = 0;
        }
        else if(dato == '-'){
            c--;
            dato = 0;
        }
        
        //Impresión en la consola
        TXREG = '\f';
        envio_caract(cadena1);
        TXREG = '\r';
        envio_caract(cadena2);
        TXREG = '\r';
        
         __delay_ms(100);
         
        if (ADCON0bits.GO == 0){
            //Cambio de canal y igualación al valor del pot
            if (ADCON0bits.CHS == 0){
                valor1 = ADC(valadc);
                ADCON0bits.CHS = 1;
            }
            else if(ADCON0bits.CHS == 1){
                valor2 = ADC(valadc);
                ADCON0bits.CHS = 0;
            }
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
    }
    
}

//------------------------------------------------------------------------------
//                      Configuración                            
//------------------------------------------------------------------------------

void setup(void){
    //Configuración de puertos
    ANSEL = 0x03;           //Se configura las entradas analogicas
    ANSELH = 0x00;
    
    TRISA = 0x03;           //El puerto A, C, D y E son salida, mientras que en
    TRISB = 0x00;           //B son entradas
    TRISC = 0x80;
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
    
    //Configuración de interrupciones
    INTCONbits.PEIE = 1;        
    PIE1bits.RCIE = 1;          //Interrupción al recibir datos
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    
    //Configuración del ADC
    ADCON0bits.ADON = 1;        //Funcione el ADC
    ADCON1bits.ADFM = 0;        //Justificado a la izquierda
    ADCON0bits.GO   = 1;        // Lectura del potenciometro
    ADCON1bits.VCFG0 = 0;       //Voltajes de referencia 
    ADCON1bits.VCFG1 = 0;       //
    ADCON0bits.CHS = 0;         // Pin AN0 para leer
    ADCON0bits.ADCS = 1;        // Clock selection
    __delay_us(50);
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;         //La configuración es asíncrona
    TXSTAbits.BRGH = 0;         //Según configuración a 9600 del baud rate
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 51;
    SPBRGH = 0;
    
    RCSTAbits.SPEN =1;          
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitación de recibimiento de datos
    
    TXSTAbits.TXEN = 1;         //Habilitación de transmisión de datos
}

//------------------------------------------------------------------------------
//                      Funciones                           
//------------------------------------------------------------------------------
double conversion(uint8_t var){
    double valor;
    valor = var * 0.0196 ; //Pasar el valor de bits a valor de Voltaje
    return valor;
}