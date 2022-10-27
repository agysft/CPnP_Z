/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1503
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#define FCY 16000000UL    //16MHz
//#define FCY 31000UL     //31KHz
#include <stdio.h>
#include <string.h>

/******** I2C ********/
void I2C_init(){
    SSP1CON1 = 0b00101000;  // Enables the serial port and configures the SDAx and SCLx pins as the source of the serial port pins, I2C Master mode
    ANSELC = 0;             // 0:Digital
    TRISCbits.TRISC0 = 1;   // 1:input ; When enabled, the SDAx and SCLx pins must be configured as inputs.
    TRISCbits.TRISC1 = 1;
    SSP1CON3 =  0b00000000;
    SSP1ADD =   9;          // 16MHz/(400KHz x4) -1 = 16000000/(400000 x4)-1 = 9
}

void i2c_START(){
    PIR1bits.SSP1IF = 0;
    SSP1CON2bits.SEN = 1;
    while (PIR1bits.SSP1IF == 0) {}
    PIR1bits.SSP1IF = 0;
}

void i2c_TXDAT(char data){
    PIR1bits.SSP1IF = 0;
    SSP1BUF = data;
    while (PIR1bits.SSP1IF == 0) {}
    PIR1bits.SSP1IF = 0;
}

void i2c_STOP(){
    SSP1CON2bits.PEN = 1;
    while (SSP1CON2bits.PEN == 1) {}
    PIR1bits.SSP1IF = 0;
}

/******** I2C LCD ********/
#define LCD_ADD 0x3E
bool LCD = true;//false;

int Is_LCD_Connected(){
    int exist_the_lcd = 1;  // 0: exist the LCD, 1: No LCD
    
    if ( (RC0 | RC1) != 0){ // if no pull-up
        i2c_START();
        i2c_TXDAT(LCD_ADD<<1);
        exist_the_lcd = SSP1CON2bits.ACKSTAT;   // 1: No responded (= no ACK)
        if ( exist_the_lcd == 0){
            i2c_TXDAT(0x00);
            i2c_TXDAT(0x00);    // dummy
            i2c_STOP();
        }
    }
    return exist_the_lcd;
}

void writeLCDCommand(char t_command){
    i2c_START();
    i2c_TXDAT(LCD_ADD<<1);
    i2c_TXDAT(0x00);
    i2c_TXDAT(t_command);
    i2c_STOP();
    __delay_us(30);     //Instruction Execution Time 14.3-26.3us
}

void LCD_Init(){
    __delay_ms(400);
    writeLCDCommand(0x38);
    writeLCDCommand(0x39);
    writeLCDCommand(0x14);
    writeLCDCommand(0x70);// contast LSB setting ; 0b0111 xxxx
    writeLCDCommand(0x56);// 5V=0b0101 00xx, 3V=0b0101 01xx,  xx=contrast MSB
    writeLCDCommand(0x6C);
    __delay_ms(250);
    writeLCDCommand(0x38);
    writeLCDCommand(0x0C);
    writeLCDCommand(0x01);
    __delay_us(1100);        //Instruction Execution Time 0.59-1.08ms (550:NG, 600:GOOD)
}

void LCD_xy(uint8_t x, uint8_t y){
    writeLCDCommand(0x80 + 0x40 * y + x);
}

void LCD_str2(char *c) {
    unsigned char wk;
    i2c_START();
    i2c_TXDAT(LCD_ADD<<1);
    for (int i=0;i<8;i++){
        wk = c[i];
        if  (wk == 0x00) {break;}
        i2c_TXDAT(0xc0);
        i2c_TXDAT(wk);
    }
    i2c_STOP();
    __delay_us(30);
}

void LCD_clear(){
    writeLCDCommand(0x01);
    __delay_us(1100);        //Instruction Execution Time 0.59-1.08ms (550:NG, 600:GOOD)
}

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    __delay_ms(20);
    I2C_init();
    if ( Is_LCD_Connected() == 0 ) {
        LCD = true; 
    } else {
        LCD = false;
    }

    if (LCD) LCD_Init();
    if (LCD) { LCD_xy(0,0); LCD_str2("Compact"); }
    if (LCD) { LCD_xy(0,1); LCD_str2(" PnP n.k"); }

    while (1)
    {
        // Add your application code
        if (LC2OUT == 0) {
            RC2=1;
        } else {
            RC2=0;
        }
    }
}
/**
 End of File
*/