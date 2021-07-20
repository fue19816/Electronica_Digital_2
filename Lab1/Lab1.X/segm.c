/* 
 * File:   segm.c
 * Author: Usuario
 *
 * Created on 19 de julio de 2021, 01:04 PM
 */

#include <xc.h>
#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segm.h"
/*
 * 
 */
void segm(uint8_t a){
    //Función para obtener el valor para el display de 7 segmentos
    switch(a){
        case 0:
            PORTC = 0x3F;  //Se regresa el valor de 0 para mostrar en el disp.
            break;
        case 1:
            PORTC = 0x06;  //Se regresa el valor de 1 para mostrar en el disp.
            break;
        case 2:
            PORTC = 0x5B;  //Se regresa el valor de 2 para mostrar en el disp.
            break;
        case 3:
            PORTC = 0x4F;  //Se regresa el valor de 3 para mostrar en el disp.
            break;
        case 4:
            PORTC = 0x66;  //Se regresa el valor de 4 para mostrar en el disp.
            break;
        case 5:
            PORTC = 0x6D;  //Se regresa el valor de 5 para mostrar en el disp.
            break;
        case 6:
            PORTC = 0x7D;  //Se regresa el valor de 6 para mostrar en el disp.
            break;
        case 7:
            PORTC = 0x07;  //Se regresa el valor de 7 para mostrar en el disp.
            break;
        case 8:
            PORTC = 0x7F;  //Se regresa el valor de 8 para mostrar en el disp.
            break;
        case 9:
            PORTC = 0x67;  //Se regresa el valor de 9 para mostrar en el disp.
            break;
        case 10:
            PORTC = 0x77; //Se regresa el valor de 10 para mostrar en el disp.
            break;
        case 11:
            PORTC = 0x7C; //Se regresa el valor de 11 para mostrar en el disp.
            break;
        case 12:
            PORTC = 0x39; //Se regresa el valor de 12 para mostrar en el disp.
            break;
        case 13:
            PORTC = 0x5E; //Se regresa el valor de 13 para mostrar en el disp.
            break;
        case 14:
            PORTC = 0x79; //Se regresa el valor de 14 para mostrar en el disp.
            break;
        case 15:
            PORTC = 0x71; //Se regresa el valor de 15 para mostrar en el disp.
            break;
    }
}

