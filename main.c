///*
// * main.c
// *
// * Danica Fujiwara & Spencer Shaw
// *
// * CPE 329-17/18 Spring 2019
// *
// *  MCLK = = DCO ~24MHz
// *  SMCLK = MCLK / 8
// *  TIMER_A0 utilize SMCLK 3MHz
// *  TIMER_A0 CCR0 and CCR1 to create 2 timing events
// */

#include <stdint.h>
#include "msp.h"

#include "my_msp.h"
#include "delay.h"
#include "led.h"
#include "spi.h"
#include "timers.h"

#define FREQ FREQ_24_MHZ

#define MAX 1300
/*
 *  SPI Example using eUSCI_B0 for SPI
 *  P1.5  UCB0CLK   SCLK
 *  P1.6  UCB0SIMO  MOSI
 *  P1.7  UCB0SOMI  MISO
 */

volatile uint16_t data = 0;

int main(void)
{
    volatile uint16_t data;
    volatile uint32_t i;

    init(FREQ);
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
// #include "msp.h"

// int main(void)
// {
//     WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
//             WDT_A_CTL_HOLD;

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

    while(1); // do nothing
}

// UART interrupt service routine
void EUSCIA0_IRQHandler(void)
{
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        // Check if the TX buffer is empty first
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

        // Echo the received character back
        EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
    }
}


