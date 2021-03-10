#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 
#include "read_cJSON.h"

#define CLIENT_PORT 80
#define server_IP "123.57.54.168"

int weather(void)
{
  
  // 第1步：创建客户端的socket。
  int sockfd;
  if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
 
  // 第2步：向服务器发起连接请求。
  struct hostent* h;
  if ( (h = gethostbyname(server_IP)) == 0 )   // 指定服务端的ip地址。
  { printf("gethostbyname failed.\n"); close(sockfd); return -1; }
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(CLIENT_PORT); // 指定服务端的通信端口。
  memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
  if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  // 向服务端发起连接清求。
  { perror("connect"); close(sockfd); return -1; }
 
  char buffer[1024*1024];
 
  // 第3步：与服务端通信，发送一个报文后等待回复，然后再发下一个报文。
    int iret;
    memset(buffer,0,sizeof(buffer));
    sprintf(buffer,"GET http://api.yytianqi.com/observe?city=123.113.202.176&key=53fojkolhat2vf72\r\n");
    if ( (iret=send(sockfd,buffer,strlen(buffer),0))<=0) // 向服务端发送请求报文。
    { perror("send"); }
    printf("发送：%s\n",buffer);
 
    memset(buffer,0,sizeof(buffer));
    if ( (iret=recv(sockfd,buffer,sizeof(buffer),0))<=0) // 接收服务端的回应报文。
    {
       printf("iret=%d\n",iret);
    }
    printf("接收：%s\n",buffer);
    
	doit(buffer);
 
  // 第4步：关闭socket，释放资源。
  close(sockfd);
  
  return 0;
}
