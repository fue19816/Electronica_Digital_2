/* 
 * File:   USART.c
 * Author: Usuario
 *
 * Created on 30 de julio de 2021, 05:18 PM
 */

#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "USART.h"

/*
 * 
 */

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