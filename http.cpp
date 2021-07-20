
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "http.h"

void Http::HttpInit(){

    //用作储存sock相关信息的结构体
	int n;
 	char *copy = "copy that";
 	memset(&sockaddr,0,sizeof(sockaddr));//清空sockaddr的内容

	sockaddr.sin_family = AF_INET;//AF_INET代表socket范围是IPV4的网络
 	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY代表不管数据包目标地址是哪里都接收
 	sockaddr.sin_port = htons();//htons代表将本地字节序(port信息)转化成网络字节序，

 	sfd = socket(AF_INET,SOCK_STREAM,0);//创建IPV4(AF_INET)的TCP(SOCK_STREAM)的socket

	bind(sfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr));//将socket与sockaddr绑定

 	listen(sfd,1024);//开始监听，最长字符串长度1024

	std::cout<<"Please wait for the client information\n"<<std::end;
 	if((cfd = accept(sfd,(struct sockaddr*)NULL,NULL))==-1)//进入接收阻塞状态，直到收到信息
 	{
 		printf("accpet socket error: %s errno :%d\n",strerror(errno),errno);
		exit(0);
 	}
}

HttpPraseState Http::HttpPraseStatu(){
    int length = recv(cfd,recv_content,1024,0);
    recv_content.replace(0,lengt+1,'\0')
    std::cout << "msg from client:" << recv_content << "\n"<<std::endl;
    string::iterator iter = s1.begin();
    for( ; iter < s1.end() ; iter++)
    {
        if(strcmp("\n",*iter));
        
    }
    std::string method = recv_content.
}

void Http::HttpLoop(){

}