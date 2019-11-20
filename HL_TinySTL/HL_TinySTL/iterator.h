/************************************************************
名称：iterator.h
作用：萃取迭代器类型
*************************************************************/

#ifndef ITERATOR_H
#define ITERATOR_H
#include<cstddef> //为了使用ptrdiff_t(一个与机器相关的数据类型，ptrdiff_t类型变量通常用来保存两个指针减法操作的结果)
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



	//被继承，如 class ListIterator： public iterator<bidirectional_iterator_tag, T>
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

	//以下函数用于计算迭代器之间的距离
	template<class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		return distance_dispatch(first, last, iterator_category(first));
	}

		//distance的input_iterator_tag版本
		template<class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
			distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
			typename iterator_traits<InputIterator>::difference_type n = 0;
			while (first != last) {
				++first;
				++n;
			}
			return n;
		}

		//distance的random_access_iterator_tag版本
		template <class RandomIter>
		typename iterator_traits<RandomIter>::difference_type
			distance_dispatch(RandomIter first, RandomIter last,
				random_access_iterator_tag)
		{
			return last - first;
		}

	//**********************************************************************************
	//reverse_iterator
	template<class Iterator>
	class reverse_iterator {
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef const pointer const_pointer;
		typedef typename iterator_traits<Iterator>::reference reference;
		typedef const reference const_reference;
	private:
		Iterator base_;
		Iterator cur_;
	public:
		//构造。复制，析构相关
		reverse_iterator() :base_(0), cur_(0) {}
		//explicit reverse_iterator(const iterator_type& it) :base_(it), cur_(it - 1){}
		explicit reverse_iterator(const iterator_type& it) :base_(it) {
			auto temp = it;
			cur_ = --temp;
		}
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& rev_it) {
			base_ = (iterator_type)rev_it.base();
			auto temp = base_;
			cur_ = --temp;
		};

		//其他成员函数
		iterator_type base() { return base_; }
		reference operator*() { return (*cur_); }
		const_reference operator*()const { return(*cur_); }
		pointer operator->() { return &(operator *()); }
		const_pointer operator->()const { return &(operator*()); }
		reverse_iterator& operator ++() {
			--base_;
			--cur_;
			return *this;
		}
		reverse_iterator& operator ++(int) {
			reverse_iterator temp = *this;
			++(*this);
			return temp;
		}
		reverse_iterator& operator--() {
			++base_;
			++cur_;
			return *this;
		}
		reverse_iterator  operator--(int) {
			reverse_iterator temp = *this;
			--(*this);
			return temp;
		}
		reference operator[] (difference_type n) {
			return base()[-n - 1];
		}
		reverse_iterator operator + (difference_type n)const;
		reverse_iterator& operator += (difference_type n);
		reverse_iterator operator - (difference_type n)const;
		reverse_iterator& operator -= (difference_type n);
	private:
		Iterator advanceNStep(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			random_access_iterator_tag) {
			if (right) {
				it += n;
			}
			else {
				it -= n;
			}
			return it;
		}
		Iterator advanceNStep(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			bidirectional_iterator_tag) {
			difference_type i;
			difference_type absN = n >= 0 ? n : -n;
			if ((right && n > 0) || (!right && n < 0)) {// ->
				for (i = 0; i != absN; ++i) {
					it = it + 1;
				}
			}
			else if ((!right && n > 0) || (right && n < 0)) {// <-
				for (i = 0; i != absN; ++i) {
					it = it - 1;
				}
			}
			return it;
		}
	public:
		template <class Iterator>
		friend bool operator == (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
		template <class Iterator>
		friend bool operator != (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
		template <class Iterator>
		friend bool operator < (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
		template <class Iterator>
		friend bool operator <= (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
		template <class Iterator>
		friend bool operator > (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
		template <class Iterator>
		friend bool operator >= (const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);

		template <class Iterator>
		friend reverse_iterator<Iterator> operator + (
			typename reverse_iterator<Iterator>::difference_type n,
			const reverse_iterator<Iterator>& rev_it);
		template <class Iterator>
		friend typename reverse_iterator<Iterator>::difference_type operator- (
			const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs);
	};// end of reverse_iterator

	template<class Iterator>
	reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator += (difference_type n) {
		base_ = advanceNStep(base_, n, false, iterator_category());
		cur_ = advanceNStep(cur_, n, false, iterator_category());
		return *this;
	}
	template<class Iterator>
	reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator -= (difference_type n) {
		base_ = advanceNStep(base_, n, true, iterator_category());
		cur_ = advanceNStep(cur_, n, true, iterator_category());
		return *this;
	}
	template<class Iterator>
	reverse_iterator<Iterator> reverse_iterator<Iterator>::operator + (difference_type n)const {
		reverse_iterator<Iterator> res = *this;
		res += n;
		return res;
	}
	template<class Iterator>
	reverse_iterator<Iterator> reverse_iterator<Iterator>::operator - (difference_type n)const {
		reverse_iterator<Iterator> res = *this;
		res -= n;
		return res;
	}

	template <class Iterator>
	bool operator == (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.cur_ == rhs.cur_;
	}
	template <class Iterator>
	bool operator != (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(lhs == rhs);
	}
	template <class Iterator>
	bool operator < (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.cur_ < rhs.cur_;
	}
	template <class Iterator>
	bool operator > (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return lhs.cur_ > rhs.cur_;
	}
	template <class Iterator>
	bool operator >= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(lhs < rhs);
	}
	template <class Iterator>
	bool operator <= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
		return !(lhs > rhs);
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator + (
		typename reverse_iterator<Iterator>::difference_type n,
		const reverse_iterator<Iterator>& rev_it) {
		return rev_it + n;
	}
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator - (
		const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return lhs.cur_ - rhs.cur_;
	}

} //namespace TinySTL
#endif // !ITERATOR_H

