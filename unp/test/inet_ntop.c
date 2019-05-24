#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<strings.h>

int main(int argc,char **argv){
	char dst[100];
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in serv;
	memset(&serv,0,sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(5555);
	if((inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr))==0) printf("inet_pton\n");
	if((inet_ntop(AF_INET,&serv.sin_addr.s_addr,dst,sizeof(dst)))==NULL) printf("inet_ntop\n");
	printf("dst=%s,sizeof(dst)=%d\n",dst,sizeof(dst));
	
	bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));
	listen(sockfd,15);
	return 0;
}
