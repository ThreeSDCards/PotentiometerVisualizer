// CONFIG1
#pragma config FOSC = INTOSC    // INTOSC oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer PWRT disabled
#pragma config MCLRE = OFF      // CLR/VPP pin function is digital input
#pragma config CP = OFF         // Program memory code protection is disabled
#pragma config CPD = OFF        // Data Memory Code Protection disabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config CLKOUTEN = OFF   // Clock Out Enable disabled
#pragma config IESO = OFF       // Internal/External Switchover mode disabled
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config WRT = OFF        // Write protection off
#pragma config PLLEN = OFF      // 4x PLL disabled
#pragma config STVREN = ON      // Stack Over/Underflow will cause a Reset
#pragma config BORV = LO        // Brown-out Reset Voltage low trip point 
#pragma config LVP = OFF        // Low-Voltage Programming disabled

#include <xc.h>
#include <stdint.h>
#include "uartlib.h"

#define _XTAL_FREQ      1000000

void _pic_init();
const char HEX_CHAR[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'A', 'B', 'C', 'D', 'E', 'F'};

void main(void) {
    _pic_init();
    uart_init(ONENINETYTWOHUNDERED, ONEM);
    while(1){
        GO = 1;
        __delay_ms(1);
        uart_write_char(HEX_CHAR[ADRESH & 0xF]);    // MsN HB
        uart_write_char(HEX_CHAR[ADRESL >> 4]);     // MsN LB
        uart_write_char(HEX_CHAR[ADRESL & 0xF]);    // LsN LB
        uart_write_newline();                       // \r\n
    }
    return;
}

void _pic_init(){
     // ** CORE ** 
    OSCCONbits.IRCF = 0b1011;   // 1MHz clock

    // ** IO **
    TRISC = 0;                  // PORTC all output
    
    // ** ADC ** 
    ADCON0bits.ADON = 1;        //ADC is enabled
    ADCON0bits.CHS = 0b00011;   //AN3/RA4 selected
    ADCON1bits.ADFM = 1;        //RES is right-justified
    ADCON1bits.ADCS = 0;        //Clock = Fosc/2, TAD = 2us, del = 22us
    ADCON1bits.ADNREF = 0;      //Neg volt = Vss
    ADCON1bits.ADPREF = 0;      //Pos volt = VDD
}

void __interrupt() isr(){
    GIE = 0;
    if(RCIF)
        uart_handle_rcv_int(void);
    GIE = 1;
}