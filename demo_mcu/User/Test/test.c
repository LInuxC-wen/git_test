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

DHT11_Data_TypeDef DHT11_Data; //dht11��Ϣ�ṹ��

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	char *pRecStr = NULL; 
	char *tRecStr = NULL; 
	
  printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n���� ESP8266 ���\r\n" );
	
	sprintf ( cStr,"start\r\n" );
	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //ȷ������
	printf("%s �ѷ���\n",cStr);
		
	while ( 1 )
	{		
		
		pRecStr = ESP8266_ReceiveString(ENABLE);
		printf("%s\n",pRecStr);
		
		Delay_ms ( 100 );
		
		if(strcmp(pRecStr,led_on) ==0)																	//�ƿ�
		{
			LED_G(ON);
			
			sprintf ( cStr,"OK" );
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
		}
		
		else if(strcmp(pRecStr,led_off) ==0)														//�ƹ�
		{
			LED_G(OFF);
			
			sprintf ( cStr,"OK" );
			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               
		}
		
		else if(strcmp(pRecStr,dht11) ==0)															//��ʪ�ȴ�����
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
		
		else if(strcmp(pRecStr,stepping_motor) ==0)														//�������
		{
			while(1)
			{
				
			tRecStr = ESP8266_ReceiveString(ENABLE);
			Delay_ms ( 100 );
			
			if(strcmp(tRecStr,stepping_motor_exit) ==0)												//�˳��������
			{
				printf("%s\n",tRecStr);
				break;
			}
			
			else																															//���ƾ������
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
		
		if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}
	}
	
		
}


