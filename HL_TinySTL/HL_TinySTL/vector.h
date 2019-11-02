/************************************************************
名称：vector.h
作用：vector容器，存储数据，提供基本方法对元素的一些访问
*************************************************************/
#ifndef VECTOR_H
#define VECTOR_H
#include"iterator.h"
#include"allocator.h"
#include"uninitialized.h"
namespace TinySTL {
	template<typename T, typename Alloc= allocator<T>>
	class vector {
	private:
		T* first_;
		T* last_;
		T* end_;
		typedef Alloc data_allocator;
	public:
		typedef T						value_type;
		typedef value_type*	iterator;
		typedef value_type*	pointer;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		/*vector创建对象的方式：
		vector<T> v1;
		vector<T> v2(10); //10个默认T
		vector<T> v3(10,T()); //10个T类型的值
		vector<T> v4 = v1; //=赋值
		vector<T> v5(v2); //拷贝
		
		array<string, 5> words {"one", "two","three", "four", "five"};
		vector<string> v6 {begin(words) , end(words)}; 接收一对迭代器进行创建
		*/
	public:
		vector() : first_(nullptr), last_(nullptr), end_(nullptr) {
		}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last);
		~vector()
		{
			data_allocator::destory(first_, last_); 
			data_allocator::deallocate(first_, end_-first_);
		}

		//迭代器相关
		iterator begin() { return iterator(first_); }
		iterator end() { return iterator(last_); }

	private:
		//helper functions
		void allocate_and_fill_n(const size_type n, const value_type& value) {
			first_ = data_allocator::allocate(n);
			uninitialized_fill_n(first_, n, value);
			last_ = end_ = first_ + n;
		}
		template<typename InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last) {
			first_ = data_allocator::allocate(last - first); //自己多此一举 (last-first)/sizeof(InputIterator),两指针相减自动会除去类型单位
			uninitialized_copy(first, last, first_);
			last_ = end_ = first_ + (last - first);
		}

		template<typename Integer>
		void vector_construct(Integer n, Integer value, __true_type) {
			allocate_and_fill_n(n, value);
		}
		template<typename InputIterator>
		void vector_construct(InputIterator first, InputIterator last,__false_type) {
			allocate_and_copy(first, last);
		}
	};

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocate_and_fill_n(n, value_type());
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type &value) {
		//当我们定义vector<T>时，这个T只影响value_type, size_type和T 无关，就是个size_t
		allocate_and_fill_n(n, value);
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer INTEGER;
		vector_construct(first, last, INTEGER());
	}
}//namaspace TinySTL
#endif // !VECTOR_H
