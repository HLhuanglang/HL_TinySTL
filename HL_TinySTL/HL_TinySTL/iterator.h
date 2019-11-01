/************************************************************
名称：iterator.h
作用：萃取迭代器类型
*************************************************************/

#ifndef ITERATOR_H
#define ITERATOR_H
#include<cstddef> //为了使用ptrdiff_t
namespace TinySTL {

	//五种类型标签，用来萃取和函数重载
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//input_iterator
	template<class T, class Distance>
	struct input_iterator {
		typedef input_iterator_tag		iterator_category;
		typedef T									value_type;
		typedef Distance						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	//output_iterator
	template <class T, class Distance>
	struct output_iterator {
		typedef output_iterator_tag			iterator_category;
		typedef T											value_type;
		typedef Distance								difference_type;
		typedef T*										pointer;
		typedef T&										reference;
	};

	//forward_iterator
	template <class T, class Distance>
	struct forward_iterator {
		typedef forward_iterator_tag		iterator_category;
		typedef T										value_type;
		typedef Distance							difference_type;
		typedef T*									pointer;
		typedef T&									reference;
	};

	//bidirectional_iterator
	template <class T, class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T												value_type;
		typedef Distance									difference_type;
		typedef T*											pointer;
		typedef T&											reference;
	};

	//random_access_iterator
	template <class T, class Distance>
	struct random_access_iterator {
		typedef random_access_iterator_tag	iterator_category;
		typedef T												value_type;
		typedef Distance									difference_type;
		typedef T*											pointer;
		typedef T&											reference;
	};


	//用来帮助我们在自己的迭代器类中包含要求的类型别名
	//class my_iterator： public iterator
	template<class Category,
		class T,
		class Distance = ptrdiff_t,
		class Pointer = T * ,
		class Reference = T & >
		struct iterator {
		typedef Category		iterator_catagory;
		typedef T						value_type;
		typedef Distance			difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};

	//萃取迭代器特性
	//这个模板为迭代器的类型特性定义了一套标准的类型别名，让算法既可以用迭代器，也可以用一般的指针
	template<typename Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category		iterator_category;
		typedef typename Iterator::value_type				value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer						pointer;
		typedef typename Iterator::reference					reference;
	};

	//针对pointer 和pointer-to-const进行特化
	/*
		随机访问迭代器所要求的全部操作都可以运用到指针上，因此对于指针，iterator_category
		总是等同于random_access_iterator_tag
	*/
	template<typename T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag					iterator_category;
		typedef T																value_type;
		typedef ptrdiff_t													difference_type;
		typedef T*															pointer;
		typedef T&															reference;
	};

	template<typename T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag					iterator_category;
		typedef T																value_type;
		typedef ptrdiff_t													difference_type;
		typedef const T*													pointer;
		typedef const T&												reference;
	};

	//这个函数决定迭代器的类型 category
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//这个函数决定某个迭代器的distance type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//这个函数决定迭代器的value type
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::value_type
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


} //namespace TinySTL
#endif // !ITERATOR_H

