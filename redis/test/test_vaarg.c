#include<stdio.h>
#include<stdarg.h>

int FindMax(int n,...){
	int i,val,largest;
	va_list vl;
	va_start(vl,n);
	largest = va_arg(vl,int);
	for(i=1;i<n;i++){
		val=va_arg(vl,int);
		largest = (largest>val)?largest:val;
	}
	va_end(vl);
	return largest;
}

int main(){
	int m;
	m=FindMax(7,702,422,631,834,892,104,772);
	printf("The largest value is: %d\n",m);
	return 0;
}
