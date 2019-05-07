///*
// * main.c
// *
// * Danica Fujiwara & Spencer Shaw
// *
// * CPE 329-17/18 Spring 2019
// *
// */

#include <stdint.h>
#include "msp.h"

#include "my_msp.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "spi.h"

#define FREQ FREQ_24_MHZ

volatile unsigned char char_data;
volatile uint8_t got_fresh_char;

int main(void)
{
    unsigned int value;

    init(FREQ);

    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // Baud Rate calculation
    // 3000000/(115200) = 26.041667
    // Fractional portion = 0.041667
    // User's Guide Table 21-4: UCBRSx = 0x00
    // UCBRx = int (26.041667 / 16) = 1
    // UCBRFx = int (((26.041667/16)-1)*16) = 10

    EUSCI_A0->BRW = 1;                      // Using baud rate calculator
    EUSCI_A0->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS) |
                      EUSCI_A_MCTLW_OS16;
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
    // Enable global interrupt
    __enable_irq();

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

    led_on();
    delay_ms(500, FREQ);
    led_off();
    
    while(1){
            value = uart_get_int();
            dac_set(value);
            uart_write_nl();
            uart_write_int(value);
            uart_write_nl();
    }
}

// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        if (has_new){
          return;
        }
        led_on();
        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)){}

        new_char = EUSCI_A0->RXBUF;
        has_new = TRUE;
        // Echo the received character back
        EUSCI_A0->TXBUF = new_char;
        delay_ms(10, FREQ);
        led_off();
    }
}


