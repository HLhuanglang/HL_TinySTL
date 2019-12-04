/*****************************************************
名称：utility.h
作用：提供一些通用的工具，如swap、move、forward等函数，pair
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

