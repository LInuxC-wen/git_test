#include "input_manager.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define SERVER_PORT 8888

typedef struct protocol{
	int tmp_iSocketClient;
	char ClientAddr[1000];
}TMP;

int g_iSocketServer;
int iSocketClient[4];
char getmcubuffer[1000];
void *func(void *tmp);

void *func(void *tmp)
{
	int iRecvLen;
	int sockfd;
	char aRecvBuf[1000];
	char tSocketClientAddr[1000];
	TMP *tmp1 = (TMP *)tmp;
	
	sockfd = tmp1->tmp_iSocketClient;
	strcpy(tSocketClientAddr,tmp1->ClientAddr);
	free(tmp);	
	
	while(1)
	{
	iRecvLen = recv(sockfd, aRecvBuf, 999, 0);
	
	if (iRecvLen > 0)
	{
		aRecvBuf[iRecvLen] = '\0';
		printf("Get Msg From %s : %s\n", tSocketClientAddr, aRecvBuf);
		strcpy(getmcubuffer,aRecvBuf);
	}
	else{
		printf("接收来自ip:%s的信息失败\n",tSocketClientAddr);
		printf("关闭来自ip:%s的socket\n",tSocketClientAddr);
		close(sockfd);
		break;
		}
	}
	
}

static int NetinputGetInputEvent(PInputEvent ptInputEvent)
{
	struct sockaddr_in tSocketClientAddr;
	int i=0;
	unsigned int iAddrLen = sizeof(struct sockaddr);
	pthread_t rcv;
	
	while(1)
	{
	while(i<=4)
	{
	iSocketClient[i] = accept(g_iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		printf("client:ip:%s   port:%d  \n",
			inet_ntoa(tSocketClientAddr.sin_addr),tSocketClientAddr.sin_port);
		
		TMP *tmp = (TMP *)malloc(sizeof(TMP));
		tmp->tmp_iSocketClient = iSocketClient[i];
		strcpy(tmp->ClientAddr, inet_ntoa(tSocketClientAddr.sin_addr));
		
        	pthread_create(&rcv,NULL,func, (void*)tmp);	
			
		i++;
	}
	}
}

static int NetinputDeviceInit(void)
{
	struct sockaddr_in tSocketServerAddr;
	int iRet;
	memset(&tSocketServerAddr,0,sizeof(tSocketServerAddr));
	g_iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == g_iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}
	
	iRet = listen(g_iSocketServer,5);
	if (-1 == iRet)
	{
		printf("listen error!\n");
		return -1;
	}

	return 0;
}

static int NetinputDeviceExit(void)
{
	close(g_iSocketServer);	
	return 0;
}


static InputDevice g_tNetinputDev ={
	.name = "Netinput",
	.GetInputEvent  = NetinputGetInputEvent,
	.DeviceInit     = NetinputDeviceInit,
	.DeviceExit     = NetinputDeviceExit,
};


void NetInputRegister(void)
{
	RegisterInputDevice(&g_tNetinputDev);
}

