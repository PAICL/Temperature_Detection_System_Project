#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__

//����semtech����������ֲ
//����������ַ https://github.com/Lora-net/LoRaMac-node/tree/master/src/radio  �������� 2021/2/3
#define SOFT_VERSION	"LLCC68 driver for stm32f103 V0.0.0"

//--------------------------------------------- ����Ĭ������ ---------------------------------------------
#define LORA_FRE									868500000	// �շ�Ƶ��  //ԭ����ֵ��868500000
#define LORA_TX_OUTPUT_POWER                        20        // ����Ĭ��ʹ�õķ��书�ʣ�126x���书��0~22dbm��127x���书��2~20dbm
#define LORA_BANDWIDTH                              1         // [0: 125 kHz,	����Ĭ��ʹ�õĴ���LLCC68��[0: 125 kHz,1: 250 kHz,2: 500 kHz,3: Reserved]
#define LORA_SPREADING_FACTOR                       9         // ����Ĭ��ʹ�õ���Ƶ���ӷ�Χ7~12
#define LORA_CODINGRATE                             1         // ����Ĭ��ʹ�õľ��������[1: 4/5,2: 4/6,3: 4/7,4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // ǰ���볤��
#define LORA_LLCC68_SYMBOL_TIMEOUT                  0         // Symbols(LLCC68�õ�����0,127x�õ�����5)
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false			// �Ƿ�Ϊ�̶����Ȱ�(��ʱֻ��LLCC68�õ���)
#define LORA_IQ_INVERSION_ON                        false			// ���Ӧ���������Ƿ�ת�жϵ�ƽ��(��ʱֻ��LLCC68�õ���)
#define LORA_RX_TIMEOUT_VALUE                       5000

/*!
 * Board MCU pins definitions
 */
//SPI
#define RADIO_NSS_PIN       GPIO_Pin_4
#define RADIO_NSS_PORT      GPIOA

#define RADIO_MOSI_PIN      GPIO_Pin_7
#define RADIO_MOSI_PORT     GPIOA

#define RADIO_MISO_PIN      GPIO_Pin_6
#define RADIO_MISO_PORT     GPIOA

#define RADIO_SCK_PIN       GPIO_Pin_5
#define RADIO_SCK_PORT      GPIOA

//RST��λ��
#define RADIO_nRESET_PIN    GPIO_Pin_1
#define RADIO_nRESET_PORT   GPIOB

//DIO1 ����
#define RADIO_DIO1_PIN      GPIO_Pin_11
#define RADIO_DIO1_PORT     GPIOB

//BUSY ����
#define RADIO_DIO4_BUSY_PIN      GPIO_Pin_0
#define RADIO_DIO4_BUSY_PORT     GPIOB               //ԭ����ֵΪGPIOA

#endif // __PROJECT_CONFIG_H__
