
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "http.h"
void Http::HttpInit(){

    //用作储存sock相关信息的结构体
    int n;
    struct sockaddr_in sockaddr;
    std::string string = "copy that";
    memset(&sockaddr,0,sizeof(sockaddr));//清空sockaddr的内容

    sockaddr.sin_family = AF_INET;//AF_INET代表socket范围是IPV4的网络
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY代表不管数据包目标地址是哪里都接收
    sockaddr.sin_port = htons(8080);//htons代表将本地字节序(port信息)转化成网络字节序，

    sfd = socket(AF_INET,SOCK_STREAM,0);//创建IPV4(AF_INET)的TCP(SOCK_STREAM)的socket

    bind(sfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr));//将socket与sockaddr绑定

    listen(sfd,1024);//开始监听，最长字符串长度1024

    std::cout<<"Please wait for the client information\n"<<std::endl;
}

void Http::HttpFirstLine(){
    int line_end;
    line_end = recv_content.find("\n");
    first_line = recv_content.substr(0, line_end);
   
}
void Http::HttpPraseMethod(){
    int line_end;
    std::string result;
    line_end = first_line.find(" ");
    result = first_line.substr(0,line_end);
    if(result.compare("POST") == 0){
        method = POST;
    }
    else if(result.compare("GET") == 0){
        method = GET;
    }
}
void Http::HttpPraseUrl(){
    switch(method){
        case GET:
        {
                int begin = 0 ,end = 0;
                begin = first_line.find(" ");
                end = first_line.find(" ",begin + 1);
                http_url = first_line.substr(begin+1 , end-begin-1);
                
                std::size_t found = http_url.find("?");
                if(found != std::string::npos)
                    http_url.pop_back();
        
                break;
        }
        case POST:
        {
            int begin = 0,end = 0;
                begin = first_line.find(" ");
                end = first_line.find(" ",end + 1);
                http_url = first_line.substr(begin + 1, end-begin-1);
                break;
        }
        default:
        {
            http_url.erase();
            break;
        }
            
    }
    
}

void Http::HttpPraseStatu(){
    int seek =0;
    seek = first_line.find(" ");
    seek = first_line.find(" ",seek + 1);
    http_url = first_line.substr(seek + 1);
    
}

void Http::do_request(){
        std::string response="HTTP/1.1 200 ok\r\n/connection: close\r\n\r\n";
    if(http_url.compare("/") == 0){
        
        int s = send(cfd,response.c_str(),response.length(),0);//发送响应
        std::cout << "sending index.html" <<std::endl;
        int fd = open("index.html",O_RDONLY);//消息体
        sendfile(cfd,fd,NULL,2500);//零拷贝发送消息体
        close(fd);
    }else{
        std::string::size_type idx = http_url.find("png");
        if(idx != std::string::npos)
            response.append("Content-Type: image/png\r\n");

        int s = send(cfd,response.c_str(),response.length(),0);//发送响应
        std::cout << "sending "<< http_url<<std::endl;
        http_url.insert(0,".");
        std::cout << "http_url.c_str():"<<http_url.c_str() <<std::endl;
        int fd = open(http_url.c_str(),O_RDONLY);//消息体
        if(fd == -1)
            std::cout <<"open file error:"<<__func__<<std::endl;
        
        sendfile(cfd,fd,NULL,2500);//零拷贝发送消息体
        close(fd);
    }
    
    
}


void Http::HttpLoop(){
    while(1){
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);
        std::cout<<"one step before accept\n"<<std::endl;
        if((cfd = accept(sfd,(struct sockaddr*)&client,&client_addrlength))==-1)//进入接收阻塞状态，直到收到信息
        {
            std::cout<<"accpet socket error:"<<strerror(errno)<<errno<<std::endl;
            exit(0);
        }

        if(cfd < 0){
            std::cout << "error\n" << std::endl;
        }
        else{
            char buf[1024];
            recv(cfd,buf,1024,0);
            buf[strlen(buf)+1]='\0';
            recv_content = buf;
            HttpFirstLine();
            std::cout << "first line:"<<first_line<<std::endl;
            HttpPraseMethod();
            std::cout << "method:"<<method<<std::endl;

            HttpPraseUrl();
            std::cout << "http_url:"<<http_url<<std::endl;
            std::cout << "http_url_length:" <<http_url.length()<<std::endl;
            //HttpPraseStatu();
            do_request();
            //std::cout << buf << "\nsuccussful\n"<<std::endl;
            memset(buf,0,sizeof(buf));
            
            close(cfd);
        }
    }

}