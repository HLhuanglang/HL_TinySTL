#include<iostream>

#include"alloc.h"

int main() {
	int count = 0;
	for (int i = 1; i <= 100; ++i)
	{
		for (int j = 8; j <= 136; j+= 8)//0-128 、136保证一二级配置都执行 
		{
			void *p_allocate = TinySTL::alloc::allocate(j);
			void *q_reallocate = TinySTL::alloc::reallocate(p_allocate, j, j + 8);
			if (p_allocate != nullptr)
				count++;
		}
			std::cout << i << ":" << count << std::endl;
			count = 0;
	}
	system("pause");
	return 0;
}