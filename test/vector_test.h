#ifndef  VECTOR_TEST_H
#define VECTOR_TEST_H

#include"vector.h"

#include<iostream>
#include<vector>
#include<ctime>
using std::ostream;
using std::cout;
using std::endl;

class test {
public:
	test() : a(1), b('a') {}
	test(int a, char b) : a(a), b(b) {}
	friend ostream &operator<<(ostream &out, test& item);
private:
	int a;
	char b;
};
ostream &operator<<(ostream &out, test& item) {
	out << item.a << " " << item.b;
	return out;
}

template<typename T>
void display(TinySTL::vector<T>& v) {
	for (auto n : v) {
		cout << n << endl;
	}
	cout << "--------------" << endl;
}

template<typename T>
void big_data_test(TinySTL::vector<T>&v, int size) {
	double start_time_tinystl = clock();
	TinySTL::vector<T> v_tinystl;
	for (int i = 0; i < 10000; i++) {
		v_tinystl.push_back(T(i));  //T(i)这种写法只是为了区分当T是自定义类型时
	}
	double end_time_tinystl = clock();
	cout << "TinySTL:push_back " << " " << end_time_tinystl - start_time_tinystl << endl;

	double start_time_std = clock();
	std::vector<T> v_std;
	for (int i = 0; i < size; i++) {
		v_std.push_back(T(i));
	}
	double end_time_std = clock();
	cout << "std:push_back " << " " << end_time_std - start_time_std << endl;
}

void vector_test() {
	//POD型
		/***********************构造、copy等函数测试*******************************/
	TinySTL::vector<int> v1;
	TinySTL::vector<int> v2(10);
	TinySTL::vector<int> v3(10, 100);
	TinySTL::vector<int>v4(v3);
	int a[] = { 1,2,3,4,5 };
	TinySTL::vector<int> v5(a, a + 5);
	//TinySTL::vector<int> v6 = v5;  v6直接指向了v5..
	//TinySTL::vector<int> v7{ 1,2,3,4,5,6,7 }; 缺少这种构造函数

	cout << "***************************元素访问相关函数测试*****************************" << endl;
	cout << "v5容器元素为：" << endl;
	display(v5);
	cout << "[ ]运算符访问" << " " << v5[4] << endl;
	cout << "front()函数：" << " " << v5.front() << endl;
	cout << "back()函数：" << " " << v5.back() << endl;

	cout << "**************************容量相关函数测试*********************************" << endl;
	cout << "v4容器元素为：" << endl;
	display(v4);
	cout << "v4.size()：" << " " << v4.size() << endl;
	cout << "v4.capacity()：" << " " << v4.capacity() << endl;

	cout << "-------------reserve函数--------------------" << endl;
	v4.reserve(9);
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	cout << "---------" << endl;
	v4.reserve(11);
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	cout << "----------------------------------------------" << endl;


	cout << "-------------------resize函数----------------" << endl;
	v4.resize(10);
	cout << "resize(10)" << endl;
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);

	v4.resize(11);
	cout << "resize(11)" << endl;
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	cout << "------------------------------------------------" << endl;


	cout << "shrink_to_fit函数：" << endl;
	v4.shrink_to_fit();
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	/******************************容器修改相关操作********************************/

	big_data_test(v3, 10000);  //push_back测试

}



#endif // ! VECTOR_TEST_H
