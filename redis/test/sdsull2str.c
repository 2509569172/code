#include<stdlib.h>
#include<stdio.h>

int sdsull2str(char *s,unsigned long long v){
	char *p,aux;
	size_t l;

	p = s;
	do{
		*p++ = '0'+(v%10);
		v /= 10;
	}while(v);

	l = p-s;
	*p = '\0';

	p--;
	while(s < p){
		aux = *s;
		*s = *p;
		*p = aux;
		s++;
		p--;
	}
	return l;
}

int main(void){
	char* p;
	unsigned long long v = 123456;

	sdsull2str(p,v);
	printf("%s\n",p);
	return 0;
}
