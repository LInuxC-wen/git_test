#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "bmp.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <input_manager.h>

static InputEvent event;

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
	pthread_t tid_input;
	LCD_bmp("led.bmp");
	
	ret =  pthread_create(&tid_input, NULL,thread_input, NULL);//创建thread_ts线程
	while(1)
	{
		if(50<event.iX&& event.iX<360 && 70<event.iY && event.iY<180)
		{
		printf("led on\n");
		return 0;
		}
		if(50<event.iX&& event.iX<360 && 210<event.iY && event.iY<320)
		{
		printf("led off\n");
		return 0;
		}
	}
}