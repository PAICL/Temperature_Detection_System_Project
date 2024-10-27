#include "HAL_SPI.h"
#include "project_config.h"



//SPI初始化(LLCC68和sx127x使用的SPI配置相同的，可以统一初始化)
uint8_t Spi1Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	//初始化SPI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//SPI时钟使能
	//初始化MISO,MOSI,SCK为推挽外设模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能指定端口时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz，这里不用传参，直接写死用最大速度50MHZ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	//片上外设推挽模式
	GPIO_InitStructure.GPIO_Pin = RADIO_SCK_PIN|RADIO_MISO_PIN|RADIO_MOSI_PIN;
	GPIO_Init(RADIO_SCK_PORT, &GPIO_InitStructure);	//初始化GPIO

	//配置SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//设置时钟极性(串行同步时钟的空闲状态为高电平还是低电平)
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//设置相位(串行同步时钟的第几个跳变沿（上升或下降）数据被采样)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:设置为软件控制(SSI控制)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//定义波特率预分频的值:波特率预分频值为256(256是最低,可以设置完成之后调整速度,如果速度过快导致通信失败再调小)
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式(CRC校验相关)
	SPI_Init(SPI1,&SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI1,ENABLE); //使能SPI外设
	
	//初始化NSS脚为输出高电平 RADIO_NSS_PIN		RADIO_NSS_PORT
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能指定端口时钟
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz，这里不用传参，直接写死用最大速度50MHZ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Pin = RADIO_NSS_PIN;
	GPIO_Init(RADIO_NSS_PORT, &GPIO_InitStructure);	//初始化GPIO
	//GPIO_ResetBits(gpioObj->portIndex,gpioObj->pinIndex);	//输出低电平
	GPIO_SetBits(RADIO_NSS_PORT,RADIO_NSS_PIN);	//输出高电平
	
	return 0;
}

//spi传输数据(返回值是接收到的数据)
//参数：
//     spiName	:需要通过哪个spi传输数据
//     data		:要发送的数据
//返回值：
//       接收到的数据
//注意：这里没有控制CS信号，需要自己控制（通过一个普通的GPIO控制就可以）
uint8_t HALSpi1InOut(uint8_t data){
	uint32_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){//检查指定的SPI标志位设置与否:发送缓存空标志位(缓冲区空了就可以拷贝数据了)
		if((retry++)>2000){
			//超时了
			return 0xff;
		}
	}
	SPI_I2S_SendData(SPI1, data); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){ //检查指定的SPI标志位设置与否:接受缓存非空标志位（非空了就表示接收数据完成了）
		if((retry++)>2000){
			//超时了
			return 0xff;
		}
	}
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}
