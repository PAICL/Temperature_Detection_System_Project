#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "DHT11.h"
#include "oled.h"
#include "LED.h"
#include "Key.h"
#include "radio.h"
#include "llcc68.h"
#include "HAL_SPI.h"
#include "HAL_uart.h"
#include "project_config.h"

uint8_t DHT_Get_Temp_Humi_Data(uint8_t buffer[]);
void ExampleLLCC68SetingMode(uint8_t Lora_Mode, char sendData);//

#endif
