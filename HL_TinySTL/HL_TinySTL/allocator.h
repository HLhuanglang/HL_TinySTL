/************************************************************
名称：allocator.h
作用：对alloc.h和construct.h进行封装，对外提供接口。
注意：所有申请、释放都是以对象个数来进行的
*************************************************************/
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include"alloc.h"
#include"construct.h"
#include"exceptdef.h"

#include<new>
template<typename T>
class allocator {
public:
	//内嵌类型
	typedef T					value_type;
	typedef T*				pointer;
	typedef const T*		const_pointer;
	typedef T&				reference;
	typedef const T&	const_reference;
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

public:
	//接口
	static T* allocate();
	static T* allocate(size_t n); 
	static void deallocate(T* ptr);
	static void deallocate(T* ptr, size_t n); 

	static void construct(T *ptr);
	static void construct(T *ptr, const T&value);
	static void destory(T *ptr);
	static void destory(T *first, T *last);
};

template<typename T>
T* allocator<T>::allocate() {
	return static_cast<T*>(TinySTL::alloc::allocate(sizeof(T)));
}

template<typename T>
T* allocator<T>::allocate(size_t n) {
	STL_DEBUG(n != 0);
	return static_cast<T*>(TinySTL::alloc::allocate(n * sizeof(T)));
}

template<typename T>
void allocator<T>::deallocate(T *ptr){
	TinySTL::alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
}

template<typename T>
void allocator<T>::deallocate(T *ptr, size_t n) {
	STL_DEBUG(n != 0);
	TinySTL::alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
}

template<typename T>
void allocator<T>::construct(T *ptr) {
	TinySTL::construct(ptr);
}

template<typename T>
void allocator<T>::construct(T* ptr, const T&value) {
	TinySTL::construct(ptr, value);
}

template<typename T>
void allocator<T>::destory(T *ptr) {
	TinySTL::destory(ptr);
}
template<typename T>
void allocator<T>::destory(T *first, T *last) {
	for (; first != last; ++first)
		TinySTL::destory(first);
}

#endif // !ALLOCATOR_H

