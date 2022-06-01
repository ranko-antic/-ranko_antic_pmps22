#include "delay.h"
#include "dht11.h"
#include "lcd.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

uint8_t RH_integral, RH_decimal, TEMP_integral, TEMP_decimal;
uint16_t CHECKSUM, RH, TEMP;
float temperature = 0.0f;
volatile float setTemperature = 20.0f;
uint8_t presence = 0;
const float increment = 1.0f;
LCD_TypeDef *pom;

#define LCD_D6_Pin GPIO_PIN_2
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_3
#define LCD_D7_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_4
#define LCD_RS_GPIO_Port GPIOE
#define LCD_RW_Pin GPIO_PIN_5
#define LCD_RW_GPIO_Port GPIOE
#define LCD_E_Pin GPIO_PIN_6
#define LCD_E_GPIO_Port GPIOE
#define LCD_D4_Pin GPIO_PIN_0
#define LCD_D4_GPIO_Port GPIOE
#define LCD_D5_Pin GPIO_PIN_1
#define LCD_D5_GPIO_Port GPIOE

PortPin_Map LCD_RS = {.GPIOx = LCD_RS_GPIO_Port, .GPIO_Pin_x = LCD_RS_Pin};
PortPin_Map LCD_RW = {.GPIOx = LCD_RW_GPIO_Port, .GPIO_Pin_x = LCD_RW_Pin};
PortPin_Map LCD_E = {.GPIOx = LCD_E_GPIO_Port, .GPIO_Pin_x = LCD_E_Pin};
PortPin_Map LCD_D4 = {.GPIOx = LCD_D4_GPIO_Port, .GPIO_Pin_x = LCD_D4_Pin};
PortPin_Map LCD_D5 = {.GPIOx = LCD_D5_GPIO_Port, .GPIO_Pin_x = LCD_D5_Pin};
PortPin_Map LCD_D6 = {.GPIOx = LCD_D6_GPIO_Port, .GPIO_Pin_x = LCD_D6_Pin};
PortPin_Map LCD_D7 = {.GPIOx = LCD_D7_GPIO_Port, .GPIO_Pin_x = LCD_D7_Pin};

static void LCD_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOE,
                    LCD_D6_Pin | LCD_D7_Pin | LCD_RS_Pin | LCD_RW_Pin |
                        LCD_E_Pin | LCD_D4_Pin | LCD_D5_Pin,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = LCD_D6_Pin | LCD_D7_Pin | LCD_RS_Pin | LCD_RW_Pin |
                        LCD_E_Pin | LCD_D4_Pin | LCD_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
void set_TEMP(void);
void check_TEMP(float);

void display_TEMP(LCD_TypeDef *lcd, float temp) {
  char str[20] = {0};
  char str1[20] = {0};
  LCD_setCursor(lcd, 0, 0);
  LCD_clearScreen(lcd);
  sprintf(str, "SetTemp:  %.1f C", setTemperature);
  LCD_putstr(lcd, (uint8_t *)(&str));
  LCD_setCursor(lcd, 1, 0);
  sprintf(str1, "CurrTemp: %.1f C", temp);
  LCD_putstr(lcd, (uint8_t *)(&str1));
  delay_ms(2000);
}

int main(void) {
  HAL_Init();
  initSYSTIM();

  // konfiguracija pinova za ventilator i grijalicu:
  GPIO_InitTypeDef GPIO_InitStruct_;
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct_.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct_.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_);

  //zbog inverzne logike dvokanalnog releja
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);

  
  // konfiguracija pinova za  push button-e:
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

