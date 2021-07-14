#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024
int main(int argc,char **argv){
	int listenfd,connfd;//这两个稍后用作socket文件描述符
	struct sockaddr_in sockaddr;//用作储存sock相关信息的结构体
	char buff[MAXLINE];
	int n;
 	char *copy = "copy that";
 	memset(&sockaddr,0,sizeof(sockaddr));//清空sockaddr的内容

	sockaddr.sin_family = AF_INET;//AF_INET代表socket范围是IPV4的网络
 	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY代表不管数据包目标地址是哪里都接收
 	sockaddr.sin_port = htons(10004);//htons代表将本地字节序(port信息)转化成网络字节序，

 	listenfd = socket(AF_INET,SOCK_STREAM,0);//创建IPV4(AF_INET)的TCP(SOCK_STREAM)的socket

	bind(listenfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr));//将socket与sockaddr绑定

 	listen(listenfd,1024);//开始监听，最长字符串长度1024

	printf("Please wait for the client information\n");
 	if((connfd = accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)//进入接收阻塞状态，直到收到信息
 	{
 		printf("accpet socket error: %s errno :%d\n",strerror(errno),errno);
		exit(0);
 	}

	for(;;){
		 n = recv(connfd,buff,MAXLINE,0);//recvAPI可以获取规定字符长度sockfd的内容到指定buffer
		 buff[n+1] = '\0';//添加文件结束符
		 printf("recv msg from client:%s\n",buff);
		 send(connfd,copy,strlen(copy),0);//将收到的消息发送给客户端，提示这边已经接收完毕
		 memset(buff,0,sizeof(buff));//清空buff内容	 
	}
  	close(connfd);//关闭accept的文件描述符
   	close(listenfd);//关闭socketfd的文件描述符
	return 0;
 }
