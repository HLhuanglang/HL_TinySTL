/************************************************************
���ƣ�vector.h
���ã�vector�������洢���ݣ��ṩ����������Ԫ�ص�һЩ����
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

		/*vector��������ķ�ʽ��
		vector<T> v1;
		vector<T> v2(10); //10��Ĭ��T
		vector<T> v3(10,T()); //10��T���͵�ֵ
		vector<T> v4 = v1; //=��ֵ
		vector<T> v5(v2); //����
		
		array<string, 5> words {"one", "two","three", "four", "five"};
		vector<string> v6 {begin(words) , end(words)}; ����һ�Ե��������д���
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

		//���������
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
			first_ = data_allocator::allocate(last - first); //�Լ����һ�� (last-first)/sizeof(InputIterator),��ָ������Զ����ȥ���͵�λ
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
		//�����Ƕ���vector<T>ʱ�����TֻӰ��value_type, size_type��T �޹أ����Ǹ�size_t
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
