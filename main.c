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
#include <xc.h>
#include "mcc_generated_files/mcc.h"
//#define FCY 16000000UL    //16MHz
#define _XTAL_FREQ 16000000
//#define FCY 31000UL     //31KHz
#include <stdio.h>
#include <string.h>
char ReadData[8];
char DisplayData[8]="12345678";
int i,j,k;
int Laser;

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

void LCD_str2(const char *c) {
    unsigned char wk;
    i2c_START();
    i2c_TXDAT(LCD_ADD<<1);
    i2c_TXDAT(0x40);
    for (int i=0;i<8;i++){
        wk = c[i];
        if  (wk == 0x00) break;
        i2c_TXDAT(wk);
    }
    i2c_STOP();
    __delay_us(30);
}

void LCD_clear(){
    writeLCDCommand(0x01);
    __delay_us(1100);        //Instruction Execution Time 0.59-1.08ms (550:NG, 600:GOOD)
}
void writeLCDData(char t_data){
    i2c_START();
    i2c_TXDAT(LCD_ADD<<1);
    i2c_TXDAT(0x40);
    i2c_TXDAT(t_data);
    i2c_STOP();
    __delay_us(30);     //Instruction Execution Time 14.3-26.3us
}
void LCD_SetCG(const char *c){
    for (int i=0;i<40;i++){
        writeLCDCommand(0x48+i);
        writeLCDData(c[i]);
    }
}
void LCD_Level15R(int a){
    if (a<0) a=0;
    if (a>0x000f) a=0x000f;
    for(int i=0;i<8;i++){
        if(( a >> 1 ) >= i){
            DisplayData[i] = 2;
            if( (( a >> 1 ) == i) && (( a & 1 ) == 0)){
                DisplayData[i] = DisplayData[i]-1;
            }
        } else {
            DisplayData[i]=0x20;
        }
    }
    if (LCD) { LCD_xy(0,0); LCD_str2( DisplayData ); }
}

void LCD_Meter15R(int a){
    if (a<0) a=0;
    if (a>0x000f) a=0x000f;
    for(int i=0;i<8;i++){
        if( ( a >> 1 ) == i){
            DisplayData[i] = 3;
            if( ( a & 1 ) == 0 ){
                DisplayData[i] = DisplayData[i]+1;
            }
        } else {
            DisplayData[i]=5;
        }
    }
    if (LCD) { LCD_xy(0,0); LCD_str2( DisplayData ); }
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
    const char CG[]={
        0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
        0x1b,0x1b,0x1b,0x1b,0x1b,0x1b,0x1b,0x1b,
        0x1b,0x03,0x03,0x03,0x03,0x03,0x03,0x1b,
        0x1b,0x18,0x18,0x18,0x18,0x18,0x18,0x1b,
        0x1b,0x00,0x00,0x00,0x00,0x00,0x00,0x1b};
    LCD_SetCG(CG);
    
//    if (LCD) { LCD_xy(0,0); LCD_str2("Compact "); }
//    if (LCD) { LCD_xy(0,1); LCD_str2(" PnP n.k"); }
//    __delay_ms(1000);

    uint16_t convertedValue;
    uint32_t VDDValue100;
    int CalculatedResult;

    ADC_Initialize();
    
    while (1)
    {
        // Add your application code
        /* Measure own power supply voltage */
        ADC_Initialize();
        ADC_SelectChannel(channel_FVR); //2.048V
        ADC_TemperatureAcquisitionDelay();
        ADC_StartConversion();
        while(!ADC_IsConversionDone());
        convertedValue = ADC_GetConversionResult();
        VDDValue100 = 419430 / convertedValue;    //Not enough memory to do float operations. So, x100

        /* Measure LED Current value */
        ADC_Initialize();   //required this
        ADC_SelectChannel(channel_AN3); //channel_AN3=current sensor
        ADC_StartConversion();
        while(!ADC_IsConversionDone());
        convertedValue = ADC_GetConversionResult();

        convertedValue = (VDDValue100 * (uint32_t)convertedValue ) >> 10;
        /* 
         * Rev.05 board may remove the display
         *  as it does not detect well a large voltage drop in the LEDs
         */
        sprintf(DisplayData, "%dmA ", convertedValue ); 
        if (LCD) { LCD_xy(4,1); LCD_str2( DisplayData ); }


        /* schematics ZX board 
         * Rev.05
         *  RA2: Laser OUT
         *  (CLC1 Out:
         *     Normally, RA5(CLC1IN1) is output directly.
         *     When RC4(CLC2IN1) is Low, output fixed duty cycle.)
         *  RC2: LED blue
         *  RC3: Vacuum ADC =input Analog7
         *  RC4: Push-SW for Laser Marker
         *  RC5: T input    =GPIO input
         */
        /*
         * If continuous low-level for 500ms, judge no laser.
         * Wait for a continuous high-level for 80mS
         *  = Detects breaks in the data sequence
         */
        i = 0;
        k = 0;
        Laser = 1;  // Laser-Connected
        while(i < 80){    //80ms
            __delay_us(1000);
            i++;
            if(RC5 ==0){    // 0 = No laser
                i=0;
                k++;
                if(k>500){
                    Laser = 0;  // Determine No Laser
                    break;
                }
            }
        }
        if(Laser == 1){
            for(j=0;j<7;j++){
                ReadData[j]=0;
                while(RC5==1){} //The edge detect of start-bit
                __delay_us(7500);
                for(i=0;i<8;i++){
                    ReadData[j] = ReadData[j] >> 1 ;
                    if(RC5==1){
                        ReadData[j] =  ReadData[j] | 0b10000000 ;
                    }
                    __delay_us(5000);
                }
            }
            for(j=0;j<8;j++){
                DisplayData[j]=0;//NULL;
            }
            if ( ReadData[5] > 4) {
                ReadData[5] = ReadData[5] -5;
            } else {
                ReadData[5] = ReadData[5] +5;
            }
            if(ReadData[4]==0) ReadData[5] =0;
            if (LCD) { LCD_clear(); }
            sprintf(DisplayData, "T=%2d.%1d%cC", ReadData[4], ReadData[5], 0xf2);
            if (LCD) { LCD_xy(0,0); LCD_str2( DisplayData ); }
            sprintf(DisplayData, "%2x", ReadData[3]);
            if (LCD) { LCD_xy(0,1); LCD_str2( DisplayData ); }
            if(ReadData[3]==0x24){  //0x24=500mW Laser, 0x25=2.5W Laser
                PWM3DCH=0x2;PWM3DCL=0x40;   //Duty Cycle: 0.89%
            } else {
                PWM3DCH=0x1;PWM3DCL=0x0;    //Duty Cycle: 0.39%
            }
        }
        else
        {
            /* read the air pressure in pipe */
            ADC_Initialize();   //required this
            ADC_SelectChannel(channel_AN7); //channel_AN7=pressure sensor
            ADC_StartConversion();
            while(!ADC_IsConversionDone());
            convertedValue = ADC_GetConversionResult();

            convertedValue = ((VDDValue100 * (uint32_t)convertedValue ) >> 10);
            LCD_Meter15R((convertedValue-50) >>4);
            sprintf(DisplayData, "%d", convertedValue ); 
            if (LCD) { LCD_xy(0,1); LCD_str2( DisplayData ); }

        }
        if (RC4 == 0) { //RC4 is Push-SW for Laser Marker
            RC2=1;
        } else {
            RC2=0;
        }
        /* for test*/
        //RC2=~RC2;
    }
}
/**
 End of File
*/