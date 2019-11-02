/*
问题：
 1、TinySTL::vector<test>A1_3;	      !!!0字节问题，需要处理!!!
*/
#include<iostream>

#include"allocator.h"
#include"uninitialized.h"
#include"vector.h"

#include<vector>

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
	cout << "********************************************" << endl;
	cout << "自定义类型" << endl;
	TinySTL::vector<test> A1_1(10, test(1));
	TinySTL::vector<test>A1_2(10);
	for (auto n : A1_1)
	{
		cout << n << endl;
	}
	cout << "---------" << endl;
	for (auto n : A1_2)
	{
		cout << n << endl;
	}

	cout << "********************************************" << endl;
	cout << "POD型" << endl;
	TinySTL::vector<char> A2_1(10, 'a');
	TinySTL::vector<int> A2_2(10, int(22)); //这样A2_2占据了内存，但是没内容
	for (auto n : A2_1)
	{
		cout << n << endl;
	}
	cout << "---------" << endl;
	for (auto n : A2_2) 
	{
		cout << n << endl;
	}

	int array[3] = {1,2,3 };
	TinySTL::vector<int> vec(array, array + 3);
	for (auto a : vec)
	{
		cout << a << endl;
	}

	test array_test[3] = { test(2), test(3), test(4) };
	TinySTL::vector<test> vr(array_test, array_test + 3);
	for (auto n : vr)
	{
		cout << n << endl;
	}
	system("pause");
	return 0;
}