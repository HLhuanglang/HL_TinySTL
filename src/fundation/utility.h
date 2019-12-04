/*****************************************************
���ƣ�utility.h
���ã��ṩһЩͨ�õĹ��ߣ���swap��move��forward�Ⱥ�����pair
*****************************************************/
#ifndef UTILITY_H
#define UTILITY_H
namespace TinySTL {

	template<typename T>
	void swap(T&a, T&b) {
		T temp = a;
		a = b;
		b = temp;
	}

}//namespace TinySTL
#endif // !UTILITY_H

