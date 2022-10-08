#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

void set_pin_input (void);
void set_pin_output(void);
void DHT11_start_signal(void);
uint8_t DHT11_check_response(void);
uint8_t DHT11_read(void);

#endif 
