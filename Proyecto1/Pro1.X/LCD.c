/*
 * File:   LCD.c
 * Author: 
 * Created on 25 de agosto de 2021, 07:07 PM
 */
//-------------------------------------------------------
//  Definiciones
//-------------------------------------------------------
#include <xc.h>
#include "LCD.h"

unsigned char RS, i2c_add = 0x08;

unsigned char RS, i2c_add, BackLight_State = LCD_BACKLIGHT;
//-------------------------------------------------------
//   Funciones
//-------------------------------------------------------
//--------------------------I2C--------------------------
void I2C_Master_Init(const unsigned long c)
{
   SSPCON = 0b00101000;
   SSPCON2 = 0;
   SSPADD = (_XTAL_FREQ/(4*c))-1;
   SSPSTAT = 0;
   TRISCbits.TRISC3 = 1;
   TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait(){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(){
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart(){
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop(){
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_Master_Write(unsigned d)
{
   I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
   SSPBUF = d;             
}

unsigned short I2C_Master_Read(unsigned short a){
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}    
//---------------- LCD ---------------------------------

void LCD_Init(unsigned char I2C_Add) {
  i2c_add = I2C_Add;
  IO_Expander_Write(0x00);
  __delay_ms(30);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(LCD_RETURN_HOME);
  __delay_ms(5);
  LCD_CMD(0x20 | (LCD_TYPE << 2));
  __delay_ms(50);
  LCD_CMD(LCD_TURN_ON);
  __delay_ms(50);
  LCD_CMD(LCD_CLEAR);
  __delay_ms(50);
  LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
  __delay_ms(50);
}

void IO_Expander_Write(unsigned char Data){
  I2C_Master_Start();
  I2C_Master_Write(i2c_add);
  I2C_Master_Write(Data | BackLight_State);
  I2C_Master_Stop();
}

void LCD_Write_4Bit(unsigned char Nibble) {
  // Get The RS Value To LSB OF Data  
  Nibble |= RS;
  IO_Expander_Write(Nibble | 0x04);
  IO_Expander_Write(Nibble & 0xFB);
  __delay_us(50);
}

void LCD_CMD(unsigned char CMD) {
  RS = 0; // Command Register Select
  LCD_Write_4Bit(CMD & 0xF0);
  LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(char Data){
  RS = 1;  // Data Register Select
  LCD_Write_4Bit(Data & 0xF0);
  LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(char* Str){
    for(int i=0; Str[i]!='\0'; i++)
       LCD_Write_Char(Str[i]); 
}

void LCD_Set_Cursor(unsigned char ROW, unsigned char COL) {    
  switch(ROW) 
  {
    case 2:
      LCD_CMD(0xC0 + COL-1);
      break;
    case 3:
      LCD_CMD(0x94 + COL-1);
      break;
    case 4:
      LCD_CMD(0xD4 + COL-1);
      break;
    // Case 1  
    default:
      LCD_CMD(0x80 + COL-1);
  }
}

void Backlight() {
  BackLight_State = LCD_BACKLIGHT;
  IO_Expander_Write(0);
}

void noBacklight() {
  BackLight_State = LCD_NOBACKLIGHT;
  IO_Expander_Write(0);
}

void LCD_SL(){
  LCD_CMD(0x18);
  __delay_us(40);
}

void LCD_SR(){
  LCD_CMD(0x1C);
  __delay_us(40);
}

void LCD_Clear(){
  LCD_CMD(0x01); 
  __delay_us(40);
}