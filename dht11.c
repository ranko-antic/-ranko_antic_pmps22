#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "dht11.h"
#include "delay.h"
#include"stdio.h"
#include"lcd.h"

void set_pin_input (void){
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;   
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void set_pin_output (void){
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DHT11_start_signal(void){
  set_pin_output();    // postavimo PIN1 kao izlaz
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0); // vrijednost izlaza na 0 
  delay_us(18000);    //pauza 18 ms
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);   // setujemo izlaz na 1
  delay_us(20);  // pauza 20 us
  set_pin_input();  // setujemo pin kao ulaz
}

uint8_t DHT11_check_response(void){
  uint8_t odziv = 0;
  delay_us(40);
  if(!(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1))) { //ako je signal na 1 => odziv = 1
    delay_us(80);
    if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)))	
      odziv = 1;
    else
      odziv = -1;
  }
  while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)))
    ;  // ceka se 0 na pinu 
  return odziv;
}

//PRIKUPLJANJE BITA SA SENZORA

uint8_t DHT11_read(void){
  uint8_t i,j;
  for(j = 0; j < 8; j++) {
    while(!(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)))
      ;   // cekamo visoki nivo, tj. log 1
    delay_us(40);   // provjeravamo da li se salje 0 ili 1
    if(!(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1))) {
      i &= ~(1 << (7 - j)); // upis 0 ;
    }
    else
      i |= (1 << (7 - j));  // upisemo 1
    while((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)))
      ;   // cekamo 0 
  }
  return i;
}
