/* 
 * File:   LCD.h
 * Author: Usuario
 *
 * Created on 23 de julio de 2021, 10:44 AM
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);

#endif	/* LCD_H */

