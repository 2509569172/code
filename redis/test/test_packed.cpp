#include <stdio.h>
#include <iostream>

using namespace std;

struct test1 {
	char c;
	int i;
};

struct __attribute__ ((__packed__)) test2 {
	char c;
	int i;
};

struct __attribute__ ((__packed__)) test3 {
	char c;
	int i;
	char buf[];
};

int main(){
	cout << "sizeof test1:" << sizeof(struct test1) << endl;
	cout << "sizeof test2:" << sizeof(struct test2) << endl;
	cout << "sizeof test3:" << sizeof(struct test3) << endl;
	return 0;
}
