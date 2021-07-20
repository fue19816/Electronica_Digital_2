/* 
 * File:   valorADC.c
 * Author: Usuario
 *
 * Created on 16 de julio de 2021, 05:14 PM
 */
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "valorADC.h"
/*
 * 
 */
uint8_t valorADC(uint8_t var){
    var = ADRESH;               //El valor de la variable es igual al pot
    return var;
}
