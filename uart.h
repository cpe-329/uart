/*
 * uart.h
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 */
#ifndef __UART_H__
#define __UART_H__


#include <stdint.h>

#include "msp.h"
#include "my_msp.h"

#define UART_PINS (P1_2 | P1_3)

#define ESCAPE_VAL 'a'

volatile unsigned int has_new;
volatile char new_char;

inline void uart_init();
void uart_write(unsigned char c);
void uart_write_int(unsigned int acc);
unsigned int uart_get_int();
static inline uint8_t ascii_to_int(unsigned char val);

#endif
