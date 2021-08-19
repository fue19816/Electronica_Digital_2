/*
 * File:   main.c
 * Author: Oscar
 * Ejemplo de uso de I2C Master
 * Created on 13 de agosto de 2020,01:57 AM
 */
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t contador = 0;
float f;
char s[10];
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void envio_char(char dato);
void envio_caract(char st[]);
float conversion(uint8_t valor);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
    if(INTCONbits.RBIF){            //Si se enciende la bandera del puerto B
        if (PORTBbits.RB0 == 0)     //Si se presiona el primer boton que incre.
            contador++;
        if (PORTBbits.RB1 == 0)     //Si se presiona el 2do boton que decr.
            contador--;
        INTCONbits.RBIF = 0;        //Se limpia la bandera del puerto B
    }
    if(PIR1bits.RCIF){
       PORTD = RCREG;   //Valor del teclado igual al puerto D
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    while(1){
        f = conversion(contador);
        
        sprintf(s,"%0.0f",f);
        
        TXREG = ',';
        envio_caract(s);
        __delay_ms(5000);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0x03;
    TRISC = 0x80;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    //Configuración del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
    
    //Se activan las interrupciones del puerto B
    INTCONbits.PEIE = 1;        
    PIE1bits.RCIE = 1;          //Interrupción al recibir datos
    PIR1bits.RCIF = 0;
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
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void envio_char(char dato){
    while(!TXIF);           //Mientras la bandera de transmisión sea 0
    TXREG = dato;           //Se envía el caracter
}

void envio_caract(char st[]){
    int i = 0;              //Se declara una variable que recorrera la cadena
    while (st[i] != 0){     //Mientras st no sea igual a 0
        envio_char(st[i]);  //Se enviara el caracter por caracter
        i++;                //Se aumenta en 1 el caracter a mostrar en la cadena
    }
}
float conversion(uint8_t valor){
    float dato;
    dato = valor*1;
    return dato;
}