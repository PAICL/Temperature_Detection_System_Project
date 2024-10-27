#include "stm32f10x.h"
#include "stdio.h"

/*这段代码是用来支持printf打印到串口的代码*/
#pragma import(__use_no_semihosting)             
//标准库需要支持的函数
struct __FILE 
{
	int handle;
};

FILE __stdout;       
//定义_sys_exit()以避免工作在半主机状态
void _sys_exit(int x) 
{
	x = x; 
}
//重定义fputc函数
//这个需要根据MCU和我们希望printf从哪个串口输出来确认 __WAIT_TODO__
int fputc(int ch, FILE *f)
{
	//注意：USART_FLAG_TXE是检查发送缓冲区是否为空，这个要在发送前检查，检查这个提议提高发送效率，但是在休眠的时候可能导致最后一个字符丢失
	//USART_FLAG_TC是检查发送完成标志，这个在发送后检查，这个不会出现睡眠丢失字符问题，但是效率低（发送过程中发送缓冲区已经为空了，可以接收下一个数据了，但是因为要等待发送完成，所以效率低）
	//不要两个一起用，一起用效率最低
	
	//循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)              //原来的值为USART1
  {}
	USART_SendData(USART2, (uint8_t) ch);                                   //原来的值为USART1,下面都是

  /* 循环等待直到发送结束*/
  //while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  //{}

	return ch;
}

//USART2中断函数(接收)
void USART2_IRQHandler(void){
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
	{
		USART_SendData(USART2, USART_ReceiveData(USART2));
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
}

//uart1(PA_9 TX,PA_10 RX)初始化函数
uint8_t HALUart2Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_DeInit(USART2);  //复位串口
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);//开启串口时钟,注意APB1和APB2时钟使能函数不一样
	
	//PA_9 初始化为外设模式  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz，这里不用传参，直接写死用最大速度50MHZ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//片上外设，推挽模式(需要配合PSEL设置才能确定是复用哪个功能)
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO
	//PA_10初始化为上拉输入模式  //RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO

	//设置中断回调
	//中断优先级管理，当前中断优先级分组为2(NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2))已经定义，不要修改
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	//指定配置哪个中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//配置 NVIC


	//USART配置
	USART_InitStructure.USART_BaudRate = 115200;	//设置串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //配置USART参数

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开启接收中断(不开启无法接收数据,没有接收回调函数就不开启了，因为没有回调就没有处理接收的到数据，开启了也没有意义)
	USART_Cmd(USART2, ENABLE);                    //使能USART
	
	return 0;
}
