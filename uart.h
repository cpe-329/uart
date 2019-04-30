/*
 * uart.h
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 */
#ifndef __UART_H__
#define __UART_H__

#include "msp.h"
#include "my_msp.h"

#define UART_PINS (P1_2 | P1_3)

void uart_init();

#endif
