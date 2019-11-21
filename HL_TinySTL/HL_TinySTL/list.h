/*****************************************************************************
���ƣ�list.h
ע�⣺
	������˫��������Ϊ������ǰ�պ�ԭ��������Ԫ���
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
	class list_iterator : public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T> {
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
		list_iterator(const list_iterator& rhs) : node_(rhs.node_) {}

		//���ز�����
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

		//���رȽ������
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

		// ���رȽϲ�����
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
	};

	/**********************************************************list�����ṹ���******************************************************/
	template<class T>
	class list {
	public:
		typedef	TinySTL::allocator<T>									allocator_type;		//������ȡallocator��һЩ�ͱ���
		typedef	TinySTL::allocator<T>									data_allocator;		//���ڶ�����
		typedef	TinySTL::allocator<list_node<T>>				node_allocator;	//����list_node���
		typedef	TinySTL::allocator<list_node_base<T>>		base_allocaror;		//����list_node_base��㣬��Ϊ��Ԫ���

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
		node_ptr create_node(value_type&val);
		void destory_node(node_ptr p);
	
		//��ʼ������
		void fill_init(size_type n, const value_type& val);
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
		iterator copy_insert(const_iterator pos, size_type n, Iter first);			//ȡ����[first��last����ÿ��Ԫ�ص�ֵ��Ϊvalue���н�㹹�죬Ȼ�����
	
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
		iterator begin();
		iterator end();

		/********************************************************************************/
		//����������
		bool empty();
		size_type size() const;
		void resize(size_type _Newsize);
		void resize(size_type _Newsize, const value_type& val);

		/********************************************************************************/
		//Ԫ�ط������
		reference front();
		reference back();

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
	typename list<T>::node_ptr	list<T>::create_node(value_type& val) {
		node_ptr node = node_allocator::allocate(1);
		data_allocator::construct(&node->value, val);
		node->prev = nullptr;
		node->next = nullptr;
		return node;
	}

	template<class T>
	void list<T>::fill_init(size_type n, const value_type&val) {
		base_ptr Fn = base_allocaror::allocate(1);	//Fn������Ԫ���
		Fn->unlink();
		size_ = n;
		if (n == 0) return Fn;
		for (; n > 0; --n) {
			node_ptr new_node = create_node(val);
			link_node_at_back(new_node->as_base(), new_node->as_base());
		}
	}
	
	template<class T>
	template<class Iter>
	void list<T>::copy_init(Iter first, Iter last) {
		size_type n = TinySTL::distance(first, last);
		size_ = n;
		base_ptr Fn = base_allocaror::allocate(1);
		Fn->unlink();
		for (; n > 0; n--, ++first) {
			node_ptr  new_node = create_node(*first);
			link_node_at_back(new_node->as_base(), new_node->as_base());
		}
	}

	template<class T>
	typename list<T>::iterator list<T>::link_one_node(const_iterator pos, base_ptr node) {
		if (pos == node_->next){
			link_node_at_front(node, node);
		}
		else if (pso == node_) {
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
		node_->next = first;
		node_->next->prev = last;
		first->prev = node_;
	}

	template<class T>
	void list<T>::link_node_at_back(base_ptr first, base_ptr last) {
		link_node(node_, first, last);
	}

	template<class T>
	void list<T>::unlink_node(base_ptr first, base_ptr last) {
		first->prev->next = last->next;
		last->next->prev = first->prev;
	}

	template<class T>
	typename list<T>::iterator list<T>::fill_insert(const_iterator pos, size_type n, const value_type& val) {
		size_ += n;
		node_ptr new_node = create_node(val);
		for (; n > 0; n--) {
			link_one_node(pos, new_node->as_base());
		}
	}

	template<class T>
	template<class Iter>
	typename list<T>::iterator 
		list<T>::copy_insert(const_iterator pos, size_type n, Iter first) {
		size_type add_size = n;
		size_ += add_size;
		for (; n > 0; n--,++first) {
			node_ptr new_node = create_node(*first);
			link_one_node(pos, new_node->as_base());
		}
	}


} //namespace TinySTL
#endif // !LIST_H

