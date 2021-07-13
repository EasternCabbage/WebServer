#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
	std::cout << "socket client" << std::endl;
	int cfd = socket(AF_INET,SOCK_STREAM,0);
	if(cfd == -1){
		std::cout << "error : socket" << std::endl;
		return 0;
	}
	struct sockaddr_in  addr;
        addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(cfd,(struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1){
		std::cout << "Error : connet" << std::endl;
		return 0;
	}
	else{
		std::cout << "connect sucessful"<<std::endl;
	}
	for(;;){
		char str[1024];
		std::cout << "请输入信息\n";
		std::cin >> str;
		if(strcmp(str,"exit") == 0){
			std::cout << "...disconneting" << std::endl;
			break;
		}else{
			if( send(cfd,str,strlen(str),0) == -1){
				std::cout << "error send" << std::endl;
				break;
			}
		}
		memset(str,0,sizeof(str));
	}
	
	close(cfd);
	return 0;	




		
}
