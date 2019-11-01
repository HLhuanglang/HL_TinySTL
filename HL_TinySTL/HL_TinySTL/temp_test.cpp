#include<iostream>

#include"allocator.h"
#include"uninitialized.h"
using std::ostream;
using std::cout;
using std::endl;

class test {
public:
	test() : a(1), b('a'){}
	test(int a) : a(a), b('A') {}
	friend ostream &operator<<(ostream &out, test& item);
private:
	int a;
	char b;
};
ostream &operator<<(ostream &out, test& item) {
	out << item.a << " " << item.b;
	return out;
}

int main() {

	for (int i = 1; i <= 20; ++i) {
		auto p = allocator<test>::allocate(5); //申请5个元素大小的空间
		auto q = allocator<test>::allocate(5);
		TinySTL::uninitialized_fill(p, p+5, test(i)); //申请了多大，就只能对这个范围进行填充，p- p+6还是填充5个
		TinySTL::uninitialized_copy(p, p + 5, q);
		for(int j=0; j<5; ++j)
			cout << *(p+j) <<"  "<<*(q+j)<< endl;
		cout << "------"<<endl;
	}
	cout << "****************************************" << endl;
	int array[20];
	TinySTL::uninitialized_fill_n(array, 20, 22); //对array填充20个22
	for (auto n : array)
	{
		cout << n << endl;
	}

	system("pause");
	return 0;
}