
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

/* WDT operating mode ? WDT Disabled */
#pragma config WDTE = OFF
/* Low-voltage programming enabled, RE3 pin is MCLR */
#pragma config LVP = ON


#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 1000000UL


void initSysClk(void);
void initUsart(void);
void initPort(void);
void writeUsart(uint8_t txData);
void initAdc(void);
void startAdccConversion(uint8_t channel);
void initTimer2(void);
uint16_t getFilterValue(void);
void initInterrupt(void);
void __interrupt() INTERRUPT_InterruptMAnger(void);

void initSysClk(void) {
    OSCCON1 = 0x60; /* set HFINTOSC as new oscillator source */
    OSCFRQ = 0x00; /* set HFFRQ to 1 MHz */
}

void initTimer2(void) {
    // T2CS FOSC/4; 
    T2CLKCON = 0x01;
    // T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; 
    T2HLT = 0x00;
    // T2RSEL T2CKIPPS pin; 
    T2RST = 0x00;
    // PR2 249; 
    T2PR = 0xF9;
    // TMR2 0; 
    T2TMR = 0x00;
    // Clearing IF flag.
    PIR3bits.TMR2IF = 0;
    // T2CKPS 1:1; T2OUTPS 1:1; TMR2ON on; 
    T2CON = 0x80;
}

void initUsart(void) {
    /* Transmit Enable */
    U1CON0bits.TXEN = 1;
    /*Baud Rate Generator is high speed with 4 baud clocks per bit */
    U1CON0bits.BRGS = 1;
    /*Asynchronous 8-bit UART mode*/
    U1CON0bits.MODE = 0;
    /* Baud rate 9600 */
    U1BRG = 25; //see Page 559 for more details
    /*Serial port enabled*/
    U1CON1bits.ON = 1;
}

void initPort(void) {
    /* Configure RF0 as output. */
    TRISFbits.TRISF0 = 0;
    /* RF0 is TX1 */
    RF0PPS = 0x20;
    /*set pin RA0 as InPut*/
    TRISAbits.TRISA0 = 1;
    /*set pin RA0 as analog*/
    ANSELAbits.ANSELA0 = 1;
}

void writeUsart(uint8_t txData) {
    while (0 == PIR4bits.U1TXIF) {
        ;
    }
    U1TXB = txData;
}

void initInterrupt(void) {
    /*enable global interrupt*/
    INTCON0bits.GIE = 1;
}

void initAdc(void) {
    /*ADCC Module Enable*/
    ADCON0bits.ADON = 1;

    /* select FRC clock*/
    ADCON0bits.ADCS = 1;

    /*result right justified*/
    ADCON0bits.ADFM = 1;

    /*channel number to connect to VSS*/
    ADPCH = 0b111100;

    /*Low Pass Filter Mode*/
    ADCON2bits.MD = 4;

    /*repeat Value */
    ADRPT = 16;

    /*result is rightshifted by 4*/
    ADCON2bits.CRS = 4; // cutoff Freq 10hz

    /* error calculation Method : First derivative of single measurement*/
    ADCON3bits.CALC = 0;

    /*Threshold Interrupt Mode Select:Interrupt regardless of threshold test results */
    ADCON3bits.TMD = 8;

    /*Auto Trigger with Timer2 at 1ms*/

    ADACTbits.ADACT = 4;

    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;

    /*clear ADCC Threshold Interrupt Flag*/
    PIR2bits.ADTIF = 0;

    /*Enable ADCC threshold interrupt*/
    PIE2bits.ADTIE = 1;
}

void startAdccConversion(uint8_t channel) {
    // select the A/D channel
    ADPCH = channel;
    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Start the conversion
    ADCON0bits.ADGO = 1;
}

uint16_t getFilterValue(void) {
    return ((uint16_t) ((ADFLTRH << 8) + ADFLTRL));
}

void __interrupt(irq(IRQ_ADT)) INTERRUPT_InterruptMAnger(void) {

    if (PIE2bits.ADTIE == 1 && PIR2bits.ADTIF == 1) {
        PIR2bits.ADTIF = 0;
    }

}

void main(void) {

    uint16_t adcRd = 0;
    initSysClk();
    initPort();
    initUsart();
    initAdc();
    initInterrupt();
    initTimer2();
    startAdccConversion(0);

    while (1) {

        adcRd = getFilterValue();
        writeUsart(0x03);
        writeUsart(adcRd & 0xFF);
        writeUsart((adcRd >> 8));
        writeUsart(0xFC);
        __delay_ms(500);

    }
}