#include "spi.h"
#include "delay.h"

inline void dac_init(){
    P5->SEL0 &= ~(DAC_LATCH_PIN | SPI_CS_PIN);
    P5->SEL1 &= ~(DAC_LATCH_PIN | SPI_CS_PIN);
    P5->DIR |= DAC_LATCH_PIN | SPI_CS_PIN;
    cs_high();
    latch_low();
}


inline void dac_set(const unsigned int val){
    cs_low();
//    NOP
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){}
    EUSCI_B0->TXBUF = CONTORL_BITS | ((val & 0xF00) >> 8);
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){}
    EUSCI_B0->TXBUF = val & 0xFF;//data;//dac_cmd(0x800);
//    NOP
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){}
    cs_high();
//    NOP
    latch_low();
//    NOP
    latch_high();
//    NOP
//    delay_ms_auto(1);
}

inline void cs_low(){
    P5->OUT &= ~SPI_CS_PIN;
}
inline void cs_high(){
    P5->OUT |= SPI_CS_PIN;
}

inline void latch_low(){
    P5->OUT &= ~DAC_LATCH_PIN;
}
inline void latch_high(){
    P5->OUT |= DAC_LATCH_PIN;
}
