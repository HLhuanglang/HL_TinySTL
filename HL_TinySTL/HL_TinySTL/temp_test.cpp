#include<iostream>

#include"allocator.h"
class test {
private:
	int a;
	char b;
	test* p;
};

int main() {
	for (int i = 1; i != 100000; ++i) {
		auto p = allocator<test>::allocate();
		allocator<test>::construct(p, test());
		allocator<test>::destory(p);
		allocator<test>::deallocate(p);
	}
	system("pause");
	return 0;
}