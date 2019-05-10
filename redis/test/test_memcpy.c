#include <stdio.h>
#include <string.h>

int main(){
	const char src[50] = "http://www.runoob.com";
	char dest[50];
	
	memcpy(dest,src,strlen(src));
	printf("dest = %s\n",dest);
	
	return 0;
}
