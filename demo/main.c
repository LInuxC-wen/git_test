#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "bmp.h"
#include "chat.h"
#include "weather_client.h"
#include "input_manager.h"
#include "font_test.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


static InputEvent event;
extern int g_iSocketServer;//与MCU通信的服务端句柄
extern int iSocketClient[4];//与MCU通信的客户端句柄
extern char getmcubuffer[1000];//与MCU通信收到的信息
int touch_clear();//触摸屏数据初始化

void *thread_input (void *data) //thread_input线程
{
	int ret;
	//InputEvent event;
	
	InputInit();
	IntpuDeviceInit();

	while (1)
	{
		printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		ret = GetInputEvent(&event);

		printf("%s %s %d, ret = %d\n", __FILE__, __FUNCTION__, __LINE__, ret);
		if (ret) {
			printf("GetInputEvent err!\n");
			exit(0);
		}
		else
		{
			printf("%s %s %d, event.iType = %d\n", __FILE__, __FUNCTION__, __LINE__, event.iType );
			if (event.iType == INPUT_TYPE_TOUCH)
			{
				printf("Type      : %d\n", event.iType);
				printf("iX        : %d\n", event.iX);
				printf("iY        : %d\n", event.iY);
				printf("iPressure : %d\n", event.iPressure);
			}
			else if (event.iType == INPUT_TYPE_NET)
			{
				printf("Type      : %d\n", event.iType);
				printf("str       : %s\n", event.str);
			}
		}
	}
	
	
	
	
}


int main (void)
{
	int ret;
	char sendBuf[1000];
	pthread_t tid_input;
	ret =  pthread_create(&tid_input, NULL,thread_input, NULL);//创建thread_ts线程
	
	LCD_bmp("main.bmp");//主界面
	while(1)
	{	
		if(50<event.iX&& event.iX<360 && 70<event.iY && event.iY<180)//LED灯开
		{
			touch_clear();
			if(iSocketClient[0]!=0)
			{
				strcpy(sendBuf,LED_on);
				ret = send(iSocketClient[0],sendBuf,999,0);
				if(ret<=0)
					 { perror("send"); break; }
				 printf("发送：%s\n",sendBuf);
			}
		printf("led on\n");
		}
		else if(50<event.iX&& event.iX<360 && 210<event.iY && event.iY<320)//LED灯关
		{
			touch_clear();
			if(iSocketClient[0]!=0)
			{
				strcpy(sendBuf,LED_off);
				ret = send(iSocketClient[0],sendBuf,999,0);
				if(ret<=0)
					 { perror("send"); break; }
				printf("发送：%s\n",sendBuf);
			}
		printf("led off\n");
		}
		
		else if(50<event.iX&& event.iX<360 && 360<event.iY && event.iY<465)//天气预报
		{
		touch_clear();
		LCD_bmp("weather.bmp");
		ret = weather();
			while(1)
			{
			if(800<event.iX&& event.iX<1024 && 500<event.iY && event.iY<600)//退出
			{
				touch_clear();
				LCD_bmp("main.bmp");//主界面
				break;
			}
			}
		}
		
		else if(520<event.iX&& event.iX<800 && 60<event.iY && event.iY<180)//室内温湿度
		{
			touch_clear();
			LCD_bmp("humiture.bmp");
			if(iSocketClient[0]!=0)
			{
				strcpy(sendBuf,DHT11);
				ret = send(iSocketClient[0],sendBuf,999,0);
				if(ret<=0)
					 { perror("send"); break; }
				printf("发送：%s\n",sendBuf);
				while(strcmp(getmcubuffer,OK)== 0)
				{
					
				}
				font_test(130,180,50,getmcubuffer);
				
				while(1)
				{
				if(800<event.iX&& event.iX<1024 && 500<event.iY && event.iY<600)//退出
				{
					touch_clear();
					LCD_bmp("main.bmp");//主界面
					break;
				}
				}
			}
		}
		
	}
}



int touch_clear()
{
	event.iX = 0;
	event.iY = 0;
}

