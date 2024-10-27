/*







*/

/*
硬件介绍
LoRa Module:
	LoRa Model		  	  	E220-400M22S
	SPI1_NSS_PIN			PA4
	SPI1_MOSI_PIN      		PA7
	SPI1_MISO_PIN     		PA6
	SPI1_SCK_PIN       		PA5
	RESET_PIN    		    PB1
	BUSY_PIN        	    PB0
	DIO1_PIN      		    PB11
	USRAT2_TX_PIN			PA_9
	USART2_RX_PIN			PA_10

DHT11 Temperature & Humidity Sensor Module
	Sensor Model			DHT11
	DATA_PIN                PA8
	DHT11_VCC               PA9

OLED Display Module
	Display Model			SSD1306(64*128)
	I2C1_SDA_PIN			PB7
	I2C1_SCL_PIN			PB6

USER Key
	NRST_Key				RSW
	WKUP_KEY				PA0
	USER_KEY1				PB3(--VCC)
	USER_KEY2				PA15(--VCC)

DeBug LED
	POWER_LED
	TAMPER_RTC				PC13
	DEBUG1(右下)			PB9(--VCC)
	DEBUG2(左下)			PB8(--VCC)
	DEBUG3(左上)			PB5(--VCC)
	DEBUG4(右上)			PB4(--VCC)
*/

#include "main.h"

//创建一般字符数据的参数
char str[20];

//创建LoRa模式参数
uint8_t Lora_Mode = 0;//默认为0，接受选1，发送选2

//创建接受字符数据的参数
char DHT_recive_str[16];

//创建发送数据的参数
char sendData[20];

//创建温湿度传感器数据的参数
char DHT_str[16];
uint8_t DHT_Buffer[5];


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
void display_setting_messing(void);
void DHT_display(void);

/* _|______________________________ 进入程序运行模式 mian(void) { … } _____________________________________________ */

int main(void)
{
/* _______________________________ 加载系统基础配置 _____________________________________________ */
	DEBUG_LED_Init();			//DeBug灯加载配置
	OLED_Init();				//显示模块加载配置
	//DHT_Power(1);				//开启DHT11传感器的电源开关
	Key_Init();					//用户交互按键加载配置
	
/* _______________________________ 进入基础用户界面和交互 _____________________________________________ */
	Loading_LED();				//LED灯旋转一圈

	//显示内容用户选择以哪种模式加载
	sprintf(str,"Choose your mode");
	OLED_ShowString(1,1,(char *)str);
	sprintf(str,"Left to master");
	OLED_ShowString(2,1,(char *)str);
	sprintf(str,"Right to sleave");
	OLED_ShowString(3,1,(char *)str);
	Testing_LED_Init();
	delay_s(1);

/*	
	//用户交互按键检测
	for(uint8_t Key_Mode = 0; Key_Mode == 0; )//可能需要修改为使用case函数来选择
	{
		Key_Mode = Key_GetNum();
		if(Key_Mode == 1){
		Lora_Mode = 1;
		Send_LED();
		break;
		}	
		if(Key_Mode == 2){
		Lora_Mode = 2;
		Receive_LED();
		break;
		}
	}
	OLED_Clear();
*/	

	Lora_Mode = 1;///////////////////////////////////////////////////////暂定值
	



	if(Lora_Mode == 1 || Lora_Mode == 2){
		if(Lora_Mode == 1){
			OLED_Clear();
			sprintf(str,"Enter R_Mode");
			OLED_ShowString(1,1,(char *)str);
			Sending_LED();
			delay_s(1);
		}
		if(Lora_Mode == 2){
			OLED_Clear();
			sprintf(str,"Enter S_Mode");
			OLED_ShowString(1,1,(char *)str);
			Receiving_LED();
			delay_s(1);
		}
	}
	else{
		ERROR_LED();
	}
	OLED_Clear();


/* _____|__________________________ 进入循环模式 While(1) { … } _____________________________________________ */

	while (1)
	{
		Loading_LED();	

/* _________|______________________发送数据（Lora_Mode == 1）_____________________________________________ */
		if(Lora_Mode == 1){
			Sending_LED();
			Loading_LED();
			ExampleLLCC68SetingMode(Lora_Mode,0);
			delay_ms(2);
		}


/* _________|______________________接受数据（Lora_Mode == 2）_____________________________________________ */
		if(Lora_Mode == 2){
			Receiving_LED();
			Loading_LED();
			ExampleLLCC68SetingMode(Lora_Mode,*sendData);
			delay_ms(2);
		}
		delay_ms(250);
	}
}	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DHT_display(void){
	sprintf(DHT_str,"%d.%d|%d.%d",DHT_Buffer[2],DHT_Buffer[3],DHT_Buffer[0],DHT_Buffer[1]);
	OLED_ShowString(2,3,(char *)DHT_str);
}

void ExampleLLCC68SetingMode(uint8_t Lora_Mode,char send_Data){

	//const char sendData[20] = send_Data;
	//sendData ;
	
	uint32_t u32_count=0;
	uint8_t OCP_Value = 0;
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
	//参数：lora模式,发射功率,fsk用的lora设置为0就可以，带宽，纠错编码率，前导码长度，固定长度数据包(一般是不固定的所以选false)，crc校验，0表示关闭跳频，跳频之间的符号数(关闭跳频这个参数没有意义)，这个应该是表示是否要翻转中断电平的，超时时间

	OCP_Value = Radio.Read(REG_OCP);
	//printf("[%s()-%d]read OCP register value:0x%04X\r\n",__func__,__LINE__,OCP_Value);
	
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                     LORA_LLCC68_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                     0, true, 0, 0, LORA_IQ_INVERSION_ON, false );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(Lora_Mode == 1){
		snprintf(str,sizeof(str),"start enter rx mode\r\n");
		OLED_ShowString(4,1,(char *)str);
		Radio.Rx( 0 );	//进入接收模式
		snprintf(str,sizeof(str),"all setting");

		
		
		while(1){
			Radio.IrqProcess( ); // Process Radio IRQ
			u32_count++;
			delay_ms(1);
		}
	}

	if(Lora_Mode == 2){
		while(1){

		


		Radio.IrqProcess( ); // Process Radio IRQ
		
		if(0==u32_count%1000){
		
		if(DHT_Get_Temp_Humi_Data(DHT_Buffer)){
			snprintf(DHT_str, sizeof(DHT_str), "%d.%d|%d.%d", DHT_Buffer[2],DHT_Buffer[3], DHT_Buffer[0], DHT_Buffer[1]);
    		//OLED_ShowString(3,1,(char *)DHT_str);
			/*将温湿度参数传递给sendData*/
			strncpy(sendData, DHT_str, sizeof(sendData) - 1);
			sendData[sizeof(sendData) - 1] = '\0';
			//sprintf(str,"%s",sendData);
			OLED_ShowString(4,2,sendData);
		}

			//printf("systick=%d ,send u32 data:%d\r\n", Get_SysTick(),u32_count);
			if(0==u32_count%2000){
				Radio.Send((uint8_t *)&u32_count,4);
			}else
				Radio.Send((uint8_t *)sendData,(strlen(sendData)+1));
		}
		u32_count++;
		delay_ms(1);
		//OLED_Clear();
		}
	}
}

static void LLCC68OnTxDone( void )
{
	//printf("TxDone\r\n");
	Radio.Standby();
	
	//发送完成闪烁一下led提示
	Done_LED();
}

static void LLCC68OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
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

		if(DHT_Get_Temp_Humi_Data(DHT_Buffer))
		{
			/*
			sprintf(DHT_str,"L_Temp:%d.%d",DHT_Buffer[2],DHT_Buffer[3]);
			OLED_ShowString(1,1,(char *)DHT_str);
			sprintf(DHT_str,"L_Humi:%d.%d",DHT_Buffer[0],DHT_Buffer[1]);
			OLED_ShowString(2,1,(char *)DHT_str);
			*/
			sprintf(DHT_str,"Temp|Humi");
			OLED_ShowString(1,3,(char *)DHT_str);
			sprintf(DHT_str,"L:");
			OLED_ShowString(2,1,(char *)DHT_str);
			sprintf(DHT_str,"S:");
			OLED_ShowString(3,1,(char *)DHT_str);

			DHT_display();

			//sprintf(DHT_str,"%d.%d|%d.%d",DHT_Buffer[2],DHT_Buffer[3],DHT_Buffer[0],DHT_Buffer[1]);
			//OLED_ShowString(2,3,(char *)DHT_str);
		}


		//显示接受到的数据//////////////////////////////////////////////////////////////////
		sprintf(DHT_recive_str,"%s",(char *)payload);
		OLED_ShowString(3,3,(char *)DHT_recive_str);

	}else {
		ERROR_LED();
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

void display_setting_messing(void)
{
	snprintf(str,sizeof(str),"freq: %d",LORA_FRE);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
	snprintf(str,sizeof(str),"Tx power: %d",LORA_TX_OUTPUT_POWER);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
	snprintf(str,sizeof(str),"band width: %d",LORA_BANDWIDTH);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
	snprintf(str,sizeof(str),"FS: %d",LORA_SPREADING_FACTOR);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
	snprintf(str,sizeof(str),"CODINGRATE: %d",LORA_CODINGRATE);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
	snprintf(str,sizeof(str),"PREAMBLE_LENGTH: %d\r\n",LORA_PREAMBLE_LENGTH);
	OLED_ShowString(4,1,(char *)str);
	delay_ms(500);
}




















