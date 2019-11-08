/*********************************************************************
���ƣ�algobase.h
���ܣ�������STL�еĻ����㷨
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

