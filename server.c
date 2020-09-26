#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>//sockaddr结构
#include<arpa/inet.h>
#include<unistd.h> /*包含头文件。*/
#define PORT 5678/*端口*/
#define MAX 10/*最多的连接数。*/
main()
{
	int sockfd,newsockfd,is_connected[MAX],fd;
	/*定义相关的变量。*/
	struct sockaddr_in addr;//bind,accept用到
	int addr_len = sizeof(struct sockaddr_in);//sockaddr_in的大小
	fd_set myreadfds;
	char msgbuffer[256];
	char msg[] ="This is the message from server.Connected.\n";
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0))<0) /*建立一个 socket。*/
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("socket created .\n");
		/*socket 建立成功,输出提示。*/
		printf("socked id: %d \n",sockfd);
	}
	bzero(&addr,sizeof(addr));
	addr.sin_family =AF_INET;//
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /*清空 addr 所在的内存。*/
	if(bind(sockfd,&addr,sizeof(addr))<0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("connected.\n");
		printf("local port:%d\n",PORT)
			;
	} /*绑定端口。*/
	if(listen(sockfd,1)<0)
	{
		perror("listen");
		exit(1);
	}
	else
	{
		printf("listenning......\n\n");
	} /*开始监听。*/

	for(fd=0;fd<MAX;fd++)
	{
		is_connected[fd]=0;
	}

    int xun=0;

	while(1)
	{
	    xun++;
	    printf("###循环次数=%d\n",xun);
		FD_ZERO(&myreadfds);
		printf("sockfd=%d\n",sockfd);
		FD_SET(sockfd,&myreadfds);
		for(fd=0;fd<MAX;fd++)
		{
			if(is_connected[fd])
			{
				FD_SET(fd,&myreadfds);
				printf("fd添加myreadfds,fd=%d\n",fd);
			}
		}
		
		if(!select(MAX,&myreadfds,NULL,NULL,NULL))
			/*如果到达了最大的连接数则进
			 * 入下次循环。*/
		{
		    printf("跳过本次循环\n");
			continue;//跳过本次循环
		}
		
		for(fd=0;fd<MAX;fd++)
		{
		    printf("+%d,%d+",fd,is_connected[fd]);
		    if(fd==9)printf("\n");
		}
		
		int current;
		
		for(fd=0;fd<MAX;fd++)
			/*进入一个循环。*/
		{
		    printf("for循环,fd=%d.\n",fd);
			if(FD_ISSET(fd,&myreadfds))
			{
				if(sockfd==fd)
					/*如果新建的 socket 与 fd 相同。*/
				{
					if((newsockfd = accept (sockfd,&addr,&addr_len))<0)
						/*接受一个连接,新建一个 socket。*/
					{
						perror("accept");
					}
					write(newsockfd,msg,sizeof(msg));
					/*给客户端发送一段信息。*/
					is_connected[newsockfd] =1;
					/*设置标志。*/
					printf("cnnect from %s\n",inet_ntoa(addr.sin_addr));
					/*输出客户端的 IP。*/
				}
				else
				{
					printf("sockfd!=fd\n");
					bzero(msgbuffer,sizeof(msgbuffer));
					/*清空字符串。*/
					if(read(fd,msgbuffer,sizeof(msgbuffer))<=0)//重要
						/*读取结果。*/
					{
						printf("connect closed.\n");
						/*输出连接关闭。*/
						is_connected[fd]=0;
						/*设置标志。*/
						close(fd);
						/*关闭一个 socket。*/
					}
					else
					{
					    current=fd;
					    for(fd=0;fd<MAX;fd++)
					    {
					    if(is_connected[fd]==1) 
					    {
					    if(fd!=current){write(fd,msgbuffer,sizeof(msgbuffer));
					    printf("写给%d\n",fd);}
					    }
					    }
						printf("message:%s\n",msgbuffer);
						/*输出接收到的信息。*/
					}
				}
		    }
	    }
	printf("###结束本次循环\n\n\n");
    }
}
