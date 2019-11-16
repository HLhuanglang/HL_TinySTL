#ifndef LIST_H
#define LIST_H

#include"allocator.h"
#include"iterator.h"
#include"type_traits.h"
#include"utility.h"

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
		bool operator==(const list_node& n) {
			return data == n.data && pre == n.pre && next == n.next;
		}
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

		//以全局函数重载
		template<typename T>
		friend bool operator==(const list_iterator<T>& lhs, const list_iterator<T>&rhs) {
			return lhs.p == rhs.p; //这里涉及到结点的比较
		}
		template<typename T>
		friend bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>&rhs) {
			return !(lhs.p == rhs.p); //这里涉及到结点的比较
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
		typedef list_node<T>*						node_pointer;
		iterator		head;
		iterator		tail;
	private:
		//helper function
		node_pointer create_new_node(const value_type &val = value_type());
		void delete_node(node_pointer p);
		
		void construct_aux(size_type n, const value_type& val, __true_type);
		template<typename InputIterator>
		void construct_aux(InputIterator first, InputIterator last, __false_type);

		void insert_aux(iterator position, size_type n, const value_type& val, __true_type);
		template<typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last,__false_type);

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
		bool empty() { return head == tail; }
		size_type size() const;
		void resize(size_type _Newsize);
		void resize(size_type _Newsize, const value_type& val);
		
		/********************************************************************************/
		//元素访问相关
		reference front() { return head.p->data; }
		reference back() { return tail.p->data; }
		
		/********************************************************************************/
		//元素修改相关相关
		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back();
		void pop_front();

		void clear();
		void reverse();
		void swap(list& other);
		
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

		void sort();
		template<typename Compare>
		void sort(Compare comp);

		void splice(iterator position, list&other);
		void splice(iterator position, list& other, iterator i);
		void splice(iterator position, list& other, iterator first, iterator last);
		
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
	void list<T>::delete_node(node_pointer p) {
		p->pre = p->next = nullptr;
		node_allocator::destory(p);
		node_allocator::deallocate(p);
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

	template<typename T>
	void list<T>::insert_aux(iterator position, size_type n, const value_type &val, __true_type) {
		while (n--) {
			position= insert(position, val);
		}
	}

	template<typename T>
	template<typename InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, __false_type) {
		for (--last; first != last; --last) {
			position = insert(position, *last);
		}
		insert(position, *last);
	}
	/********************************************************************************/
	//构造、析构、复制等函数实现
	template<typename T>
	list<T>::~list() {
		//从头开始析构
		for (; head!= tail;) {
			iterator tmp = head++;
			delete_node(tmp.p);
		}
		//剩下最后一个空间结点
		delete_node(tail.p);
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

	template<typename T>
	list<T>::list(const list& other) {
		head.p = tail.p = create_new_node();
		for (node_pointer node = other.head.p; node != other.tail.p; node = node->next) {
			push_back(node->data);
		}
	}
	/********************************************************************************/
	//容量相关相关
	template<typename T>
	typename list<T>::size_type list<T>::size() const {
		size_type count=0;
		auto tmp = head;
		while (tmp!= tail) {
			count++;
			tmp++;
		}
		return count;
	}

	template<typename T>
	void list<T>::resize(size_type _Newsize) {
		resize(_Newsize, value_type());
	}

	template<typename T>
	void list<T>::resize(size_type _Newsize, const value_type&val){
		auto _Oldsize = size();
		if (_Newsize > _Oldsize) {
			auto size_to_add = _Newsize - _Oldsize;
			while (size_to_add--)
				push_back(val);
		}
		else {
			auto size_to_delete = _Oldsize - _Newsize;
			while (size_to_delete--)
				pop_back();
		}
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

	template<typename T>
	void list<T>::push_front(const value_type& val){
		node_pointer new_head = create_new_node(val);
		head.p->pre = new_head;
		new_head->next = head.p;
		head.p = new_head;
	}

	template<typename T>
	void list<T>::pop_back() {
		//肯定要保存末尾后一个空结点
		node_pointer tmp = tail.p->pre;
		tmp->pre->next = tail.p;
		tail.p->pre = tmp->pre;
		delete_node(tmp);
	}

	template<typename T>
	void list<T>::pop_front() {
		iterator old_head = head;
		head++;
		delete_node(old_head.p);
	}

	template<typename T>
	void list<T>::clear() {
		//删除所有结点
		while (head != tail) {
			iterator tmp = head++;
			delete_node(tmp.p);
		}
	}

	template<typename T>
	void list<T>::reverse() {
		//链表反转，可以用栈，用前插尾的方式
		if (empty() || head.p->next ==tail.p)  //后者条件表示只有一个结点，因为有一个伪结点
			return;
		node_pointer real_head = tail.p->pre;
		while(head.p != real_head){
			iterator curr_head = head++;
			curr_head.p->next->pre = nullptr;
			curr_head.p->next = real_head->next;
			real_head->next->pre = curr_head.p;
			real_head->next = curr_head.p;
			curr_head.p->pre = real_head;
		}
	}

	template<typename T>
	void list<T>::swap(list& other) {
		TinySTL::swap(head.p, other.head.p);
		TinySTL::swap(tail.p, other.tail.p);
	}

	template<typename T>
	typename list<T>::iterator list<T>::insert(iterator position, const value_type& val) {
		if (position == head) {
			push_front(val);
			return head;
		}
		else if(position ==tail) {
			push_back(val);
			return position;
		}
		node_pointer new_node = create_new_node(val);
		new_node->next = position.p;
		position.p->pre = new_node;
		position.p->pre->next = new_node;
		new_node->pre = position.p->pre;
		return iterator(new_node);
	}

	template<typename T>
	void list<T>::insert(iterator position, size_type n, const value_type& val) {
		insert_aux(position, n, val, typename __is_integer<size_type>::is_integer());
	}

	template<typename T>
	template<typename InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, typename __is_integer<InputIterator>::is_integer());
	}

} //namespace TinySTL
#endif // !LIST_H

