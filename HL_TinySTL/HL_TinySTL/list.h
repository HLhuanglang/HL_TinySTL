/*****************************************************************************
名称：list.h
注意：
	链表是双向环形链表，为了满足前闭后开原则，增设首元结点
	当链表为空时，只有首元结点需要初始化首元结点的指针，否则容易出现野指针的不可预估行为
	用node_指向首元结点，	node_->next 表示头结点  
											node_本身表示尾 
											node_->pre表示最后一个结点
*****************************************************************************/

#ifndef LIST_H
#define LIST_H

#include"allocator.h"
#include"iterator.h"
#include"type_traits.h"
#include"utility.h"
#include"functional.h"
#include"exceptdef.h"


namespace TinySTL {

	template<class T> class list_node_base;
	template<class T> class list_node;
	
	/**********************************************************list结点设计******************************************************/
	template<class T>
	class list_node_base
	{
	public:
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
		
			//指针域
		base_ptr prev;
		base_ptr next;

		list_node_base() = default;

		base_ptr self() {
				//将这个list_node_base对象的地址 转换成一个指针类型的地址
			return static_cast<base_ptr>(&*this);
		}
		void unlink() {
				//空结点首尾指针都指向自己
			prev = next = self();
		}
		node_ptr as_node() {
				//基类指针转换成派生类指针，使其可以访问派生类新增的成员
			return static_cast<node_ptr>(self());
		}
	};

	template<class T>
	class list_node : public list_node_base<T>
	{
	public:
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
			
			//值域
		T value;

		list_node() = default;
		list_node(const T&val) : value(val) {}

		node_ptr self() {
			return static_cast<node_ptr>(&*this);
		}
		base_ptr as_base() {
			//派生类转换成基类指针，去除了指向value的能力
			return static_cast<base_ptr>(&*this);
		}
	};

	/**********************************************************list迭代器设计*****************************************************/
	//const迭代器和非const迭代器，无法直接用强制转换实现。因为内部操作是不同的

	template<class  T>
	struct list_iterator : public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T> {
		typedef T		value_type;
		typedef T*	pointer;
		typedef T&	reference;
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
		typedef list_iterator<T>	self;

		base_ptr node_;	//指向结点

		//构造函数
		list_iterator() = default;
		list_iterator(base_ptr x) : node_(x) {}
		list_iterator(node_ptr x): node_(x->as_base()){}
		//copy构造函数
		list_iterator(const list_iterator<T>& rhs)  : node_(rhs.node_) {}

		//重载操作符
		reference operator*() const { return  node_->as_node()->value; }
		pointer		operator->() const { return &(operator*()); }
		
		self& operator++() {	//前置++
			STL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}

		self operator++(int) {	//后置++
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--() {
			STL_DEBUG(node_ != nullptr);
			node_ = node_->prev;
			return *this;
		}

		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}

		//重载比较运算符
		bool operator ==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator !=(const self& rhs) const { return node_ != rhs.node_; }
	};

	template <class T>
	struct list_const_iterator : public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T>
	{
		typedef T                                 value_type;
		typedef const T*                          pointer;
		typedef const T&                          reference;
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
		typedef list_const_iterator<T>            self;

		base_ptr node_;

		list_const_iterator() = default;
		list_const_iterator(base_ptr x) : node_(x) {}
		list_const_iterator(node_ptr x) : node_(x->as_base()) {}
		list_const_iterator(const list_iterator<T>& rhs)   : node_(rhs.node_) {}
		//copy构造函数
		list_const_iterator(const list_const_iterator<T>& rhs)   : node_(rhs.node_) {}

		reference operator*()  const { return node_->as_node()->value; }
		pointer   operator->() const { return &(operator*()); }

		self& operator++()
		{
			STL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--()
		{
			STL_DEBUG(node_ != nullptr);
			node_ = node_->prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}

		// 重载比较操作符
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
	};

	/**********************************************************list容器结构设计******************************************************/
	template<class T>
	class list {
	public:
		typedef	TinySTL::allocator<T>									allocator_type;		//用于提取allocator的一些型别定义
		typedef	TinySTL::allocator<T>									data_allocator;		//用于在结点的值域处 构造对象
		typedef	TinySTL::allocator<list_node<T>>				node_allocator;	//申请、销毁list_node结点
		typedef	TinySTL::allocator<list_node_base<T>>		base_allocaror;		//申请、销毁list_node_base结点

		//list内嵌型别定义
		typedef typename allocator_type::value_type			value_type;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::reference				reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::size_type				size_type;
		typedef typename allocator_type::difference_type		difference_type;

		//迭代器定义
		typedef list_iterator<T>					iterator;
		typedef list_const_iterator<T>		const_iterator;
		typedef TinySTL::reverse_iterator<iterator>				reverse_iterator;
		typedef TinySTL::reverse_iterator<const_iterator>		const_reverse_iterator;

		//内部结点指针定义
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;

	private:
		base_ptr	node_;	//指向 为了满足前闭后开原则设置的结点 的指针
		size_type	size_;	//链表结点个数

	private:
		//helper function

		//申请结点和销毁结点
		node_ptr create_node(const value_type&val);
		void destory_node(node_ptr p);
	
		//初始化构建
		void fill_init(size_type n, const value_type& val = value_type());
		template<class Iter>
		void copy_init(Iter fistr, Iter last);

		/*插入构建
		插入新结点时，需生成新结点和选择插入位置，不同位置插入导致链接方式也不一样
		可以自定义新结点值，也可以根据一段序列的值进行结点创建
		*/
		iterator link_one_node(const_iterator position, base_ptr node);		//在position处链接node指向的结点。返回node结点位置
		void link_node(base_ptr p, base_ptr first, base_ptr last);					//在p处链接  [first,last）这一段结点
		void link_node_at_front(base_ptr first, base_ptr last);							//在链表头链接[first，last）这段结点
		void link_node_at_back(base_ptr first, base_ptr last);							//在链表尾部链接[first，last)这段结点
		void unlink_node(base_ptr first, base_ptr last);									//从链表中断开[first,last)这段结点

		iterator fill_insert(const_iterator pos, size_type n, const value_type& val);	//自定义值、位置、个数，插入新结点段
		template<class Iter>
		iterator copy_insert(const_iterator pos, size_type n, Iter first);		//取序列[first，last）中每个元素的值作为value进行结点构造，然后插入
	

		template<class InputIterator>
		void list_aux(InputIterator first, InputIterator last, __false_type) 
		{
			copy_init(first, last);
		}
		void list_aux(size_type n, const value_type& val, __true_type) 
		{
			fill_init(n, val);
		}
		template<class InputIterator>
		void insert_aux(const_iterator pos, InputIterator first, InputIterator last, __false_type)
		{
			size_type n = 0;
			auto tmp = first;
			for (; tmp != last;++tmp)
				n++;
			copy_insert(pos, n, first);
		}
		void insert_aux(const_iterator pos, size_type n, const value_type & val, __true_type) 
		{
			fill_insert(pos, n, val);
		}

	public:
		/********************************************************************************/
		//对象构造、析构相关
		list();
		list(size_type n);
		list(size_type n, const value_type& val);
		template<typename InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& other);
		~list();

		/********************************************************************************/
		//迭代器相关
		iterator begin() { return node_->next; }
		iterator end() { return node_; }
		const_iterator begin() const { return	 node_->next; }
		const_iterator end() const { return node_; }

		const_iterator cbegin()  const { return begin(); }
		const_iterator cend()  const { return end(); }

		/********************************************************************************/
		//容量相关相关
		bool empty() { return node_->next == node_; }
		size_type size() const { return size_; }
		void resize(size_type _Newsize);
		void resize(size_type _Newsize, const value_type& val);

		/********************************************************************************/
		//元素访问相关
		reference front() { STL_DEBUG(!empty()); return *begin(); }
		reference back() { STL_DEBUG(!empty()); return *(--end()); }

		/********************************************************************************/
		//元素修改相关相关
		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back();
		void pop_front();

		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		void splice(iterator position, list&other);
		void splice(iterator position, list& other, iterator i);
		void splice(iterator position, list& other, iterator first, iterator last);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		void merge(list &other);
		template<typename Compare>
		void merge(list& other, Compare comp);

		void remove(const value_type& val);
		template<typename Predicate>
		void remove_if(Predicate pred);

		void unique();
		template<typename BinaryPredicate>
		void unqiue(BinaryPredicate);

		void clear();
		void reverse();
		void swap(list& other);
		void sort();
		template<typename Compare>
		void sort(Compare comp);
	}; //end of  list class


	/********************************************************************************/
	//helper function 实现
	template<class T>
	typename list<T>::node_ptr	list<T>::create_node(const value_type& val) {
		node_ptr node = node_allocator::allocate(1);
		data_allocator::construct(&node->value, val);
		node->prev = nullptr;
		node->next = nullptr;
		return node;
	}

	template<class T>
	void list<T>::destory_node(node_ptr p) {
		data_allocator::destory(&p->value);
		node_allocator::deallocate(p);
	}

	template<class T>
	void list<T>::fill_init(size_type n, const value_type&val) {
		node_= base_allocaror::allocate(1);
		node_->unlink();	//如果没写，首元结点node_的指针会变成野指针
		size_ = n;
		if (n == 0) return;
		for (; n > 0; --n) {
			node_ptr new_node = create_node(val);
			link_node_at_back(new_node->as_base(), new_node->as_base());
		}
	}
	
	template<class T>
	template<class Iter>
	void list<T>::copy_init(Iter first, Iter last) {
		node_ = base_allocaror::allocate(1);
		node_->unlink();
		//auto tmp = first; 
		for (; first != last; ++first) {
			size_++;
			node_ptr  new_node = create_node(*first);
			link_node_at_back(new_node->as_base(), new_node->as_base());
		}
	}

	template<class T>
	typename list<T>::iterator list<T>::link_one_node(const_iterator pos, base_ptr node) {
		if (pos == node_->next){
			link_node_at_front(node, node);
		}
		else if (pos == node_) {
			link_node_at_back(node, node);
		}
		else
			link_node(pos.node_, node, node);
		return node;
	}

	template<class T>
	void list<T>::link_node(base_ptr p, base_ptr first, base_ptr last) {
		p->prev->next = first;
		last->next = p;
		first->prev = p->prev;
		p->prev = last;
	}

	template<class T>
	void list<T>::link_node_at_front(base_ptr first, base_ptr last) {
		last->next = node_->next;
		node_->next->prev = last;
		first->prev = node_;
		node_->next = first;
	}

	template<class T>
	void list<T>::link_node_at_back(base_ptr first, base_ptr last) {
		last->next = node_;
		first->prev = node_->prev;  //
		first->prev->next = first;		 //如果node_之前没有结点，这两句句的作用就是first=first；
		node_->prev = last;
	}

	template<class T>
	void list<T>::unlink_node(base_ptr first, base_ptr last) {
		first->prev->next = last->next;
		last->next->prev = first->prev;
	}

	template<class T>
	typename list<T>::iterator list<T>::fill_insert(const_iterator pos, size_type n, const value_type& val) {
		size_ += n;
		iterator  r;
		for (; n > 0; n--) {
			node_ptr new_node = create_node(val);
			r=link_one_node(pos, new_node->as_base());
		}
		return r;
	}

	template<class T>
	template<class Iter>
	typename list<T>::iterator
		list<T>::copy_insert(const_iterator pos, size_type n, Iter first) {
		size_type add_size = n;
		size_ += add_size;
		iterator  r;
		for (; n > 0; n--, ++first) {
			node_ptr new_node = create_node(*first);
			r = link_one_node(pos, new_node->as_base());
		}
		return r;
	}

	/********************************************************************************/
	//对象构造、析构相关
	template<class T>
	list<T>::list() {
		fill_init(0, value_type());
	}

	template<class T>
	list<T>::list(size_type n) {
		fill_init(n, value_type());
	}

	template<class T>
	list<T>::list(size_type n, const value_type& val) {
		typedef typename __is_integer<size_type>::is_integer IS_INTEGER;
		list_aux(n, val, IS_INTEGER());
	}

	template<class T>
	template<class InputIterator>
	list<T>::list(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer IS_INTEGER;
		list_aux(first,last, IS_INTEGER());
	}

	template<class T>
	list<T>::list(const list& other) {
		copy_init(other.cbegin(), other.cend());
	}

	template<class T>
	list<T>::~list() {
		//普通结点和首元结点分开销毁
		if (node_) {
			clear();
			base_allocaror::deallocate(node_);
			node_ = nullptr;
			size_ = 0;
		}
	}

	/********************************************************************************/
	//容量相关相关
	template<class T>
	void list<T>::resize(size_type _Newsize) {
		resize(_Newsize, value_type());
	}

	template<class T>
	void list<T>::resize(size_type _Newsize, const value_type& val) {
		if (_Newsize > size()) {
			size_type add_size = _Newsize - size_;
			while (add_size--)
				push_back(val);
			size_ = _Newsize;
		}
		else {
			size_type delete_size = size() - _Newsize;
			while (delete_size--)
				pop_back();	//注意pop_back本身也会对size_进行减一
			size_ = _Newsize;
		}
	}

	/********************************************************************************/
	//元素修改相关相关
	template<class T>
	void list<T>::clear() {
		if (size_ !=  0) {
			base_ptr cur = node_->next;
			for (base_ptr next = cur->next; cur != node_; cur = next, next = next->next) {
				destory_node(cur->as_node());
			}
		}
		//清除完毕，剩下首元结点
		node_->unlink();
		size_ = 0;
	}

	template<class T>
	void list<T>::push_back(const value_type& val) {
		node_ptr new_node = create_node(val);
		link_node_at_back(new_node->as_base(),new_node->as_base());
		size_++;
	}

	template<class T>
	void list<T>::push_front(const value_type& val) {
		node_ptr  new_node = create_node(val);
		link_node_at_front(new_node->as_base(), new_node->as_base());
		size_++;
	}

	template<class T>
	void list<T>::pop_back() {
		if (!empty()) {
			base_ptr tmp = node_->prev;
			unlink_node(tmp, tmp);
			tmp->unlink();
			destory_node(tmp->as_node());
			size_--;
		}
	}

	template<class T>
	void list<T>::pop_front() {
		if(!empty()){
			base_ptr tmp = node_->next;
			unlink_node(tmp, tmp);
			tmp->unlink();
			destory_node(tmp->as_node());
			size_--;
		}
	}

	template<class T>
	typename list<T>::iterator
		list<T>::insert(iterator pos, const value_type&val) {
		node_ptr new_node = create_node(val);
		size_++;
		return 	link_one_node(pos, new_node->as_base());
	}

	template<class T>
	void list<T>::insert(iterator pos, size_type n, const value_type& val) {
		typedef typename __is_integer<size_type>::is_integer	IS_INTEGER;
		insert_aux(pos, n, val, IS_INTEGER());
	}

	template<class T>
	template<class InputIterator>
	void list<T>::insert(iterator pos, InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer	IS_INTEGER;
		insert_aux(pos, first, last, IS_INTEGER());
	}

	template<class T>
	typename list<T>::iterator
		list<T>::erase(const_iterator pos) {
		if (pos != cend()) {
			base_ptr tmp = pos.node_;
			base_ptr next = tmp->next;
			unlink_node(tmp, tmp);
			destory_node(tmp->as_node());
			--size_;
			return iterator(next);
		}
	}

	template<class T>
	typename list<T>::iterator
		list<T>::erase(const_iterator first, const_iterator last) {
		for (; first != last;++first) {
			erase(first);
		}
		return last.node_;
	}

} //namespace TinySTL
#endif // !LIST_H

