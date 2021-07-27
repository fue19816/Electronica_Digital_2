/* 
 * File:   ADC.c
 * Author: Usuario
 *
 * Created on 23 de julio de 2021, 12:14 PM
 */
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "ADC.h"
/*
 * 
 */
uint8_t ADC(uint8_t a){
    a = ADRESH;         //Varibale igual a valor del pot
    return a;
}
