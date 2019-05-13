#include "unp.h"
int main(int argc,char **argv){
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in6 servaddr;

	if(argc != 2){
		err_quit("usage:a.out<IP address>");
	}
	
	if((sockfd = socket(AF_INET6,SOCK_STREAM,0))<0)
		err_sys("socket error");
}
