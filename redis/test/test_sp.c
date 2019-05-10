#include<stdio.h>
#define STRING(x) #x
#define CONNECT(a,b) a##b

int main(){
	printf("%s\n",STRING(HELLO WORLD!));
	int CONNECT(a,1);
	a1 = 2;
	printf("%d\n",a1);
	return 0;
}
