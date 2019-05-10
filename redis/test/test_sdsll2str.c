#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sdsll2str(char *s,long long value) {
	char *p,aux;
	unsigned long long v;
	size_t l;
	
	v = (value < 0) ? -value : value;
	p = s;
	do {
		*p++ = '0'+(v%10);
		v /= 10;
	} while(v);
	if(value < 0) *p++ = '-';

	l = p-s;
	*p = '\0';

	p--;
	while(s < p) {
		aux = *s;
		*s = *p;
		*p = aux;
		s++;
		p--;
	}
	return l;
}

int main(void){
	char *s;
	long long value = -12335;
	
	int l = sdsll2str(s,value);
	puts(s);
	printf("l=%d\n",l);
	return 0;
}
