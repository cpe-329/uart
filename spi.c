#include "spi.h"
#include "delay.h"

inline void spi_dac_init(){
    P5->SEL0 &= ~SPI_CS_PIN;
    P5->SEL1 &= ~SPI_CS_PIN;
    P5->DIR |= SPI_CS_PIN;

    P6->SEL0 &= ~DAC_LATCH_PIN;
    P6->SEL1 &= ~DAC_LATCH_PIN;
    P6->DIR |= DAC_LATCH_PIN;
    cs_high();
    latch_low();
}


inline void dac_set(const unsigned int val){
    // Activate periphrial
    cs_low();
    while(!(EUSCI_B3->IFG & EUSCI_B_IFG_TXIFG)){} // wait for tx avail
    //  Buffer Tx data
    EUSCI_B3->TXBUF = CONTORL_BITS | ((val & 0xF00) >> 8);
    while(!(EUSCI_B3->IFG & EUSCI_B_IFG_TXIFG)){} // wait for tx avail
    // Buffer Tx data
    EUSCI_B3->TXBUF = val & 0xFF;
    // wait for transmission to finish
    while(!(EUSCI_B3->IFG & EUSCI_B_IFG_TXIFG)){} 
    // Deactivate periphrial
    cs_high();
    // Latch data from DAC input reg to DAC data reg
    latch_low();
    latch_high();
}

inline void cs_low(){
    P5->OUT &= ~SPI_CS_PIN;
}
inline void cs_high(){
    P5->OUT |= SPI_CS_PIN;
}

inline void latch_low(){
    P6->OUT &= ~DAC_LATCH_PIN;
}
inline void latch_high(){
    P6->OUT |= DAC_LATCH_PIN;
}
