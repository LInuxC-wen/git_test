/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   WF-ESP8266 WiFi模块测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_dht11.h"
#include "SMD_test.h" 
#include "MicroStepDriver.h" 
#include "bsp_exti.h" 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	/* 初始化 */
  USARTx_Config ();                                                              //初始化串口1
	SysTick_Init ();                                                               //配置 SysTick 为 1ms 中断一次 
	ESP8266_Init ();                                                               //初始化WiFi模块使用的接口和外设
  LED_GPIO_Config();																														 //初始化LED
	DHT11_Init ();																																	//初始化DTT11
	SMD_Init();																																			//初始化电机
	EXTI_Key_Config();																															//按键中断初始化
	printf ( "\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n" );                          //打印测试例程提示信息
	

	
  ESP8266_StaTcpClient_UnvarnishTest ();
	
	
  while ( 1 );
	
	
}


/*********************************************END OF FILE**********************/
