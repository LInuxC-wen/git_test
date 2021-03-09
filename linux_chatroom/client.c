#include "chat.h"

int registe(int fd);
int login  (int fd);
int logout (int fd);
int broadcast (int fd);
void *func(void *arg);

int login_f = -1;
int sockfd;

int main(int argc,char **argv)
{
  int portnumber;
  int addrlen;
  int sel; //功能选择
  int ret; //登陆返回值
  
  pthread_t pid;
  struct sockaddr_in   server_addr;
  char buffer[520];

 sockfd = socket(AF_INET,SOCK_STREAM,0);
 if(sockfd<0)
{
  perror("socket() fail\n");
  return;
}
  portnumber=atoi(argv[2]);//将端口转化为int型

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(portnumber);

  addrlen = sizeof(struct sockaddr_in);

  connect(sockfd,(struct sockaddr* )&server_addr,addrlen);

  pthread_create(&pid,NULL,func,NULL);

  while(1)
  {
   printf("\t 1 注册 \n");
   printf("\t 2 登录 \n");
   printf("\t 3 公聊 \n");
   printf("\t 0 退出 \n");

   scanf("%d",&sel);
  
  switch(sel)
	{
	case 1:
	registe(sockfd);
	break;
	
	case 2:
	ret = login(sockfd);
	break;

	case 3:
	broadcast(sockfd);
	break;

	case 0:
	logout(sockfd);
	break;

	default:
	break;
        }
	
  if(sel == 0)
	{
	exit(0);
	}

   }

    close(sockfd);
    exit(0);
}


int  registe(int fd)
   {
    struct protocol msg;
    struct protocol msgback;

    msg.cmd = REGISTE;

    printf("input your name\n");	
    scanf("%s",msg.name);
    printf("input your passwd\n");	
    scanf("%s",msg.data);
   
    write(fd,&msg,sizeof(msg));
    read(fd,&msgback,sizeof(msgback));   
   
    if(msgback.state != OP_OK)
    {
     printf("Name had exist,try again!\n");	
    }
    else
    {
     printf("Regist success!\n");
    }
}

int login(int fd)
{
 	struct protocol msg;
 	struct protocol msg_back;

	msg.cmd = LOGIN;
	printf("input your name\n");
	scanf("%s",msg.name);
	printf("input your passwd\n");
	scanf("%s",msg.data);

	write(fd,&msg,sizeof(msg));
	read(fd,&msg_back,sizeof(msg_back));

	if(msg_back.state != OP_OK)
	{
		printf("Name had exist,try again!\n");
		getchar();
		getchar();
		login_f = -1;
		return NAME_PWD_NMATCH;
	}else{
		printf("Login success!\n");
		getchar();
		getchar();
		login_f = 1;
		return OP_OK  ;
	}

}


int logout (int fd)
{
	struct protocol msg,msg_back;

	if(login_f == -1)
	{
	 close(fd);
         return 0;	 
	}

	msg.cmd = LOGOUT;

	write(fd,&msg,sizeof(msg));
        read(fd,&msg_back,sizeof(msg_back));
	
	if(msg_back.state = ONLINEUSER_OVER)
	{
	close(fd);
	login_f = -1;
	}
}


int broadcast (int fd)
{
  	struct protocol msg;

	msg.cmd = BROADCAST;
	printf("say:\n#");
	scanf("%s",msg.data);
	printf("消息已发送%s\n",msg.data);
	write(fd,&msg,sizeof(msg));
}

void *func(void *arg)
{
  struct protocol *msg;
  char buf[520];
  int len;

  while(1)
  {
  	if(login_f != 1)
	{
	continue;
	}	
	
	memset(buf,0,sizeof(buf));
	len = read(sockfd,buf,sizeof(buf));	
	//buf[len]='\0';
	printf("%s\n",buf);
  }
}



