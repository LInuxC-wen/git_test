#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid                       "wen"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "19990809"           //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_TcpServer_IP                 "172.20.10.14"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "8888"               //Ҫ���ӵķ������Ķ˿�



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif

