/*
 * File:   main.c
 * Author: Oscar
 * Ejemplo de uso de I2C Master
 * Created on 17 de febrero de 2020, 10:32 AM
 */
//*****************************************************************************
// Palabra de configuraci�n
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
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "LCD.h"
#include <xc.h>
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t sensor, pot, contador;
char s[10];
//*****************************************************************************
// Definici�n de funciones para que se puedan colocar despu�s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    while(1){
        //Se realiza la comunicaci�n con el sensor I2C
        I2C_Master_Start();
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        sensor = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
       
        //Se realiza la comunicaci�n con el PIC con el potenciometro
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        pot = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //Se realiza la comunicaci�n con el PIC con el contador
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        contador = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1:   S2:   S3:");
        
        //Mostrar el valor del pot
        Lcd_Set_Cursor(2,1);
        sprintf(s, "%u",pot);
        Lcd_Write_String(s);
        
        //Mostrar el valor del contador
        Lcd_Set_Cursor(2,8);
        sprintf(s, "%u",contador);
        Lcd_Write_String(s);
        
        //Mostrar el valor del sensor
        Lcd_Set_Cursor(2,14);
        sprintf(s, "%u",sensor);
        Lcd_Write_String(s);   
    }
    return;
}
//*****************************************************************************
// Funci�n de Inicializaci�n
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncaci�n I2C
    
    //Configuraci�n del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
}