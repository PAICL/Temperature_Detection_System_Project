#ifndef __KEY_H
#define __KEY_H

void Key_Init(void);
uint8_t Key_GetNum(void);

#define USER_Key_1_Port GPIOB
#define USER_Key_1_GPIO GPIO_Pin_3
#define USER_Key_2_Port GPIOA
#define USER_Key_2_GPIO GPIO_Pin_15

#endif
