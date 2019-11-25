/*****************************************************************************
���ƣ�list.h
ע�⣺
	������˫��������Ϊ������ǰ�պ�ԭ��������Ԫ���
	������Ϊ��ʱ��ֻ����Ԫ�����Ҫ��ʼ����Ԫ����ָ�룬�������׳���Ұָ��Ĳ���Ԥ����Ϊ
	��node_ָ����Ԫ��㣬	node_->next ��ʾͷ���  
											node_�����ʾβ 
											node_->pre��ʾ���һ�����
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
	
	/**********************************************************list������******************************************************/
	template<class T>
	class list_node_base
	{
	public:
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
		
			//ָ����
		base_ptr prev;
		base_ptr next;

		list_node_base() = default;

		base_ptr self() {
				//�����list_node_base����ĵ�ַ ת����һ��ָ�����͵ĵ�ַ
			return static_cast<base_ptr>(&*this);
		}
		void unlink() {
				//�ս����βָ�붼ָ���Լ�
			prev = next = self();
		}
		node_ptr as_node() {
				//����ָ��ת����������ָ�룬ʹ����Է��������������ĳ�Ա
			return static_cast<node_ptr>(self());
		}
	};

	template<class T>
	class list_node : public list_node_base<T>
	{
	public:
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
			
			//ֵ��
		T value;

		list_node() = default;
		list_node(const T&val) : value(val) {}

		node_ptr self() {
			return static_cast<node_ptr>(&*this);
		}
		base_ptr as_base() {
			//������ת���ɻ���ָ�룬ȥ����ָ��value������
			return static_cast<base_ptr>(&*this);
		}
	};

	/**********************************************************list���������*****************************************************/
	//const�������ͷ�const���������޷�ֱ����ǿ��ת��ʵ�֡���Ϊ�ڲ������ǲ�ͬ��

	template<class  T>
	struct list_iterator : public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T> {
		typedef T		value_type;
		typedef T*	pointer;
		typedef T&	reference;
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;
		typedef list_iterator<T>	self;

		base_ptr node_;	//ָ����

		//���캯��
		list_iterator() = default;
		list_iterator(base_ptr x) : node_(x) {}
		list_iterator(node_ptr x): node_(x->as_base()){}
		//copy���캯��
		list_iterator(const list_iterator<T>& rhs)  : node_(rhs.node_) {}

		//���ز�����
		reference operator*() const { return  node_->as_node()->value; }
		pointer		operator->() const { return &(operator*()); }
		
		self& operator++() {	//ǰ��++
			STL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}

		self operator++(int) {	//����++
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

		//���رȽ������
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
		//copy���캯��
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

		// ���رȽϲ�����
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
	};

	/**********************************************************list�����ṹ���******************************************************/
	template<class T>
	class list {
	public:
		typedef	TinySTL::allocator<T>									allocator_type;		//������ȡallocator��һЩ�ͱ���
		typedef	TinySTL::allocator<T>									data_allocator;		//�����ڽ���ֵ�� �������
		typedef	TinySTL::allocator<list_node<T>>				node_allocator;	//���롢����list_node���
		typedef	TinySTL::allocator<list_node_base<T>>		base_allocaror;		//���롢����list_node_base���

		//list��Ƕ�ͱ���
		typedef typename allocator_type::value_type			value_type;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::reference				reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::size_type				size_type;
		typedef typename allocator_type::difference_type		difference_type;

		//����������
		typedef list_iterator<T>					iterator;
		typedef list_const_iterator<T>		const_iterator;
		typedef TinySTL::reverse_iterator<iterator>				reverse_iterator;
		typedef TinySTL::reverse_iterator<const_iterator>		const_reverse_iterator;

		//�ڲ����ָ�붨��
		typedef list_node_base<T>*	base_ptr;
		typedef list_node<T>*			node_ptr;

	private:
		base_ptr	node_;	//ָ�� Ϊ������ǰ�պ�ԭ�����õĽ�� ��ָ��
		size_type	size_;	//���������

	private:
		//helper function

		//����������ٽ��
		node_ptr create_node(const value_type&val);
		void destory_node(node_ptr p);
	
		//��ʼ������
		void fill_init(size_type n, const value_type& val = value_type());
		template<class Iter>
		void copy_init(Iter fistr, Iter last);

		/*���빹��
		�����½��ʱ���������½���ѡ�����λ�ã���ͬλ�ò��뵼�����ӷ�ʽҲ��һ��
		�����Զ����½��ֵ��Ҳ���Ը���һ�����е�ֵ���н�㴴��
		*/
		iterator link_one_node(const_iterator position, base_ptr node);		//��position������nodeָ��Ľ�㡣����node���λ��
		void link_node(base_ptr p, base_ptr first, base_ptr last);					//��p������  [first,last����һ�ν��
		void link_node_at_front(base_ptr first, base_ptr last);							//������ͷ����[first��last����ν��
		void link_node_at_back(base_ptr first, base_ptr last);							//������β������[first��last)��ν��
		void unlink_node(base_ptr first, base_ptr last);									//�������жϿ�[first,last)��ν��

		iterator fill_insert(const_iterator pos, size_type n, const value_type& val);	//�Զ���ֵ��λ�á������������½���
		template<class Iter>
		iterator copy_insert(const_iterator pos, size_type n, Iter first);		//ȡ����[first��last����ÿ��Ԫ�ص�ֵ��Ϊvalue���н�㹹�죬Ȼ�����
	

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
		//�����졢�������
		list();
		list(size_type n);
		list(size_type n, const value_type& val);
		template<typename InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& other);
		~list();

		/********************************************************************************/
		//���������
		iterator begin() { return node_->next; }
		iterator end() { return node_; }
		const_iterator begin() const { return	 node_->next; }
		const_iterator end() const { return node_; }

		const_iterator cbegin()  const { return begin(); }
		const_iterator cend()  const { return end(); }

		/********************************************************************************/
		//����������
		bool empty() { return node_->next == node_; }
		size_type size() const { return size_; }
		void resize(size_type _Newsize);
		void resize(size_type _Newsize, const value_type& val);

		/********************************************************************************/
		//Ԫ�ط������
		reference front() { STL_DEBUG(!empty()); return *begin(); }
		reference back() { STL_DEBUG(!empty()); return *(--end()); }

		/********************************************************************************/
		//Ԫ���޸�������
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
	//helper function ʵ��
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
		node_->unlink();	//���ûд����Ԫ���node_��ָ�����Ұָ��
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
		first->prev->next = first;		 //���node_֮ǰû�н�㣬�����������þ���first=first��
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
	//�����졢�������
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
		//��ͨ������Ԫ���ֿ�����
		if (node_) {
			clear();
			base_allocaror::deallocate(node_);
			node_ = nullptr;
			size_ = 0;
		}
	}

	/********************************************************************************/
	//����������
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
				pop_back();	//ע��pop_back����Ҳ���size_���м�һ
			size_ = _Newsize;
		}
	}

	/********************************************************************************/
	//Ԫ���޸�������
	template<class T>
	void list<T>::clear() {
		if (size_ !=  0) {
			base_ptr cur = node_->next;
			for (base_ptr next = cur->next; cur != node_; cur = next, next = next->next) {
				destory_node(cur->as_node());
			}
		}
		//�����ϣ�ʣ����Ԫ���
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

