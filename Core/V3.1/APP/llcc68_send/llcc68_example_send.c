#include "main.h"

#include "LLCC68_example_send.h"
#include "radio.h"
#include "project_config.h"
#include "stdio.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "string.h"

/*!
 * Radio events function pointer
 * ����Ǵ��ν��������������ˣ�������ȫ�ֱ���(�ֲ�����ʹ�������ڴ��ͷſ��ܵ����쳣)
 */
static RadioEvents_t LLCC68RadioEvents;

//void DHT_Get_Temp_Humi_Data1(uint8_t DHT_Send_Buffer[5]);

//uint8_t result = DHT_Get_Temp_Humi_Data(DHT_Send_Buffer);

//if(DHT_Get_Temp_Humi_Data(DHT_Send_Buffer))
//	{
//		sendData[20] = DHT_Send_Buffer[2];
//	}

//char middle_string[20] = "Hello World";

char DHT_Send_str[20];
uint8_t DHT_Send_Buffer[5];
//const 
char sendData[] = "Temp:";




//const char sendData = DHT_Send_Buffer[2];
//const char sendData[20] = DHT_Send_Buffer[3];
//const char sendData[20] = DHT_Send_Buffer[0];
//const char sendData[20] = DHT_Send_Buffer[1];

static void LLCC68OnTxDone( void );
static void LLCC68OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
static void LLCC68OnTxTimeout( void );
static void LLCC68OnRxTimeout( void );
static void LLCC68OnRxError( void );

//����һ����ʱ��������ÿ��1S����һ������
void ExampleLLCC68SendDemo(void){

	

if(DHT_Get_Temp_Humi_Data(DHT_Send_Buffer))
		{
			//sprintf(DHT_Send_str,"LTemp:%d.%d",DHT_Send_Buffer[2],DHT_Send_Buffer[3]);
			//OLED_ShowString(1,1,(char *)DHT_Send_Buffer);
			//sprintf(DHT_Send_str,"LHumi:%d.%d",DHT_Send_Buffer[0],DHT_Send_Buffer[1]);
			//OLED_ShowString(2,1,(char *)DHT_Send_Buffer);

        	strcat((char *)sendData, (int8_t *)DHT_Send_Buffer[2]);
		}
		else
		{
			ERROR_LED();
		}



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
                     true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
					 //������loraģʽ,���书��,fsk�õ�lora����Ϊ0�Ϳ��ԣ�������������ʣ�ǰ���볤�ȣ��̶��������ݰ�(һ���ǲ��̶�������ѡfalse)��crcУ�飬0��ʾ�ر���Ƶ����Ƶ֮��ķ�����(�ر���Ƶ�������û������)�����Ӧ���Ǳ�ʾ�Ƿ�Ҫ��ת�жϵ�ƽ�ģ���ʱʱ��
	//OCP_Value = Radio.Read(REG_OCP);
	//printf("[%s()-%d]read OCP register value:0x%04X\r\n",__func__,__LINE__,OCP_Value);
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_LLCC68_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, false );
/////////////////////////////////////////////////////////////////////////////////////////////
//����Ĵ����debug

	//printf("all setting\r\n");
	//printf("freq: %d\r\n Tx power: %d\r\n band width: %d\r\n FS: %d\r\n CODINGRATE: %d\r\n PREAMBLE_LENGTH: %d\r\n",LORA_FRE,LORA_TX_OUTPUT_POWER,LORA_BANDWIDTH,LORA_SPREADING_FACTOR,LORA_CODINGRATE,LORA_PREAMBLE_LENGTH);
	while(1){
		Radio.IrqProcess( ); // Process Radio IRQ
		
		//Sending_LED();  //2

		if(0==u32_count%1000){
			//printf("systick=%d ,send u32 data:%d\r\n", Get_SysTick(),u32_count);
			if(0==u32_count%2000){
				Radio.Send((uint8_t *)&u32_count,4);
			}else
				Radio.Send((uint8_t *)sendData,(strlen(sendData)+1));
		}
		u32_count++;
		delay_ms(1);
	}
}

static void LLCC68OnTxDone( void )
{
	//printf("TxDone\r\n");
	Radio.Standby();
	//���������˸һ��led��ʾ
	Done_LED();
	sprintf(DHT_Send_str,"Data sended");
	OLED_ShowString(1,1,(char *)DHT_Send_str);

	if(DHT_Get_Temp_Humi_Data(DHT_Send_Buffer))
		{
			sprintf(DHT_Send_str,"%d.%d",DHT_Send_Buffer[2],DHT_Send_Buffer[3]);
			OLED_ShowString(2,1,(char *)DHT_Send_str);
			sprintf(DHT_Send_str,"%d.%d",DHT_Send_Buffer[0],DHT_Send_Buffer[1]);
			OLED_ShowString(3,1,(char *)DHT_Send_Buffer);
		}
		else
		{
			ERROR_LED();
		}

	//sendData = "Hello world!";
	
	//GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	//delay_ms(100);
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);
}

static void LLCC68OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	uint32_t reciveNumber=0;
	Radio.Standby();
	//printf("RxDone\r\nsize:%d\r\nrssi:%d\r\nsnr:%d\r\n",size,rssi,snr);
	Radio.Rx( LORA_RX_TIMEOUT_VALUE );
	if(size!=4){
		//printf("recive size !=4 is error\r\n");
	}else{
		memcpy(&reciveNumber,payload,4);
		//printf("recive u32 data=%d\r\n",reciveNumber);
		//���ճɹ���˸һ��led��ʾ
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
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


