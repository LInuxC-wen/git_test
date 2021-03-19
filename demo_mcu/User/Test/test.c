#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "SMD_test.h" 
#include "MicroStepDriver.h" 

volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };
char *led_on = {"led_on"};
char *led_off = {"led_off"};
char *dht11 ={"dht11"};
char *stepping_motor ={"stepping_motor"};
char *stepping_motor_exit ={"stepping_motor_exit"};

DHT11_Data_TypeDef DHT11_Data; //dht11信息结构体

/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	char *pRecStr = NULL; 
	char *tRecStr = NULL; 
	
  printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	
	sprintf ( cStr,"start\r\n" );
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //确认连接
	printf("%s 已发送\n",cStr);
		
	while ( 1 )
	{		
		
		pRecStr = ESP8266_ReceiveString(ENABLE);
		printf("%s\n",pRecStr);
		
		Delay_ms ( 100 );
		
		if(strcmp(pRecStr,led_on) ==0)																	//灯开
		{
			LED_G(ON);
			
			sprintf ( cStr,"OK" );
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
		}
		
		else if(strcmp(pRecStr,led_off) ==0)														//灯关
		{
			LED_G(OFF);
			
			sprintf ( cStr,"OK" );
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
		}
		
		else if(strcmp(pRecStr,dht11) ==0)															//温湿度传感器
		{
			if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				sprintf( cStr, "%d.%d,%d.%d ",
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
				
				ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );  
				
			}			
			else
			{
				sprintf(cStr, "Read DHT11 ERROR!\r\n");
			}
		}
		
		else if(strcmp(pRecStr,stepping_motor) ==0)														//电机控制
		{
			while(1)
			{
				
			tRecStr = ESP8266_ReceiveString(ENABLE);
			Delay_ms ( 100 );
			
			if(strcmp(tRecStr,stepping_motor_exit) ==0)												//退出电机控制
			{
				printf("%s\n",tRecStr);
				break;
			}
			
			else																															//控制具体参数
			{
				printf("%s\n",tRecStr);
				DealSerialData(tRecStr);
				sprintf ( cStr,"OK" );
				ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
			}
		  
			}
	
			sprintf ( cStr,"OK" );
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
		}
		
		if ( ucTcpClosedFlag )                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}
	
		
}


