///*
// * main.c
// *
// * Danica Fujiwara & Spencer Shaw
// *
// * CPE 329-17/18 Spring 2019
// *
// */

//******************************************************************************
//   MSP432P401 Demo - eUSCI_A0 UART echo at 115.2 kHz baud using BRCLK = 3MHz
//
//  Echoes back characters received via a PC serial port. SMCLK/ DCO is used as
//  a clock source
//
//                MSP432P401
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |                 |
//            |     P1.3/UCA0TXD|----> PC (echo)
//            |     P1.2/UCA0RXD|<---- PC
//            |                 |
//
//******************************************************************************

#include <stdint.h>
#include "msp.h"

#include "my_msp.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "spi.h"

#define FREQ FREQ_3_MHZ

volatile unsigned char char_data;
volatile uint8_t got_fresh_char;

int main(void)
{
    char_data = 0;
    got_fresh_char = FALSE;
    init(FREQ);
    led_on();
    delay_ms(500, FREQ);
    // Enable global interrupt
    __enable_irq();
    led_off();
    while(1){
        if (got_fresh_char){
            rgb_set(char_data);
            // dac_set(translate_char_to_dac(char_data));
        }
    }
}

// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        led_on();
        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        char_data = EUSCI_A0->RXBUF;
        got_fresh_char = TRUE;
        // Echo the received character back
        EUSCI_A0->TXBUF = char_data;
        led_off();
    }
}


