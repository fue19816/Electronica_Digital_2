/*
 * Archivo:     main
 * Autor:       Oscar
 *
 * Created on 6 de agosto de 2021, 12:45 AM
 */

//------------------------------------------------------------------------------
//                      Implementación de librerias                             
//------------------------------------------------------------------------------
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"

//------------------------------------------------------------------------------
//   Configuración del PIC
//------------------------------------------------------------------------------
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
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)



//------------------------------------------------------------------------------
//                      Variables                             
//------------------------------------------------------------------------------
uint8_t tem, dia;
int i, q;

char cad[] = "Noc";
char cad1[] = "Dia";
char a[5];
char b[5];
char c[5];
char d[5];
char e[5];

char   u_dist, d_dist, c_dist;  //Desglose de valor
char   u_temp, d_temp;
char   u_luz;
//------------------------------------------------------------------------------
//                      Prototipo de funciones                             
//------------------------------------------------------------------------------
void setup(void);
char centenas (int dato);
void envio_caract(char st[]);
void envio_char(char dato);
char decenas (int dato);
char unidades (int dato);

void __interrupt() isr(void){    
	if(RBIF == 1){
    INTCONbits.RBIE = 0;
    if(RB4 == 1)                  //If ECHO is HIGH
      T1CONbits.TMR1ON = 1;                    //Start Timer
    if(RB4 == 0){
      T1CONbits.TMR1ON = 0;                    //Stop Timer
      q = (TMR1L | (TMR1H<<8))/58.82;  //Calculate Distance
    }
    
  }
    
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    return;
}
//------------------------------------------------------------------------------
//                      Ciclo principal                            
//------------------------------------------------------------------------------
void main(void) {
    setup();                            //Llamo las configuraciones
    LCD_Init(0x4E); // Initialize LCD module with I2C address = 0x4E
    LCD_Clear();
    //--------------------------------------------------------------------------
    //                      Loop principal                                      
    //--------------------------------------------------------------------------
    while(1){    
        TMR1H = 0;                  //Sets the Initial Value of Timer
        TMR1L = 0;                  //Sets the Initial Value of Timer

        PORTBbits.RB0 = 1;               //TRIGGER HIGH
        __delay_us(10);               //10uS Delay
        PORTBbits.RB0 = 0;               //TRIGGER LOW

        __delay_ms(100); //Waiting for ECHO
        q = q + 1; //Error Correction Constant
        if(q>=2 && q<=400){
            PORTDbits.RD1 = 0;
            if(q <= 100){
             PORTDbits.RD0 = 1;
            }
            else{
             PORTDbits.RD0 = 0;
             }
        }
        else{
            PORTDbits.RD1 = 1;
        }
        
        //Se realiza la comunicación con el PIC con el potenciometro
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        dia = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //Se realiza la comunicación con el PIC con el contador
        I2C_Master_Start();
        I2C_Master_Write(0x71);
        tem = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        d_temp = decenas(tem);       
        u_temp = unidades(tem);
        c_dist = centenas(q);       
        d_dist = decenas(q);       
        u_dist = unidades(q);
        u_luz =  unidades(dia);
        
        LCD_Set_Cursor(1, 1);
        LCD_Write_String("S1:   S2:   S3:");
        
        LCD_Set_Cursor(2, 1);
        itoa(a,c_dist,10);
        itoa(b,d_dist,10);
        itoa(c,u_dist,10);
        strcat(a,b);
        strcat(a,c);
        LCD_Write_String(a);
                
        LCD_Set_Cursor(2, 7);
        if(dia == 0){
           LCD_Write_String(cad); 
        }else{
           LCD_Write_String(cad1);
        }
        
        LCD_Set_Cursor(2, 14);
        itoa(d,d_temp,10);
        itoa(e,u_temp,10);
        strcat(d,e);
        LCD_Write_String(d);
        
       envio_char(44);           //Coma
       envio_char(d_temp+ 48);
       envio_char(u_temp+ 48);
       //-------Sensor ultrasonico---------------
       envio_char(44);           //Coma
       envio_char(c_dist+ 48);
       envio_char(d_dist+ 48);             
       envio_char(u_dist+ 48);
       //---------Sensor de luz------------------
       envio_char(44);           //Coma
       envio_char(u_luz+ 48);
       envio_char(44);           //Coma
       __delay_ms(500);
    }
}

//------------------------------------------------------------------------------
//                      Configuración                            
//------------------------------------------------------------------------------
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0x08;
    TRISC = 0x80;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    
    //Configuración del oscilador 
    OSCCONbits.IRCF2 = 1;       //Se configura el oscilador a 8Mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //Se utiliza el oscilador interno
    
    //  Declaramos puertos como entradas o salidas
    TRISB = 0b00010000;           //RB4 as Input PIN (ECHO)
    TRISD = 0x00; // LCD Pins as Output
    TRISA = 0x00; // LCD Pins as Output
    
    //Habilitación de pull up
    OPTION_REGbits.nRBPU = 0;
    
    //Pull Up para puerto B
    WPUBbits.WPUB4 = 1;
    
    //Interrupt on change
    IOCBbits.IOCB4 = 1;
    
    //Habilitación de interrupciones
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;         //La configuración es asíncrona
    TXSTAbits.BRGH = 1;         //Según configuración a 9600 del baud rate
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
    SPBRGH = 0;
    
    RCSTAbits.SPEN =1;          //Habilitación del puerto serial
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitación de recibimiento de datos
    
    TXSTAbits.TXEN = 1;         //Habilitación de transmisión de datos
}

//------------------------------------------------------------------------------
// Funciones
//------------------------------------------------------------------------------
void envio_caract(char st[]){
    int i = 0;              //Se declara una variable que recorrera la cadena
    while (st[i] != 0){     //Mientras st no sea igual a 0
        envio_char(st[i]);  //Se enviara el caracter por caracter
        i++;                //Se aumenta en 1 el caracter a mostrar en la cadena
    }
}

void envio_char(char dato){
    while(!TXIF);           //Mientras la bandera de transmisión sea 0
    TXREG = dato;           //Se envía el caracter
}

char centenas (int dato){       //Saco el valor de centena
    char out = dato / 100 ;
    return out;
}

char decenas (int dato){        //Saco el valor de decena
    char out;
    out = (dato % 100)/ 10;
    return out;
}

char unidades (int dato){       //Saco el valor de unidad
    char out;
    out = (dato % 100) % 10;
    return out;
}
