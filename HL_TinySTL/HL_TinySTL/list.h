#ifndef LIST_H
#define LIST_H

#include"allocator.h"
#include"iterator.h"
#include"type_traits.h"

namespace TinySTL {
	
	//链表结点
	template<typename T>
	struct list_node {
		T data;
		list_node<T>* pre;
		list_node<T>* next;
	public:
		list_node() : data(0), pre(nullptr), next(nullptr) {}
		list_node(const T& val) : data(val), pre(nullptr), next(nullptr) {}
	};

	//链表迭代器
	template<typename T>
	class list_iterator : public iterator<bidirectional_iterator_tag,T> {
	public:
		typedef list_node<T>* nodeptr;
		nodeptr p;
	public:
		list_iterator& operator++() {
			p = p->next;
			return *this;
		}
		list_iterator& operator++(int) {
			auto tmp = *this;
			++*this;
			return tmp;
		}
		list_iterator& operator--() {
			p = p->pre;
			return *this;
		}
		list_iterator& operator--(int) {
			auto tmp = *this;
			--*this;
			return tmp;
		}
		T& operator*() { return p->data; }
		T* operator->() { return &(operator*()); }

		//
		template<typename T>
		friend bool operator==(const list_iterator<T>& lhs, const list_iterator<T>&rhs) {
			return lhs.p == rhs.p;
		}
		template<typename T>
		friend bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>&rhs) {
			return !(lhs.p == rhs.p);
		}
	};

	template<typename T>
	class list {
	public:
		typedef T		value_type;
		typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;
		typedef reverse_iterator<iterator>	 reverse_iterator;
		typedef T&	reference;
		typedef size_t size_type;
	private:
		typedef allocator<list_node<T>>		node_allocator;
		typedef allocator<list_node<T>>		node_type;
		typedef list_node<T>*						node_pointer;
		iterator		head;
		iterator		tail;
	private:
		//helper function
		node_pointer create_new_node(const value_type &val = value_type());
		void delete_node();
		
		void construct_aux(size_type n, const value_type& val, __true_type);
		template<typename InputIterator>
		void construct_aux(InputIterator first, InputIterator last, __false_type);

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
		iterator begin() { return head; }
		iterator end() { return tail; }

		/********************************************************************************/
		//容量相关相关
		bool empty();
		size_type size();
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

		void clear();
		
		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		void merge(list &other);
		template<typename Compare>
		void merge(list& other, Compare comp);

		void remove(const value_type& val);
		template<typename Predicate>
		void remove_if(Predicate pred);

		void reverse();

		void sort();
		template<typename Compare>
		void sort(Compare comp);

		void splice(iterator position, list&other);
		void splice(iterator position, list& other, iterator i);
		void splice(iterator position, list& other, iterator first, iterator last);

		void swap();
		
		void unique();
		template<typename BinaryPredicate>
		void unqiue(BinaryPredicate);
	}; //end of class

	
	/********************************************************************************/
	//helper function
	template<typename T>
	typename list<T>::node_pointer list<T>::create_new_node(const value_type& val) {
		node_pointer tmp = node_allocator::allocate();
		node_allocator::construct(tmp, list_node<value_type>(val));
		return tmp;
	}

	template<typename T>
	void list<T>::delete_node() {
		//从尾巴开始析构吧
		while (head != tail)
		{
			auto tmp = head;
			node_allocator::destory(tmp.p);
			node_allocator::deallocate(tmp.p, sizeof(node_type));
			head++;
		}
	}

	template<typename T>
	void list<T>::construct_aux(size_type n, const value_type &val, __true_type) {
		//是n个value_type类型的数据
		head.p = create_new_node();
		tail.p = head.p;
		while (n--) {
			push_back(val);
		}
	}
	template<typename T>
	template<typename InputIterator>
	void list<T>::construct_aux(InputIterator first, InputIterator last, __false_type) {
		//是根据一段序列进行构造		
		head.p = create_new_node();
		tail.p = head.p;
		for (; first != last; first++) {
			push_back(*first);
		}
	}
	/********************************************************************************/
	//构造、析构、复制等函数实现
	template<typename T>
	list<T>::~list() {
		delete_node();
	}

	template<typename T>
	list<T>::list() {
		head.p = create_new_node();
		tail.p = head.p;
	}

	template<typename T>
	list<T>::list(size_type n) {
		if (n == 0)
			list();
		head.p = tail.p = create_new_node();
		while(n--){
			push_back(value_type());
		}
	}

	template<typename T>
	list<T>::list(size_type n, const value_type& val) {
		typedef typename __is_integer<size_type>::is_integer IS_INTEGER;
		construct_aux(n, val, IS_INTEGER());
	}

	template<typename T>
	template<typename InputIterator>
	list<T>::list(InputIterator first, InputIterator last) {
		typedef typename __is_integer<InputIterator>::is_integer IS_INTEGER;
		construct_aux(first, last, IS_INTEGER());
	}

	/********************************************************************************/
	//元素修改相关相关
	template<typename T>
	void list<T>::push_back(const value_type& val) {
		node_pointer new_node = create_new_node();
		tail.p->data = val;
		tail.p->next = new_node;
		new_node->pre = tail.p;
		tail.p = new_node;
	}

} //namespace TinySTL
#endif // !LIST_H

