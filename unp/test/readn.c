#include "unp.h"
ssize_t	 readn(int fd,void *vptr,size_t n){
	ssize_t nleft;
	ssize_t nread;
	char *ptr;
	ptr = vptr;
	nleft = n;
	while(nleft > 0){
		if((nread=read(fd,ptr,nleft))<0){
			if(errno == EINTR) nread = 0;
			else return -1;
		} else if(nread == 0) break;
		nleft -= nread;
		ptr += nread;
	}
	return (n-nleft);
}

ssize_t writen(int fd,const void *vptr,size_r n){
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = vptr;
	nleft = n;
	while(nleft > 0){
		if((nwritten = write(fd,ptr,nleft)) <= 0) {
			if(errno == EINTR) nwritten = 0;
			else return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

ssize_t readline(int fd,void *vptr,size_t maxlen){
	ssize_t n,rc;
	char c,*ptr;
	ptr = vptr;
	for(n=1;n<maxlen;n++){
		again:
		if((rc = read(fd,&c,1)) == 1){
			*ptr++ = c;
			if(c == '\n')
				break;
			else if(rc == 0) {
				if(n == 1) return 0;
				else break;
			}
		} else {
			if(errno == EINTR) goto again;
			return -1;
		}
	}
	*ptr = 0;
	return n;
}

static ssize_t my_read(int fd,char *ptr){
	static int read_cnt = 0;
	static char *read_ptr;
	static char read_buf[MAXLINE];
	if(read_cnt <= 0) {
		again:
      		if((read_cnt = read(fd,read_buf,sizeof(read_buf)) < 0) {
			if(errno == EINTR) goto again;
			return -1;
		} else if(read_cnt == 0) return 0;
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;
	return 1;
}
