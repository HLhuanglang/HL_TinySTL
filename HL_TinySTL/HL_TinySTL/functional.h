#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

namespace TinySTL {

	//用于一元函数的参数型别和返回值型别
	template<class Arg, class Result>
	struct unarg_function
	{
		typedef Arg		argument_type;
		typedef Result		result_type;
	};

	//用于呈现二元函数的第一参数型别、第二参数型别，以及返回值型别
	template<class Arg1, class Arg2, class Result>
	struct binary_function
	{
		typedef Arg1		first_argument_type;
		typedef Arg2		second_argument_type;
		typedef Result		result_type;
	};


	/*******************************************算术类仿函数**********************************/
	
	/*******************************************关系运算类仿函数**********************************/
	//函数对象：等于 equal_to
	template<class T>
	struct equal_to : public binary_function<T, T, bool> {
		bool  operator()(const T&x, const T&y) const { return x == y; }
	};
	//函数对象：不等于not_equal_to
	template<class T>
	struct not_equal_to : public binary_function<T, T, bool> {
		bool operator()(const T&x, const T&y) const { return x != y; }
	};
	// 函数对象：大于greater
	template <class T>
	struct greater :public binary_function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x > y; }
	};
	// 函数对象：小于less
	template <class T>
	struct less :public binary_function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	// 函数对象：大于等于greater_equal
	template <class T>
	struct greater_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};
	// 函数对象：小于等于less_equal
	template <class T>
	struct less_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x <= y; }
	};

	/*******************************************逻辑类仿函数**********************************/


} //namespace TinySYL
#endif // !FUNCTIONAL_H

