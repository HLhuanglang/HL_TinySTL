/**************************************************************
名称：construct.h
作用：负责对象的构造和析构
**************************************************************/

#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include"type_traits.h"
#include<new>  //for placement new

namespace TinySTL {
	
	template<typename T1>
	inline void construct(T1* pointer) {
		new(pointer) T1();
	}
	
	template<typename T1, typename T2>
	inline void construct(T1 *pointer, const T2& value) {
		new(pointer) T1(value);
	}
	
	template<typename T>
	inline void destory(T *pointer) {
		pointer->~T();
	}

	template<typename InputIterator>
	inline void _destory(InputIterator first, InputIterator last, __true_type) {
		//内置类型，析构与否都无所谓，所以不做处理
	}

	template<typename InputIterator>
	inline void _destory(InputIterator first, InputIterator last, __false_type) {
		//不是内置类型，属于用户自定义类型，需要析构释放资源
		while (first != last)
		{
			destory(&*first); //因为迭代器都是指针，*first取得内容，再用&取得地址
			++first;
		}
	}

	template<typename InputIterator>
	inline void destory(InputIterator firsrt, InputIterator last) {
		typedef typename __type_traits<InputIterator>::is_POD_type		IS_POD_TYPE;
		_destory(firsrt, last, IS_POD_TYPE);
	}

} //namespace TinySTL
#endif // !CONSTRUCT_H

