#ifndef __LED_H
#define __LED_H

void DEBUG_LED_Init(void);
void Loading_LED(void);
void ERROR_LED(void);
void Send_LED(void);
void Sending_LED(void);
void Receive_LED(void);
void Receiving_LED(void);
void Testing_LED_Init(void);
void Debug_Temp1_Turn(void);
void Debug_Temp2_Turn(void);
void Debug_Temp1_Off(void);
void Debug_Temp1_Off(void);
void Done_LED(void);

#define DEBUG1_LED_PORT GPIOB
#define DEBUG1_LED_GPIO GPIO_Pin_9
#define DEBUG1_LED_RCC RCC_APB2Periph_GPIOB

#define DEBUG2_LED_PORT GPIOB
#define DEBUG2_LED_GPIO GPIO_Pin_8
#define DEBUG2_LED_RCC RCC_APB2Periph_GPIOB

#define DEBUG3_LED_PORT GPIOB
#define DEBUG3_LED_GPIO GPIO_Pin_5
#define DEBUG3_LED_RCC RCC_APB2Periph_GPIOB

#define DEBUG4_LED_PORT GPIOB
#define DEBUG4_LED_GPIO GPIO_Pin_4
#define DEBUG4_LED_RCC RCC_APB2Periph_GPIOB

#endif
