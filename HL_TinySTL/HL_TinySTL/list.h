/*****************************************************************************
名称：list.h
注意：
链表是双向环形链表，为了满足前闭后开原则，增设首元结点
当链表为空时，只有首元结点需要初始化首元结点的指针，否则容易出现野指针的不可预估行为
用_Node指向首元结点，	_Node->_Next 表示头结点  
										_Node本身表示尾 
										_Node->pre表示最后一个结点
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

template<class T> class _List_node_base;
template<class T> class _List_node;
	
/**********************************************************list结点设计******************************************************/
template<class T>
class _List_node_base
	{
public:
	typedef _List_node_base<T>*	_Base_ptr;
	typedef _List_node<T>*			_Node_ptr;
		
	//指针域
	_Base_ptr _Prev;
	_Base_ptr _Next;

	_List_node_base() = default;

	_Base_ptr self()
	{	//将这个_List_node_base对象的地址 转换成一个指针类型的地址
		return static_cast<_Base_ptr>(&*this);
	}
	void unlink() 
	{	//空结点首尾指针都指向自己
		_Prev = _Next = self();
	}
	_Node_ptr as_node() 
	{	//基类指针转换成派生类指针，使其可以访问派生类新增的成员
		return static_cast<_Node_ptr>(self());
	}
};

template<class T>
	class _List_node
		: public _List_node_base<T>
	{
public:
	typedef _List_node_base<T>*	_Base_ptr;
	typedef _List_node<T>*			_Node_ptr;
			
	//值域
	T _Myval;

	_List_node() = default;
	_List_node(const T&_Val)
		:  _Myval(_Val)
	{
	}

	_Node_ptr self() 
	{
		return static_cast<_Node_ptr>(&*this);
	}
	_Base_ptr as_base() 
	{	//派生类转换成基类指针，去除了指向_Myval的能力
		return static_cast<_Base_ptr>(&*this);
	}
};

/**********************************************************list迭代器设计*****************************************************/
//const迭代器和非const迭代器，无法直接用强制转换实现。因为内部操作是不同的

template<class  T>
	struct _List_iterator
		: public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T> 
	{
	typedef T		value_type;
	typedef T*	pointer;
	typedef T&	reference;
	typedef _List_node_base<T>*	_Base_ptr;
	typedef _List_node<T>*			_Node_ptr;
	typedef _List_iterator<T>	self;

	_Base_ptr _Node;	//指向结点

	//构造函数
	_List_iterator() = default;
	_List_iterator(_Base_ptr __Base_ptr)
		: _Node(__Base_ptr) 
	{
	}
	_List_iterator(_Node_ptr __Node_ptr)
		: _Node(__Node_ptr->as_base())
	{
	}
	_List_iterator(const _List_iterator<T>& rhs) 
		: _Node(rhs._Node) 
	{	//拷贝构造函数
	}

	//重载操作符
	reference operator*() const { return  _Node->as_node()->_Myval; }
	pointer		operator->() const { return &(operator*()); }
		
	self& operator++()
	{	//前置++
		STL_DEBUG(_Node != nullptr);
		_Node = _Node->_Next;
		return *this;
	}

	self operator++(int)
	{	//后置++
		self _Tmp = *this;
		++*this;
		return _Tmp;
	}

	self& operator--()
	{
		STL_DEBUG(_Node != nullptr);
		_Node = _Node->_Prev;
		return *this;
	}

	self operator--(int)
	{
		self _Tmp = *this;
		--*this;
		return _Tmp;
	}

	//重载比较运算符
	bool operator ==(const self& rhs) const { return _Node == rhs._Node; }
	bool operator !=(const self& rhs) const { return _Node != rhs._Node; }
};

template <class T>
	struct _List_const_iterator
		: public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, T>
	{
	typedef T												value_type;
	typedef const T*									pointer;
	typedef const T&								reference;
	typedef _List_node_base<T>*				_Base_ptr;
	typedef _List_node<T>*						_Node_ptr;
	typedef _List_const_iterator<T>			self;

	_Base_ptr _Node;

	_List_const_iterator() = default;
	_List_const_iterator(_Base_ptr __Base_ptr)
		: _Node(__Base_ptr)
	{
	}
	_List_const_iterator(_Node_ptr __Node_ptr)
		: _Node(__Node_ptr->as_base())
	{
	}
	_List_const_iterator(const _List_iterator<T>& rhs)
		: _Node(rhs._Node)
	{
	}

	_List_const_iterator(const _List_const_iterator<T>& rhs)
		: _Node(rhs._Node)
	{	//拷贝构造函数
	}

	reference operator*()  const { return _Node->as_node()->_Myval; }
	pointer   operator->() const { return &(operator*()); }

	self& operator++()
	{
		STL_DEBUG(_Node != nullptr);
		_Node = _Node->_Next;
		return *this;
	}

	self operator++(int)
	{
		self _Tmp = *this;
		++*this;
		return _Tmp;
	}

	self& operator--()
	{
		STL_DEBUG(_Node != nullptr);
		_Node = _Node->_Prev;
		return *this;
	}

	self operator--(int)
	{
		self _Tmp = *this;
		--*this;
		return _Tmp;
	}

	// 重载比较操作符
	bool operator==(const self& rhs) const { return _Node == rhs._Node; }
	bool operator!=(const self& rhs) const { return _Node != rhs._Node; }
};

/**********************************************************list容器结构设计******************************************************/
template<class T>
	class list
{
public:
	typedef	TinySTL::allocator<T>										_Allocator_type;		//用于提取allocator的一些型别定义
	typedef	TinySTL::allocator<T>										_Data_allocator;		//用于在结点的值域处 构造对象
	typedef	TinySTL::allocator<_List_node<T>>				_Node_allocator;		//申请、销毁_List_node结点
	typedef	TinySTL::allocator<_List_node_base<T>>		_Base_allocator;		//申请、销毁_List_node_base结点

	//内嵌型别定义
	typedef typename _Allocator_type::value_type				value_type;
	typedef typename _Allocator_type::pointer					pointer;
	typedef typename _Allocator_type::const_pointer			const_pointer;
	typedef typename _Allocator_type::reference				reference;
	typedef typename _Allocator_type::const_reference		const_reference;
	typedef typename _Allocator_type::size_type					size_type;
	typedef typename _Allocator_type::difference_type		difference_type;

	//迭代器定义
	typedef _List_iterator<T>											iterator;
	typedef _List_const_iterator<T>									const_iterator;
	typedef TinySTL::reverse_iterator<iterator>				reverse_iterator;
	typedef TinySTL::reverse_iterator<const_iterator>		const_reverse_iterator;

	//内部结点指针定义
	typedef _List_node_base<T>*	_Base_ptr;
	typedef _List_node<T>*			_Node_ptr;

private:
	_Base_ptr	_Node;	//指向 为了满足前闭后开原则设置的结点 的指针
	size_type	_Size;	//链表结点个数

private:
	//helper function

	//申请结点和销毁结点
	_Node_ptr create_node(const value_type&_Val);
	void destory_node(_Node_ptr __Node_ptr);
	
	//初始化构建
	void fill_init(size_type _Count, const value_type& _Val = value_type());
	template<class Iter>
	void copy_init(Iter fistr, Iter _Last);

	/*插入构建
	插入新结点时，需生成新结点和选择插入位置，不同位置插入导致链接方式也不一样
	可以自定义新结点值，也可以根据一段序列的值进行结点创建
	*/
	iterator link_one_node(const_iterator _Where, _Base_ptr __Base_ptr);		
	void link_node(_Base_ptr __Base_ptr, _Base_ptr _First, _Base_ptr _Last);					
	void link_node_at_front(_Base_ptr _First, _Base_ptr _Last);							
	void link_node_at_back(_Base_ptr _First, _Base_ptr _Last);						
	void unlink_node(_Base_ptr _First, _Base_ptr _Last);								

	iterator fill_insert(const_iterator _Where, size_type _Count, const value_type& _Val);	
	template<class Iter>
	iterator copy_insert(const_iterator _Where, size_type _Count, Iter _First);		
	

	template<class _InIt>
	void list_aux(_InIt _First, _InIt _Last, __false_type) 
	{
		copy_init(_First, _Last);
	}
	void list_aux(size_type _Count, const value_type& _Val, __true_type) 
	{
		fill_init(_Count, _Val);
	}
	template<class _InIt>
	void insert_aux(const_iterator _Where, _InIt _First, _InIt _Last, __false_type)
	{
		size_type _Count = 0;
		auto _Tmp = _First;
		for (; _Tmp != _Last;++_Tmp)
			_Count++;
			//distance函数没用，后续改进以后换成distance
		copy_insert(_Where, _Count, _First);
	}
	void insert_aux(const_iterator _Where, size_type _Count, const value_type & _Val, __true_type) 
	{
		fill_insert(_Where, _Count, _Val);
	}

public:
	/********************************************************************************/
	//对象构造、析构相关
	list();
	list(size_type _Count);
	list(size_type _Count, const value_type& _Val);
	template<typename _InIt>
	list(_InIt _First, _InIt _Last);
	list(const list& _Right);
	~list();

	/********************************************************************************/
	//迭代器相关
	iterator begin() { return _Node->_Next; }
	iterator end() { return _Node; }
	const_iterator begin() const { return	 _Node->_Next; }
	const_iterator end() const { return _Node; }

	const_iterator cbegin()  const { return begin(); }
	const_iterator cend()  const { return end(); }

	/********************************************************************************/
	//容量相关相关
	bool empty() { return _Node->_Next == _Node; }
	size_type size() const { return _Size; }
	void resize(size_type _Newsize);
	void resize(size_type _Newsize, const value_type& _Val);

	/********************************************************************************/
	//元素访问相关
	reference front() { STL_DEBUG(!empty()); return *begin(); }
	reference back() { STL_DEBUG(!empty()); return *(--end()); }

	/********************************************************************************/
	//元素修改相关相关
	void push_back(const value_type& _Val);
	void push_front(const value_type& _Val);
	void pop_back();
	void pop_front();

	iterator insert(iterator _Where, const value_type& _Val);
	void insert(iterator _Where, size_type _Count, const value_type& _Val);
	template<typename _InIt>
	void insert(iterator _Where, _InIt _First, _InIt _Last);

	void splice(const_iterator _Where, list&_Right);
	void splice(const_iterator _Where, list& _Right, const_iterator _First);
	void splice(const_iterator _Where, list& _Right, const_iterator _First, const_iterator _Last);

	iterator erase(const_iterator _Where);
	iterator erase(const_iterator _First, const_iterator _Last);

	void merge(list &_Right);
	template<typename Compare>
	void merge(list& _Right, Compare comp);

	void remove(const value_type& _Val);
	template<typename Predicate>
	void remove_if(Predicate pred);

	void unique();
	template<typename BinaryPredicate>
	void unqiue(BinaryPredicate);

	void clear();
	void reverse();
	void swap(list& _Right);
	void sort();
	template<typename Compare>
	void sort(Compare comp);
}; //end of  list class


/********************************************************************************/
//helper function 实现
template<class T>
typename list<T>::_Node_ptr	list<T>::create_node(const value_type& _Val) {
	/*
	函数功能：以传入参数为值，创建结点
	函数返回：返回指向该结点的指针
	*/
	_Node_ptr __Node_ptr = _Node_allocator::allocate(1);
	_Data_allocator::construct(&__Node_ptr->_Myval, _Val);
	__Node_ptr->_Prev = nullptr;
	__Node_ptr->_Next = nullptr;
	return __Node_ptr;
}

template<class T>
void list<T>::destory_node(_Node_ptr __Node_ptr) {
	/*
	函数功能：析构传入指针所指向的结点的值域
	函数返回：无
	*/
	_Data_allocator::destory(&__Node_ptr->_Myval);
	_Node_allocator::deallocate(__Node_ptr);
}

template<class T>
void list<T>::fill_init(size_type _Count, const value_type&_Val) {
	/*
	函数功能：初始化容器对象，创建_Count个值为_Val的结点
	函数返回：无
	*/
	_Node= _Base_allocator::allocate(1);
	_Node->unlink();	//如果没写，首元结点_Node的指针会变成野指针
	_Size = _Count;
	if (_Count == 0) return;
	for (; _Count > 0; --_Count) {
		_Node_ptr _New_node = create_node(_Val);
		link_node_at_back(_New_node->as_base(), _New_node->as_base());
	}
}
	
template<class T>
template<class Iter>
void list<T>::copy_init(Iter _First, Iter _Last) {
	/*
	函数功能：以[_First,_Last)区间内结点值作为新结点的值构造新结点
	函数返回：无
	*/
	_Node = _Base_allocator::allocate(1);
	_Node->unlink();
	//auto _Tmp = _First; 
	for (; _First != _Last; ++_First) {
		_Size++;
		_Node_ptr  _New_node = create_node(*_First);
		link_node_at_back(_New_node->as_base(), _New_node->as_base());
	}
}

template<class T>
typename list<T>::iterator list<T>::link_one_node(const_iterator _Where, _Base_ptr __Base_ptr) {
	/*
	函数功能：在_Where出链接一个新结点__Base_ptr
	函数返回：返回指向新结点的迭代器
	*/
	if (_Where == _Node->_Next){
		link_node_at_front(__Base_ptr, __Base_ptr);
	}
	else if (_Where == _Node) {
		link_node_at_back(__Base_ptr, __Base_ptr);
	}
	else
		link_node(_Where._Node, __Base_ptr, __Base_ptr);
	return __Base_ptr;
}

template<class T>
void list<T>::link_node(_Base_ptr __Base_ptr, _Base_ptr _First, _Base_ptr _Last) {
	/*
	函数功能：在__Base_ptr后面连接一段结点[_First,_Last)
	函数返回：无
	*/
	__Base_ptr->_Prev->_Next = _First;
	_Last->_Next = __Base_ptr;
	_First->_Prev = __Base_ptr->_Prev;
	__Base_ptr->_Prev = _Last;
}

template<class T>
void list<T>::link_node_at_front(_Base_ptr _First, _Base_ptr _Last) {
	/*
	函数功能：在首元结点_Node后面连接一段区间[_First,_Last)
	函数返回：无
	*/
	_Last->_Next = _Node->_Next;
	_Node->_Next->_Prev = _Last;
	_First->_Prev = _Node;
	_Node->_Next = _First;
}

template<class T>
void list<T>::link_node_at_back(_Base_ptr _First, _Base_ptr _Last) {
	/*
	函数功能：在_Node前连接一段区间[_First,_Last)
	函数返回：无
	*/
	_Last->_Next = _Node;
	_First->_Prev = _Node->_Prev;	 //
	_First->_Prev->_Next = _First;		 //如果_Node之前没有结点，这两句句的作用就是_First=_First；
	_Node->_Prev = _Last;
}

template<class T>
void list<T>::unlink_node(_Base_ptr _First, _Base_ptr _Last) {
	/*
	函数功能：将一段结点[_First,_Last)从容器中切出
	函数返回：无
	*/
	_First->_Prev->_Next = _Last->_Next;
	_Last->_Next->_Prev = _First->_Prev;
}

template<class T>
typename list<T>::iterator list<T>::fill_insert(const_iterator _Where, size_type _Count, const value_type& _Val) {
	/*
	函数功能：用于insert函数。创建_Count个值为_Val的结点，并接入_Where之后
	函数返回：返回一个指向最后插入的结点的迭代器
	*/
	_Size += _Count;
	iterator  r;
	for (; _Count > 0; _Count--) {
		_Node_ptr _New_node = create_node(_Val);
		r=link_one_node(_Where, _New_node->as_base());
	}
	return r;
}

template<class T>
template<class Iter>
typename list<T>::iterator
	list<T>::copy_insert(const_iterator _Where, size_type _Count, Iter _First) {
	/*
	函数功能：用于insert。以一段结点[_First,_First+_Count)的值为新结点值创建新结点，并接入_Where之后
	函数返回：返回一个指向最后插入链表的结点的迭代器
	*/
	size_type _Add_size = _Count;
	_Size += _Add_size;
	iterator  r;
	for (; _Count > 0; _Count--, ++_First) {
		_Node_ptr _New_node = create_node(*_First);
		r = link_one_node(_Where, _New_node->as_base());
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
list<T>::list(size_type _Count) {
	fill_init(_Count, value_type());
}

template<class T>
list<T>::list(size_type _Count, const value_type& _Val) {
	typedef typename __is_integer<size_type>::is_integer IS_INTEGER;
	list_aux(_Count, _Val, IS_INTEGER());
}

template<class T>
template<class _InIt>
list<T>::list(_InIt _First, _InIt _Last) {
	typedef typename __is_integer<_InIt>::is_integer IS_INTEGER;
	list_aux(_First,_Last, IS_INTEGER());
}

template<class T>
list<T>::list(const list& _Right) {
	copy_init(_Right.cbegin(), _Right.cend());
}

template<class T>
list<T>::~list() {
	//普通结点和首元结点分开销毁
	if (_Node) {
		clear();
		_Base_allocator::deallocate(_Node);
		_Node = nullptr;
		_Size = 0;
	}
}

/********************************************************************************/
//容量相关相关
template<class T>
void list<T>::resize(size_type _Newsize) {
	resize(_Newsize, value_type());
}

template<class T>
void list<T>::resize(size_type _Newsize, const value_type& _Val) {
	if (_Newsize > size()) {
		size_type _Add_size = _Newsize - _Size;
		while (_Add_size--)
			push_back(_Val);
		_Size = _Newsize;
	}
	else {
		size_type _Delete_size = size() - _Newsize;
		while (_Delete_size--)
			pop_back();	//注意pop_back本身也会对_Size进行减一
		_Size = _Newsize;
	}
}

/********************************************************************************/
//元素修改相关相关
template<class T>
void list<T>::clear() {
	if (_Size !=  0) {
		_Base_ptr cur = _Node->_Next;
		for (_Base_ptr _Next = cur->_Next; cur != _Node; cur = _Next, _Next = _Next->_Next) {
			destory_node(cur->as_node());
		}
	}
	//清除完毕，剩下首元结点
	_Node->unlink();
	_Size = 0;
}

template<class T>
void list<T>::push_back(const value_type& _Val) {
	_Node_ptr _New_node = create_node(_Val);
	link_node_at_back(_New_node->as_base(),_New_node->as_base());
	_Size++;
}

template<class T>
void list<T>::push_front(const value_type& _Val) {
	_Node_ptr  _New_node = create_node(_Val);
	link_node_at_front(_New_node->as_base(), _New_node->as_base());
	_Size++;
}

template<class T>
void list<T>::pop_back() {
	if ( !empty() ) {
		_Base_ptr _Tmp = _Node->_Prev;
		unlink_node(_Tmp, _Tmp);
		_Tmp->unlink();
		destory_node(_Tmp->as_node());
		_Size--;
	}
}

template<class T>
void list<T>::pop_front() {
	if( !empty() ){
		_Base_ptr _Tmp = _Node->_Next;
		unlink_node(_Tmp, _Tmp);
		_Tmp->unlink();
		destory_node(_Tmp->as_node());
		_Size--;
	}
}

template<class T>
typename list<T>::iterator
	list<T>::insert(iterator _Where, const value_type&_Val) {
	_Node_ptr _New_node = create_node(_Val);
	_Size++;
	return 	link_one_node(_Where, _New_node->as_base());
}

template<class T>
void list<T>::insert(iterator _Where, size_type _Count, const value_type& _Val) {
	typedef typename __is_integer<size_type>::is_integer	IS_INTEGER;
	insert_aux(_Where, _Count, _Val, IS_INTEGER());
}

template<class T>
template<class _InIt>
void list<T>::insert(iterator _Where, _InIt _First, _InIt _Last) {
	typedef typename __is_integer<_InIt>::is_integer	IS_INTEGER;
	insert_aux(_Where, _First, _Last, IS_INTEGER());
}


template<class T>
void list<T>::splice(const_iterator _Where, list& _Right) {
	/*
	函数功能：将整个链表_Right（不能是自身）合并到链表的_Where处。（掠夺式合并）
	函数返回：无
	*/
	if ( *this != _Right && !_Right.empty() ) {
		auto f = _Right._Node->_Next;
		auto l = _Right._Node->_Prev;
		_Right.unlink_node(f, l);
		link_node(_Where, f, l);

		_Size += _Right.size();
		_Right._Size = 0;
	}
}

template<class T>
void list<T>::splice(const_iterator _Where, list& _Right, const_iterator _First) {
	/*
	函数功能：将链表_Right[_First, _First+1）合并到_Where处（掠夺式合并）
	函数返回：无
	*/
}

template<class T>
void list<T>::splice(const_iterator _Where, list& _Right, const_iterator _First, const_iterator _Last) {
	/*
	函数功能：将链表_Right（可以是自身）的一段结点[_First，_Last)合并到_Where处。（掠夺式合并）
	函数返回：无
	*/
}

template<class T>
typename list<T>::iterator
	list<T>::erase(const_iterator _Where) {
	if ( _Where != cend() ) {
		_Base_ptr _Tmp = _Where._Node;
		_Base_ptr _Next = _Tmp->_Next;
		unlink_node(_Tmp, _Tmp);
		destory_node(_Tmp->as_node());
		--_Size;
		return iterator(_Next);
	}
}

template<class T>
typename list<T>::iterator
	list<T>::erase(const_iterator _First, const_iterator _Last) {
	for (; _First != _Last;++_First) {
		erase(_First);
	}
	return _Last._Node;
}

} //namespace TinySTL
#endif // !LIST_H

