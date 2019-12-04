/*
名称：functional.h
注意：
	重载括号操作符()，用const修饰表示不会更改函数对象的状态
*/
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
	//函数对象：加法 plus<T>
	template<class T>
	struct plus : public binary_function<T, T, T> {
		T operator()(const T&x, const T& y) const { return x + y; }
	};
	//函数对象：减法minus<T>
	template<class T>
	struct minus : public binary_function<T, T, T> {
		T operator()(const T&x, const T& y) const { return x - y; }
	};
	//函数对象：乘法multiples<T>
	template<class T>
	struct multiplies : public binary_function<T, T, T> {
		T operator()(const T&x, const T& y) const { return x*y; }
	};	
	//函数对象：除法divides<T>
	template<class T>
	struct divides : public binary_function<T, T, T> {
		T operator()(const T&x, const T& y) const { return x/y; }
	};	
	//函数对象：取模modulus<T>
	template<class T>
	struct modulus : public binary_function<T, T, T> {
		T operator()(const T&x, const T& y) const { return x%y; }
	};	
	//函数对象：否定negate<T>
	template<class T>
	struct negate : public unarg_function<T,T> {
		T operator()(const T&x ) const { return -x; }
	};


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
	struct greater :public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x > y; }
	};
	// 函数对象：小于less
	template <class T>
	struct less :public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x < y; }
	};
	// 函数对象：大于等于greater_equal
	template <class T>
	struct greater_equal :public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};
	// 函数对象：小于等于less_equal
	template <class T>
	struct less_equal :public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x <= y; }
	};

	/*******************************************逻辑类仿函数**********************************/
	//函数对象：逻辑运算and logical_and<T>
	template<class T>
	struct logical_and : public binary_function<T, T, bool> {
		bool operator()(const T&x, const T&y) const { return x && y; }
	};
	//函数对象：逻辑运算or logical_or<T>
	template<class T>
	struct logical_or : public binary_function<T, T, bool> {
		bool operator()(const T&x, const T&y) const { return x || y; }
	};
	//函数对象：逻辑运算not logical_not<T>
	template<class T>
	struct logical_not : public unarg_function<T,bool> {
		bool operator()(const T&x) const { return  !x; }
	};

} //namespace TinySYL
#endif // !FUNCTIONAL_H

