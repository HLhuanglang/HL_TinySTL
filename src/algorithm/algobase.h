/*********************************************************************
名称：algobase.h
功能：包含了STL中的基本算法
*********************************************************************/
#ifndef ALGOBASE_H
#define ALGOBASE_H

#include"../fundation/utility.h"


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

	//**********[min]**********
	//**********[Algorithm Complexity: O(1)]**********
	template<class T>
	const T&min(const T&a, const T&b) {
		return a < b ? a : b;
	}
	template<class T, class Compare>
	const T& min(const T&a, const T&b, Compare comp) {
		return !comp(a, b) ? a : b;
	}

	//********* [iter_swap] ********************
	//********* [Algorithm Complexity: O(1)] ****************
	template<class Iter1, class Iter2>
	void iter_swap(Iter1 _Left, Iter2 _Right) {
		//交换两个迭代器所指对象
		TinySTL::swap(*_Left, *_Right);
	}

	//**********[fill]**********
	//**********[Algorithm Complexity: O(N)]**********
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T&val) {
		for (; first != last; ++first)
			*first = val;
	}
	inline void fill(char*first, char*last, const char&val) {
		memset(first, static_cast<unsigned char>(val), last - first);
	}
	inline void fill(wchar_t *first, wchar_t *last, const wchar_t& val) {
		memset(first, static_cast<unsigned char>(val), sizeof(wchar_t)*(last - first));
	}

	//********* [fill_n] ********************
	//********* [Algorithm Complexity: O(N)] ****************
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	template<class Size>
	char *fill_n(char *first, Size n, const char& value)
	{
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}
	template<class Size>
	wchar_t *fill_n(wchar_t *first, Size n, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
		return first + n;
	}

} //namespace TinySTL
#endif // !ALGOBASE_H

