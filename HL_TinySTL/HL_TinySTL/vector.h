/************************************************************
名称：vector.h
作用：vector容器，存储数据，提供基本方法对元素的一些访问
note:		
		vector创建对象的方式：
		vector<T> v1;
		vector<T> v2(10); //10个默认T
		vector<T> v3(10,T()); //10个T类型的值
		vector<T> v4 = v1; //=赋值
		vector<T> v5(v2); //拷贝
		
		array<string, 5> words {"one", "two","three", "four", "five"};
		vector<string> v6 {begin(words) , end(words)}; 接收一对迭代器进行创建
*************************************************************/
#ifndef VECTOR_H
#define VECTOR_H
#include"iterator.h"
#include"allocator.h"
#include"uninitialized.h"
#include"utility.h"
#include"algorithm.h"
namespace TinySTL {
	template<typename T, typename Alloc = allocator<T>>
	class vector {
	public:
		//类型参数，可以直接使用的，如vector<Test>::pointer A; A 就是一个指针
		typedef T						value_type;
		typedef T*					iterator; //vector的迭代器是普通指针
		typedef const T*			const_iterator;
		typedef TinySTL::reverse_iterator<iterator> reverse_iterator;
		typedef TinySTL::reverse_iterator<const iterator> const_reverse_iterator;
		typedef T*					pointer;
		typedef T&					reference;
		typedef const T&		const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
	private:
		iterator first_;
		iterator last_;
		iterator end_;
		typedef Alloc data_allocator;

	public:
		/********************************************************************************/
		//对象构造、析构相关
		vector();
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last);

		vector(const vector& v);
		vector(vector &&v);
		vector &operator=(const vector &v);
		vector &operator=(vector&& v);
		~vector();
		/********************************************************************************/
		//迭代器相关
		iterator begin() { return first_; }
		iterator end() { return last_; }
		const_iterator begin() const { return first_; }
		const_iterator end() const { return last_; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }
		const_reverse_iterator crbegin() const { return rbegin(); }
		const_reverse_iterator crend() const { return rend(); }
		/********************************************************************************/
		//元素访问相关
		reference operator[](const difference_type i) { return *(first_ + i); }
		const_reference operator[](const difference_type i)const { return *(first_ + i); }
		reference front() { return *(first_); }
		reference back() { return *(last_ - 1); }
		reference at(); //TO DO
		/********************************************************************************/
		//容量相关
		difference_type size() const { return last_ - first_; }
		difference_type capacity() const { return end_ - first_; }
		bool empty() { return first_ == last_; }
		void resize(size_type n, value_type val = value_type()); //重置容器大小
		void reserve(size_type n); //预留空间大小，当n>capacity()时才重新分配
		void shrink_to_fit(); //收缩容量
		/********************************************************************************/
		//修改容器元素相关
		void clear();
		void swap(vector &v);
		void push_back(const value_type& val);
		void pop_back();
		iterator insert(iterator position, const value_type& val); //在position位置前插入一个元素
		void insert(iterator position, size_type n, const value_type& val); //在position之前，插入n个元素

		//将范围[first，last）之内的一段元素插入position
		//该函数与构造函数一样，当first，last都是int类型的话，是会匹配到上面那个函数的
		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		/********************************************************************************/
		//比较操作
		bool operator==(const vector &v) const;
		bool operator!=(const vector &v) const;
		/********************************************************************************/
		//容器的空间配置器相关
		Alloc get_allocator() { return data_allocator; }
	private:
		/********************************************************************************/
		//helper functions
				//只能把给定序列的内容复制到容器的内存起点位置，局限性有点大。。可以增加一个参数代表目的地
		void allocate_and_fill_n(const size_type n, const value_type& value);
		template<typename InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);

		void destory_and_deallocate();
		template<typename Integer>
		void vector_aux(Integer n, Integer value, __true_type);
		template<typename InputIterator>
		void vector_aux(InputIterator first, InputIterator last, __false_type);

		void insert_aux(iterator position, size_type n, const value_type& val, __true_type);
		template<typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, __false_type);

		void _insert(iterator position, size_type n, const value_type& val, __true_type);
		template<typename InputIterator>
		void _insert(iterator position, InputIterator first, InputIterator last, __false_type);

		void reallocate_and_fill_n(iterator position, size_type n, const value_type&val);
		template<typename InputIterator>
		void reallocate_and_copy(iterator position, InputIterator first, InputIterator last);
		size_type get_newsize(size_type n);
	}; //end of class vector

	/********************************************************************************/
	//helper functions
	template<typename T, typename Alloc>
	void vector<T, Alloc>::allocate_and_fill_n(const size_type n, const value_type& value) {
		first_ = data_allocator::allocate(n);
		uninitialized_fill_n(first_, n, value);
		last_ = end_ = first_ + n;
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
		this->first_ = data_allocator::allocate(last - first); //自己多此一举 (last-first)/sizeof(InputIterator),两指针相减自动会除去类型单位
		uninitialized_copy(first, last, this->first_);
		this->last_ = this->end_ =this->first_ + (last - first);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::destory_and_deallocate() {
		if (capacity() != 0) {
			data_allocator::destory(first_, last_);
			data_allocator::deallocate(first_, capacity());
		}
	}

	template<typename T, typename Alloc>
	template<typename Integer>
	void vector<T, Alloc>::vector_aux(Integer n, Integer value, __true_type) {
		allocate_and_fill_n(n, value); 
	}
	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, __false_type) {
		allocate_and_copy(first, last);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::_insert(iterator position, size_type n, const value_type &val, __true_type) {
		typedef typename __is_integer<value_type>::is_integer IS_INTEGER;
		insert_aux(position, n, val, IS_INTEGER());
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::_insert(iterator position, InputIterator first, InputIterator last, __false_type) {
		typedef typename __is_integer<value_type>::is_integer IS_INTEGER;
		insert_aux(position, first, last, IS_INTEGER());
	}
	template<typename T, typename Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, size_type n, const value_type &val, __true_type) {
		//个数和值都是一样的类型(int, char等)，不加判断会导致掉用下面的函数
		difference_type left_space = end_ - first_;
		difference_type need_space = n; //n不是只代表个数吗？难道这样写会自动乘以大小？
		if (need_space < left_space) {
			auto src = position;
			auto des = last_;
			for (; src != last_; ++src, ++des) {
				construct(des, *src);
			}
			last_ = TinySTL::uninitialized_fill_n(position, n, val);
		}
		else
			reallocate_and_fill_n(position, n, val);
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, __false_type) {
		difference_type left_space = end_ - last_;
		difference_type need_space = last - first;
		if (need_space < left_space) {
			auto src = position;
			auto des = last_;
			for (; src != last_; ++src, ++des) {
				construct(des, *src);
			}
			last_ = TinySTL::uninitialized_copy(first, last, position);
		}
		else
			reallocate_and_copy(position, first, last);
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::reallocate_and_fill_n(iterator position, size_type n, const value_type&val) {
		difference_type new_size = get_newsize(n);
		iterator new_first_ = data_allocator::allocate(new_size);
		iterator new_last_ = uninitialized_copy(begin(), position, new_first_);
		new_last_ = uninitialized_fill_n(new_last_, n, val);
		new_last_ = uninitialized_copy(position, end(), new_last_);

		destory_and_deallocate();
		first_ = new_first_;
		last_ = new_last_;
		end_ = new_first_ + new_size;
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::reallocate_and_copy(iterator position, InputIterator first, InputIterator last) {
		difference_type new_size = last_ - first_;
		iterator new_first_ = data_allocator::allocate(new_size);
		iterator new_last_ = uninitialized_copy(begin(), position, new_first_);
		new_last_ = uninitialized_copy(first, last, new_last_);
		new_last_ = uninitialized_copy(position, end(), new_last_);

		destory_and_deallocate();
		first_ = new_first_;
		last_ = new_last_;
		end_ = new_first_ + new_size;
	}

	template<typename T, typename Alloc>
	typename vector<T,Alloc>::size_type vector<T, Alloc>::get_newsize(size_type n) {
		size_type old_size = end_ - first_;
		auto res = TinySTL::max(old_size, n);
		size_type new_size = ( (old_size != 0) ? (old_size + res) : n);
		return new_size;
	}
	/********************************************************************************/
	//构造、析构、复制等函数实现
	template<typename T, typename Alloc>
	vector<T, Alloc>::~vector() {
		data_allocator::destory(first_, last_);
		data_allocator::deallocate(first_, end_ - first_);
	}
	template<typename T, typename Alloc>
	vector<T, Alloc>::vector() {
		allocate_and_fill_n(1, value_type());
	}

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
		vector_aux(first, last, INTEGER());
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector& v) {
		//copy构造，深复制
		allocate_and_copy(v.begin(), v.end());
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>::vector(vector&&v) {
		//右值引用，v是一个临时对象
		first_ = v.first_;
		last_ = v.last_;
		end_ = v.end_;
		v.first_ = v.last_ = v.end_ = nullptr;
	}

	template<typename T, typename Alloc>
	vector<T, Alloc>	&vector<T, Alloc>::operator=(const vector& v) {
		if (this != v) {
			allocate_and_copy(v.first_, v.last_); //TO FIX
		}
		return *this;
	}

	template<typename T, typename Alloc>
	vector<T, Alloc> &vector<T, Alloc>::operator=(vector&&v) {
		if (this != v)
			allocate_and_copy(v.first_, v.first_); //TO FIX
		v.first_ = v.last_ = v.end_ = nullptr;
	}
	/********************************************************************************/
	//元素访问
	/*
	类外实现要写的参数名称太长了，所以放在类内部实现了
	*/
	
	/********************************************************************************/
	//容量相关
	template<typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type val){
		if (n < size()) {
			data_allocator::destory(first_ + n, last_);
			last_ = first_ + n;
		}
		else if (n > size() && n < capacity()) {
			auto insert_length = n - size();
			last_ = TinySTL::uninitialized_fill_n(last_, insert_length, val);
		}
		else if (n >= capacity()) {
			size_type insert_length = n - size();
			iterator new_first_ = data_allocator::allocate(n);
			iterator new_last_ = TinySTL::uninitialized_copy(begin(), end(), new_first_);
			new_last_ = TinySTL::uninitialized_fill_n(new_last_, insert_length, val);

			destory_and_deallocate(); //将旧的资源释放
			first_ = new_first_;
			last_ = new_last_;
			end_ = new_first_ + n;
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (n > capacity()) {
			iterator new_first_ = data_allocator::allocate(n); //申请心内存为何会是返回first_的地址
			iterator new_last_ = TinySTL::uninitialized_copy(begin(), end(), new_first_);

			destory_and_deallocate(); //将旧的资源释放
			first_ = new_first_;
			last_ = new_last_;
			end_ = new_first_ + n;
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		//收缩容量到size()大小
		if (last_ == end_)
			return;
		data_allocator::deallocate(last_, end_ - last_); //不需要析构，因为last_ ~ end_这段区间没有构造对象
		end_ = last_;
	}

	/********************************************************************************/
	//修改容器相关
	template<typename T, typename Alloc>
	void vector<T, Alloc>::clear() {
		data_allocator::destory(first_, last_);
		last_ = first_;
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::swap( vector& v) {
		if (*this != v) {
			TinySTL::swap(this->first_, v.first_);
			TinySTL::swap(this->last_, v.last_);
			TinySTL::swap(this->end_, v.end_);
		}
	}

	template<typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const value_type& val) {
		insert(end(), val);
	}
	template<typename T, typename Alloc>
	void vector<T, Alloc>::pop_back() {
		//last-指向最后一个元素的下一个位置
		last_--;
		data_allocator::destory(last_);
	}
	
	template<typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val) {
		const auto index = position - begin();
		typedef typename __is_integer<value_type>::is_integer IS_INTEGER;
		_insert(position, 1, val, IS_INTEGER()); //
		return begin() + index;
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer IS_INTEGER;
		insert_aux(position, first, last, IS_INTEGER());
	}

	template<typename T, typename Alloc>
	void vector<T,Alloc>::insert(iterator position, size_type n, const value_type& val){
		insert_aux(position, n, val, typename __is_integer<value_type>::type()); //这里<>中size_type 和value_type都一样
	}

	/********************************************************************************/
	//比较操作
	template<typename T, typename Alloc>
	bool vector<T, Alloc>::operator==(const vector &v) const {
		if (size() != v.size())
			return false;
		auto self = first_;
		auto other = v.first_;
		for (; self != last_ && other != v.last_; ++self, ++other) {
			if (*self != *other)
				return false;
		}
		return true;
	}

	template<typename T, typename Alloc>
	bool vector<T, Alloc>::operator!=(const vector &v) const {
		return (!(*this == v));
	}

}//namaspace TinySTL
#endif // !VECTOR_H
