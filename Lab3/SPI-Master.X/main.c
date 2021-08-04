//*****************************************************************************
/* 5:20 es bueno
 * File:   main.c
 * Author: Oscar Fuentes
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 30 de julio de 2021, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPI.h"
#include "USART.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
uint8_t bandera = 0;
double val1, val2, cont;
char dato;
char c[30];
char d[20];
char e[20];

void setup(void);

void __interrupt() Interrupciones(void){
    if(PIR1bits.RCIF){
        dato = RCREG;   //Valor del teclado igual a variable
    }
}

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       if(dato == '+'){
           cont++;
           dato = 0;
       }
       else if(dato == '-'){
           cont--;
           dato = 0;
       }
       
       spiWrite(bandera);
       
       if (bandera == 0){
           bandera = 1;
           val1 = spiRead();
       }
       else if(bandera == 1){
           bandera  = 0;
           val2 = spiRead();
       }
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(1000);
       
       sprintf(c,"Potenciometro 1: %3.0fbits",val1);
       sprintf(d,"Potenciómetro 2: %3.0fbits",val2);
       sprintf(e,"Contador: %3.0f",cont);
       
       TXREG = '\f';
       envio_caract(c);
       TXREG = '\r';
       envio_caract(d);
       TXREG = '\r';
       envio_caract(e);
       
       PORTD = cont;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC7 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //Configuración de interrupciones
    INTCONbits.PEIE = 1;        
    PIE1bits.RCIE = 1;          //Interrupción al recibir datos
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;         //La configuración es asíncrona
    TXSTAbits.BRGH = 0;         //Según configuración a 9600 del baud rate
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 51;
    SPBRGH = 0;
    
    RCSTAbits.SPEN =1;          //Habilitación del puerto serial
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitación de recibimiento de datos
    
    TXSTAbits.TXEN = 1;         //Habilitación de transmisión de datos

}