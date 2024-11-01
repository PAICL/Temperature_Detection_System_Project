#include "main.h"
#include "OLED.h"
#include "LLCC68_example_recive.h"
#include "radio.h"
#include "project_config.h"
#include "stdio.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "string.h"

gegegergegeggege

char DHT_recive_str[16];
uint8_t DHT_recive_Buffer[5];
/*!
 * Radio events function pointer
 * 这个是传参进入其他函数中了，所以用全局变量(局部变量使用完了内存释放可能导致异常)
 */
static RadioEvents_t LLCC68RadioEvents;

static void LLCC68OnTxDone( void );
static void LLCC68OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void LLCC68OnTxTimeout( void );
static void LLCC68OnRxTimeout( void );
static void LLCC68OnRxError( void );

//开启一个定时发送任务，每隔1S发送一条数据
void ExampleLLCC68ReciveDemo(void){
	uint32_t u32_count=0;
	//uint8_t OCP_Value = 0;
	//printf("start %s() example\r\n",__func__);
	
	LLCC68RadioEvents.TxDone = LLCC68OnTxDone;
	LLCC68RadioEvents.RxDone = LLCC68OnRxDone;
	LLCC68RadioEvents.TxTimeout = LLCC68OnTxTimeout;
	LLCC68RadioEvents.RxTimeout = LLCC68OnRxTimeout;
	LLCC68RadioEvents.RxError = LLCC68OnRxError;

	Radio.Init( &LLCC68RadioEvents );
	Radio.SetChannel(LORA_FRE);
	Radio.SetTxConfig( MODEM_LORA, LORA_TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                     LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                     LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );//参数：lora模式,发射功率,fsk用的lora设置为0就可以，带宽，纠错编码率，前导码长度，固定长度数据包(一般是不固定的所以选false)，crc校验，0表示关闭跳频，跳频之间的符号数(关闭跳频这个参数没有意义)，这个应该是表示是否要翻转中断电平的，超时时间

	//OCP_Value = Radio.Read(REG_OCP);
	//printf("[%s()-%d]read OCP register value:0x%04X\r\n",__func__,__LINE__,OCP_Value);
	
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_LLCC68_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, false );

	//printf("start enter rx mode\r\n");
	Radio.Rx( 0 );	//进入接收模式
	//printf("all setting\r\n");

	//printf("freq: %d\r\n Tx power: %d\r\n band width: %d\r\n FS: %d\r\n CODINGRATE: %d\r\n PREAMBLE_LENGTH: %d\r\n",LORA_FRE,LORA_TX_OUTPUT_POWER,LORA_BANDWIDTH,LORA_SPREADING_FACTOR,LORA_CODINGRATE,LORA_PREAMBLE_LENGTH);

	while(1){
		//Receiving_LED();
		Radio.IrqProcess( ); // Process Radio IRQ

		u32_count++;
		delay_ms(1);
	}
}

static void LLCC68OnTxDone( void )
{
	//printf("TxDone\r\n");
	Radio.Standby();
	
	//Done_LED();
	
	//发送完成闪烁一下led提示
	//GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	//delay_ms(100);
	//GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

static void LLCC68OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	OLED_Clear();
	uint32_t reciveNumber=0;
	Radio.Standby();
	//printf("RxDone\r\nsize:%d\r\nrssi:%d\r\nsnr:%d\r\n",size,rssi,snr);
	Radio.Rx( 0 );
	if(size > 0 ){
		
		if(size==4){
			memcpy(&reciveNumber,payload,4);
			//printf("recive u32 data=%d\r\n",reciveNumber);
		}else{
			//printf("recive data: %s\r\n",(char*)payload);
		}
		//接收成功闪烁一下led提示

		Done_LED();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(DHT_Get_Temp_Humi_Data(DHT_recive_Buffer))
		{
			sprintf(DHT_recive_str,"LTemp:%d.%d",DHT_recive_Buffer[2],DHT_recive_Buffer[3]);
			OLED_ShowString(1,1,(char *)DHT_recive_str);
			sprintf(DHT_recive_str,"LHumi:%d.%d",DHT_recive_Buffer[0],DHT_recive_Buffer[1]);
			OLED_ShowString(2,1,(char *)DHT_recive_str);
			delay_ms(200);
		}
		else
		{
			ERROR_LED();
		}
		
		sprintf(DHT_recive_str,"%s",(char *)payload);
		OLED_ShowString(3,1,(char *)DHT_recive_str);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		//delay_ms(100);
		//GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	}else {
		//printf("recive size !=4 is error\r\n");
	}
}

static void LLCC68OnTxTimeout( void )
{
	//printf("TxTimeout\r\n");
}

static void LLCC68OnRxTimeout( void )
{
	Radio.Standby();
	//printf("RxTimeout retry recive\r\n");
	Radio.Rx( LORA_RX_TIMEOUT_VALUE ); 
}

static void LLCC68OnRxError( void )
{
	Radio.Standby();
	//printf("RxError retry recive\r\n");
	Radio.Rx(LORA_RX_TIMEOUT_VALUE); 
}
