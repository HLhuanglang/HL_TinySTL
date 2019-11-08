/*********************************************************************
名称：algobase.h
功能：包含了STL中的基本算法
*********************************************************************/
#ifndef ALGOBASE_H
#define ALGOBASE_H
namespace TinySTL{

	//**********[max]**********
	//**********[Algorithm Complexity: O(1)]**********
	template<class T>
	const T&max(const T&a, const T&b) {
		return a < b ? b : a;
	}
	template<class T, class Compare>
	const T& max(const T&a, const T&b, Compare comp) {
		return (comp(a, b)) ? b : a;
	}
} //namespace TinySTL
#endif // !ALGOBASE_H

