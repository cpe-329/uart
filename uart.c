/*
 * uart.c
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 */
#include <stdint.h>

#include "msp.h"
#include "led.h"
#include "delay.h"
#include "my_msp.h"
#include "uart.h"


inline void uart_init(){
    // Configure UART pins
    //P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function
    has_new = FALSE;
    new_char = 0;

    // // Configure UART
    // EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    // EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
    //         EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // // Baud Rate calculation
    // // 3000000/(115200) = 26.041667
    // // Fractional portion = 0.041667
    // // User's Guide Table 21-4: UCBRSx = 0x00
    // // UCBRx = int (26.041667 / 16) = 1
    // // UCBRFx = int (((26.041667/16)-1)*16) = 10

    // EUSCI_A0->BRW = 1;                      // Using baud rate calculator
    // EUSCI_A0->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS) |
    //                   EUSCI_A_MCTLW_OS16;
    // EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    // EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    // EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
    // // Enable global interrupt
    // __enable_irq();

    // // Enable eUSCIA0 interrupt in NVIC module
    // NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

}

void uart_write(unsigned char c){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)){}
    EUSCI_A0->TXBUF = c;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)){}
}

void uart_write_int(unsigned int acc){
    rgb_set(RGB_RED);
    if(acc >= 10){
        uart_write_int(acc/10);
    }
    uart_write((uint8_t)((acc%10) + '0'));
}

unsigned int uart_get_int(){
    int i = -1;
    int accumulator = 0;
    rgb_set(RGB_BLUE);
    while(new_char != ESCAPE_VAL){
        if (has_new){
            i = ascii_to_int(new_char);
            if (i != 255){
                accumulator = (10 * accumulator) + i;
                has_new = FALSE;
            }
        }
    }
    new_char = 255;
    return accumulator;
}

static inline uint8_t ascii_to_int(uint8_t val){
    if (val == ESCAPE_VAL){
        rgb_set(RGB_WHITE);
        return val;
    }
    val  = val - '0';
    if (val <= 9){
        rgb_set(val);
        return val;
    }
    return 255;
}
