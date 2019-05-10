#include <stdio.h>
#include <string.h>

int main(){
	char szInput[256];
	printf("Enter a sentence: ");
	fgets(szInput,256,stdin);
	printf("The sentences entered is %u characters long.\n",(unsigned)strlen(szInput));
	return 0;
}
