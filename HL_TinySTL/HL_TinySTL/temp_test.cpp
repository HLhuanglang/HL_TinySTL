/*
���⣺

*/
#include"allocator.h"
#include"uninitialized.h"
#include"vector.h"

#include<iostream>
#include<vector>
#include<ctime>
using std::ostream;
using std::cout;
using std::endl;

class test {
public:
	test() : a(1), b('a'){}
	test(int a,char b) : a(a), b(b) {}
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
		v_tinystl.push_back(T(i));  //T(i)����д��ֻ��Ϊ�����ֵ�T���Զ�������ʱ
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


int main() {
	//POD��
	/***********************���졢copy�Ⱥ�������*******************************/
	TinySTL::vector<int> v1;
	TinySTL::vector<int> v2(10);
	TinySTL::vector<int> v3(10, 100);
	TinySTL::vector<int>v4(v3);
	int a[] = { 1,2,3,4,5 };
	TinySTL::vector<int> v5(a, a + 5);
	//TinySTL::vector<int> v6 = v5;  v6ֱ��ָ����v5..
	//TinySTL::vector<int> v7{ 1,2,3,4,5,6,7 }; ȱ�����ֹ��캯��
	
	cout << "***************************Ԫ�ط�����غ�������*****************************" << endl;
	cout << "v5����Ԫ��Ϊ��" << endl;
	display(v5);
	cout << "[ ]���������" << " "<< v5[4] << endl;
	cout << "front()������" << " "<< v5.front() << endl;
	cout << "back()������" << " "<<  v5.back() << endl;

	cout << "**************************������غ�������*********************************" << endl;
	cout << "v4����Ԫ��Ϊ��" << endl;
	display(v4);
	cout << "v4.size()��" << " "<<  v4.size() << endl;
	cout << "v4.capacity()��" << " "<< v4.capacity() << endl;

	cout << "-------------reserve����--------------------" << endl;
	v4.reserve(9);
	cout <<"size"<< v4.size() << endl;
	cout << "capacity"<<v4.capacity() << endl;
	display(v4);
	cout << "---------" << endl;
	v4.reserve(11);
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	cout << "----------------------------------------------" << endl;


	cout << "-------------------resize����----------------" << endl;
	v4.resize(10);
	cout << "resize(10)" << endl;
	cout <<"size"<< v4.size() << endl;
	cout <<"capacity"<<v4.capacity() << endl;
	display(v4);

	v4.resize(11);
	cout << "resize(11)" << endl;
	cout << "size" << v4.size() << endl;
	cout << "capacity" << v4.capacity() << endl;
	display(v4);
	cout << "------------------------------------------------" << endl;


	cout << "shrink_to_fit������" << endl;
	v4.shrink_to_fit();
	cout <<"size"<< v4.size() << endl;
	cout <<"capacity"<< v4.capacity() << endl;
	display(v4);
	/******************************�����޸���ز���********************************/

	big_data_test(v3, 10000);  //push_back����


	system("pause");
	return 0;
}