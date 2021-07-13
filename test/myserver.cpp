#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 100

using namespace std;

int main(int argc,char *argv[]){
	if(argc != 2){
		std::cout << "usage:server [port]\n\
			      example:./server 8000"<<std::endl; 
		exit(0);
	}
	char buf[BUF_SIZE];
	int numRead = 0;
	cout << "socket server start..." << endl;
	// init socket
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1){
		cout << "error :socket" << endl;
		return 0;
	}
	//bind
	int yes=1;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));//port
	addr.sin_addr.s_addr = INADDR_ANY;//means 0.0.0.0
	if(setsockopt(sfd, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) == -1){
		perror("setsockopt");
		exit(1);
	}
	if( bind(sfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) == -1){
		cout << "Error: bind" << endl;
		return 0;
	}
	
	//listen
	if(listen(sfd, 5) == -1){
		cout << "error:listen"<<endl;
		return 0;
	}
	
	
	while(true){
		int cfd = accept(sfd,NULL,NULL);
		if(cfd == -1){
			cout << "error:accept"<<endl;
			return 0;
		}
		numRead = recv(cfd,buf,BUF_SIZE,0);
		buf[numRead] = '\0';
		if(numRead > 0){
			if(strcmp(buf,"exit") == 0 ){
				cout << "exit sucessful" << endl;
				break;
			}
			cout << "receive message:"<< buf << endl;
			
		}
		else if(numRead == -1){
			cout << "error:read"<<endl;
			return 0;
		}
					
		close(cfd);
			
	}
	if(close(sfd) == -1){
		cout << "error:close"<<endl;
		return 0;
	}
	return 0;
	
}
