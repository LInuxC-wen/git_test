#include "chat.h"

struct ONLINE online[10];

void *func (void *arg);
void registe(int sockfd,int *index,struct protocol*msg);
void login  (int sockfd,int *index,struct protocol*msg);
void logout (int sockfd,int *index,struct protocol*msg);
void broadcast (int sockfd,int *index,struct protocol*msg);
int find_user_online(int sockfd,int *index,struct protocol*msg);

int main (int argc ,char **argv)
{
  int portnumber;
  int lsfd;
  int newfd;
  int addrlen;
  int cliaddrlen;
  int *arg = NULL;
  int i;

  struct sockaddr_in   my_addr; 
  struct sockaddr_in   cli_addr;

  pthread_t pid;
  char buff[520];

  portnumber=atoi(argv[1]);
 
  lsfd = socket(AF_INET,SOCK_STREAM,0);
  
  bzero(&my_addr,sizeof(struct sockaddr_in));

  my_addr.sin_family =  AF_INET;
  my_addr.sin_port   =  htons(portnumber);
  my_addr.sin_addr.s_addr   =  htonl(INADDR_ANY);
  
  addrlen = sizeof(struct sockaddr_in);
  cliaddrlen = sizeof(struct sockaddr_in);

  bind(lsfd,(struct sockaddr* )&my_addr ,addrlen);

  listen(lsfd,5);

  for(i=0;i<10;i++)  
  {
   online[i].fd = -1;
   online[i].flage= -1;
  }

  while(1)
  {
   newfd = accept(lsfd,(struct sockaddr *)&cli_addr,&cliaddrlen);
   printf("client:ip:%s   port:%d  \n",
                 inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);

    arg = malloc(sizeof(int));
    *arg = newfd;

   pthread_create(&pid,NULL,func, (void*)arg);	
  }
   close(newfd);
   close(lsfd);
   exit(0);

}


void *func (void *arg)
{
 int sockfd = *((int *)arg);
 int len;
 int index = -1;

 struct protocol msg;

 free(arg);

 while(1)
  {
  len = read(sockfd,&msg,sizeof(msg));
 
  if(len<=0)
	{//下线
	printf("%s offline\n",online[index].name);
	//从在线列表中删除
	//del_user_online(index);
	close(sockfd);
	return;
	}

  switch(msg.cmd)
  	{
	 case REGISTE:
	   registe(sockfd,&index,&msg);
	   break;
	 case LOGIN:
	   login(sockfd,&index,&msg);
	   break;
	 case LOGOUT:
	   logout(sockfd,&index,&msg);
	   break;
	 case BROADCAST:
            broadcast(sockfd,&index,&msg);
            break;
	 default:
	   break;
	} 
   }

}

void registe(int sockfd,int *index,struct protocol*msg)
{
  struct protocol msg_back;

  msg_back.cmd = REGISTE;

  *index = add_user(sockfd,msg);

  if(*index == -1)
  {
   msg_back.state = NAME_EXIST;
   printf("user %s exist!\n",msg->name);
   write(sockfd,&msg_back,sizeof(msg_back));
   return;  
  }
  else{
  online[*index].flage = 1;
  msg_back.state = OP_OK;
		
  printf("user %s regist success!\n",msg->name);
  write(sockfd,&msg_back,sizeof(msg_back));
		
  return;
  }
}


int add_user(int sockfd,struct protocol*msg)
{
	int i,index = -1;
	char buf[128]={0};
	
	for(i=0;i<10;i++)
	{
		if(online[i].flage == -1)
		{
			continue;			
		}
		
		if(strcmp(msg->name,online[i].name)==0)
		{
			return -1;			
		}
	}

	for(i=0;i<10;i++)//添加到在线用户列表
	{
		if(online[i].flage == -1)
		{
			online[i].flage= 1;
			strcpy(online[i].name,msg->name);
			strcpy(online[i].passwd,msg->data);
			printf("regist %s to %d \n",msg->name,i);
			index = i;
			return index;
		}		
	}
}

void login  (int sockfd,int *index,struct protocol*msg)
{
	int ret;
	char buf[520];
	struct protocol msg_back;

	msg_back.cmd = LOGIN;
	ret = find_user_online(sockfd,index,msg);

	if(ret != OP_OK)
	{
		msg_back.state = ret;
		strcpy(buf,"there is no this user\n");
		printf("user %s login fail!\n",msg->name);
		
		write(sockfd,&msg_back,sizeof(msg_back));
		return;
	}
	else
	{
		msg_back.state = OP_OK;
		strcpy(msg_back.data,"login success\n");
		printf("user %s login success!index =%d \n",msg->name,*index);
		write(online[*index].fd,&msg_back,sizeof(msg_back));
	}

}

int find_user_online(int sockfd,int *index,struct protocol*msg)
{
	int i;

	for(i=0;i<10;i++)
	{
	 
		if(online[i].flage== -1)
		{
		continue;			
		}
	
		if((strcmp(msg->name,online[i].name)==0)&&(strcmp(msg->data,online[i].passwd)==0))
		{
		if(online[i].fd == -1)
		{
				online[i].fd = sockfd;
				*index = i ;
				return OP_OK;
		}
		else
		{
				//user had loged
				printf("%s had login\n",online[i].name);
				return USER_LOGED;
		}
		}
	}
	
	return NAME_PWD_NMATCH;
}


void logout (int sockfd,int *index,struct protocol*msg)
{
 struct protocol msg_back;

  msg_back.cmd = LOGOUT;
  msg_back.state = ONLINEUSER_OVER;  
  printf("user %s logout!\n",online[*index].name);

  write(online[*index].fd,&msg_back,sizeof(msg_back));
  close(sockfd);
  online[*index].fd = -1;
}


 void broadcast (int sockfd,int *index,struct protocol*msg)
 {
         int i;
         char buf[520]={0};

         printf("from %s client: %s\n",online[*index].name,msg->data);
	 sprintf(buf,"%s say:%s\n",online[*index].name ,msg->data);

         for(i=0;i<10;i++)
         {
                 if((online[i].fd == -1)||(i == *index))
                 {
                         continue;
                 }
                 write(online[i].fd,buf,strlen(buf));
         }

 }

