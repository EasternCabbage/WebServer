  #include <stdio.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <stdlib.h>
  #include <netinet/in.h>
  #include <errno.h>
  #include <string.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define MAXLINE 1024
int main(int argc,char **argv)
{
	char *servInetAddr = "127.0.0.1";//127.0.0.1是具有特殊意义的ip地址，代表本地回环，可以用于测试
  	int socketfd;//创建socket文件描述符
  	struct sockaddr_in sockaddr;//创建socket结构体，准备存入相关信息
  	char recvline[MAXLINE], sendline[MAXLINE];//用来存收到的数据，和即将发送出去的数据
  	int n;
 
  	socketfd = socket(AF_INET,SOCK_STREAM,0);//创建IPV4(AF_INET)的TCP(SOCK_STRAM)的socket
  	memset(&sockaddr,0,sizeof(sockaddr));//重置结构体内容
  	sockaddr.sin_family = AF_INET;//作用范围是IPV4的网络
  	sockaddr.sin_port = htons(10004);//将10004转化为网络字节序
  	inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);//这个API可以将其中的参数转化为网络字节序，并存入socket结构体中。实际上这里也可以用sockaddr_in.in_addr.sin_addr=htonl(127.0.0.1)来存IP的信息
  
  	if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )//开始连接信息匹配的socket
  	{
  		printf("connect error %s errno: %d\n",strerror(errno),errno);
  		exit(0);
  	}

	for(;;){  
		printf("send message to server\n");
 
		fgets(sendline,1024,stdin);//从终端获取用户输入数据
 
  		if((send(socketfd,sendline,strlen(sendline),0)) < 0)//发送给服务端
  		{
  			printf("send mes error: %s errno : %d",strerror(errno),errno);
  			exit(0);
  		}
  		memset(sendline,0,sizeof(sendline));//将sendline内容清空
  		int length = recv(socketfd,recvline,MAXLINE,0);//开始进入接收阻塞状态，直到客户端返回信息，这里接收完毕
  		recvline[length+1] = '\0';
  		printf("%s:receive messag from server:%s\n",__FILE__,recvline);//输出信息到终端
  		memset(recvline,0,sizeof(recvline));//将recvline内容清空
	}
  	close(socketfd);//关闭socket，服务端会收到客户端关闭的信息，也关闭自己的socket。
  	printf("exit\n");
  	exit(0);
}
