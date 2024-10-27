#include "main.h"

/*
DEBUG1_LED_PORT    GPIOB
DEBUG1_LED_GPIO    GPIO_Pin_9
DEBUG1_LED_RCC     RCC_APB2Periph_GPIOB

DEBUG2_LED_PORT    GPIOB
DEBUG2_LED_GPIO    GPIO_Pin_8
DEBUG2_LED_RCC     RCC_APB2Periph_GPIOB

DEBUG3_LED_PORT    GPIOB
DEBUG3_LED_GPIO    GPIO_Pin_5
DEBUG3_LED_RCC     RCC_APB2Periph_GPIOB

DEBUG4_LED_PORT    GPIOB
DEBUG4_LED_GPIO    GPIO_Pin_4
DEBUG4_LED_RCC     RCC_APB2Periph_GPIOB
*/

/*
GPIO_ResetBits(GPIOX, GPIO_Pin_X);//将PA0引脚设置为低电平
GPIO_SetBits(GPIOX, GPIO_Pin_X);//将PA0引脚设置为高电平
GPIO_WriteBit(GPIOX, GPIO_Pin_X, Bit_RESET);//将PA0引脚设置为低电平
GPIO_WriteBit(GPIOX, GPIO_Pin_X, Bit_SET);//将PA0引脚设置为高电平
GPIO_WriteBit(GPIOX, GPIO_Pin_X, (BitAction)0);//将PA0引脚设置为低电平
GPIO_WriteBit(GPIOX, GPIO_Pin_X, (BitAction)1);//将PA0引脚设置为高电平
*/

void DEBUG_LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB2PeriphClockCmd(DEBUG1_LED_RCC|DEBUG2_LED_RCC|DEBUG3_LED_RCC|DEBUG4_LED_RCC, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DEBUG1_LED_GPIO|DEBUG2_LED_GPIO|DEBUG3_LED_GPIO|DEBUG4_LED_GPIO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(DEBUG1_LED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(DEBUG1_LED_PORT, DEBUG1_LED_GPIO);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);

	GPIO_Init(DEBUG2_LED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(DEBUG2_LED_PORT, DEBUG2_LED_GPIO);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);

	GPIO_Init(DEBUG3_LED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(DEBUG3_LED_PORT, DEBUG3_LED_GPIO);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);

	GPIO_Init(DEBUG4_LED_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(DEBUG4_LED_PORT, DEBUG4_LED_GPIO);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
}

void Testing_LED_Init(void)
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(500);
}
/*
当温湿度传感器接收到一次数据后闪一下
也可以设置为需要调试的时候才驱一次
基本设想上就是调式时的一次性代码
*/

//循环一次500ms
void Loading_LED()
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	
	delay_ms(200);
}
/*
当传入变量为“1”时，debug一直旋转
*/
                                                         

void ERROR_LED(void)
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)0);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(100);
}
/*
当传入参数为1时，四个debug灯一直闪
*/

void Send_LED(void)
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(200);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(200);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(200);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(200);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(200);
}

void Sending_LED(void)
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	
	delay_ms(200);
}

void Receive_LED(void)
{
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(200);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
}

//4231
void Receiving_LED(void)
{
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);

	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	delay_ms(100);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);

	delay_ms(200);
}

void Done_LED(void)
{
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(200);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)0);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)0);
	delay_us(500);
	GPIO_WriteBit(DEBUG1_LED_PORT, DEBUG1_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG2_LED_PORT, DEBUG2_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG3_LED_PORT, DEBUG3_LED_GPIO, (BitAction)1);
	GPIO_WriteBit(DEBUG4_LED_PORT, DEBUG4_LED_GPIO, (BitAction)1);
	delay_ms(200);
}

void Debug_Temp1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}

void Debug_Temp2_Turn(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}


void Debug_Temp1_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}


void Debug_Temp2_Off(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{                                                  
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}                                                  
	else
	{                                                  
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}
