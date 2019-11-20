/*****************************************************************************
名称：list.h
注意：
	链表是双向环形链表，为了满足前闭后开原则，增设首元结点
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
	class list_iterator : public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T> {
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
		list_iterator(const list_iterator& rhs) : node_(rhs.node_) {}

		//重载操作符
		reference operator*() const { return  node_->as_node()->value; }
		pointer		operator->() const { return &(operator*()); }
		
		self& operator++() {
			STL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}

		self operator++(int) {
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
	struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
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
		list_const_iterator(const list_iterator<T>& rhs) : node_(rhs.node_) {}
		list_const_iterator(const list_const_iterator& rhs) : node_(rhs.node_) {}

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
		typedef	TinySTL::allocator<T>									data_allocator;		//用于对象构造
		typedef	TinySTL::allocator<list_node<T>>				node_allocator;	//申请list_node结点
		typedef	TinySTL::allocator<list_node_base<T>>		base_allocaror;		//申请list_node_base结点，作为首元结点

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
		iterator begin();
		iterator end();

		/********************************************************************************/
		//容量相关相关
		bool empty();
		size_type size() const;
		void resize(size_type _Newsize);
		void resize(size_type _Newsize, const value_type& val);

		/********************************************************************************/
		//元素访问相关
		reference front();
		reference back();

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
	}; //end of class



} //namespace TinySTL
#endif // !LIST_H

