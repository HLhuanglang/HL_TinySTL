/*******************************************************************************************
名称：type_traits
作用：萃取型别的特性，判断该型别是否具有 默认构造函数、copy构造函数、赋值操作符。
*******************************************************************************************/
#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

//编译器只对 class object形式的参数才进行参数推导
struct __true_type {};
struct __false_type {};

template<class type>
struct __type_traits {
	typedef __false_type	has_trivial_default_constructor;
	typedef __false_type	has_trivial_copy_constructor;
	typedef __false_type	has_trivial_assignment_operator;
	typedef __false_type	has_trivial_destructor;
	typedef __false_type	is_POD_type;
};

/*以下针对C++基本型别
	char、signed char、 unsigned char
	short、unsigned short
	int、 unsigned int
	long、 unsigned long
	float、 double、long double
	每个成员的值都是 __true_type ，表示这些型别都可以采用最快速的方式进行拷贝或者赋值操作*/

template<>
struct __type_traits<char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<signed char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<unsigned char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<short> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<unsigned short> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<int> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<unsigned int> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};
template<>
struct __type_traits<long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<unsigned long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<float> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<double> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template<>
struct __type_traits<long double> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

//针对原生指针设计的 __type_traits偏特化版本
template<class T>
struct __type_traits<T*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};
	//判断是否为 int 类型
	template <class Type>
	struct __is_integer {
		typedef __false_type	is_integer;
	};

	template<> struct __is_integer<bool> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<char> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<signed char> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<unsigned char> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<short> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<unsigned short> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<int> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<unsigned int> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<long> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<unsigned long> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<long long> {
		typedef __true_type		is_integer;
	};

	template<> struct __is_integer<unsigned long long> {
		typedef __true_type		is_integer;
	};
#endif // !TYPE_TRAITS_H
