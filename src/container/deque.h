/**************************************************************************************
名称：deque.h
注意：
***************************************************************************************/
#ifndef DEQUE_H
#define DEQUE_H

#include<deque>

#include"../fundation/iterator.h"
#include"../fundation/allocator.h"

namespace TinySTL {
/**************************************************************************************/
	//DEQUE PARAMETERS
#define _DEQUE_MAP_SIZE 8  /*minimum  map size, at least 1*/
template <class T>
	struct deque_buf_size
	{
		static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
	};

	//CLASS TEMPLATE _Deque_iterator
template<class _Ty, class _Ref, class _Ptr>
	struct _Deque_iterator
		: public TinySTL::iterator<TinySTL::bidirectional_iterator_tag, _Ty> 
	{	//继承以后下面的型别应该可以不用写了吧
	typedef _Ty			value_type;
	typedef _Ptr		pointer;
	typedef _Ref		reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	typedef _Ty*	value_pointer;
	typedef _Ty**	map_pointer;
	static const size_type buffer_size = deque_buf_size<_Ty>::value;

	typedef _Deque_iterator<_Ty, _Ty&, _Ty*>				iterator;
	typedef _Deque_iterator<_Ty, const _Ty&, const _Ty*>	const_iterator;
	typedef _Deque_iterator<_Ty, _Ty&, _Ty*>				self;

	//以下四个成员构成一个迭代器
	value_pointer		_Cur;	//指向所属缓冲区的当前元素
	value_pointer		_First;	//指向所属缓冲区的头部
	value_pointer		_Last;	//指向所属缓冲区的尾部
	map_pointer			_Node;	//指向map，表示缓冲区所在的结点

	//构造、复制、移动函数
	_Deque_iterator() = default;

	_Deque_iterator(const iterator& _Right)
		: _Cur(_Right._Cur),
		_First(_Right._First),
		_Last(_Right._Last),
		_Node(_Right._Node)
	{	//拷贝构造函数
	}
	
	_Deque_iterator(const const_iterator& _Right)
		: _Cur(_Right._Cur),
		_First(_Right._First),
		_Last(_Right._Last),
		_Node(_Right._Node)
	{	//拷贝构造函数
	}

	_Deque_iterator(iterator && _Right)
		: _Cur(_Right._Cur),
		_First(_Right._First),
		_Last(_Right._Last),
		_Node(_Right._Node)
	{	//移动函数
		_Right._Cur = nullptr;
		_Right._First = nullptr;
		_Right._Last = nullptr;
		_Right._Node = nullptr;
	}

	self& operator=(const iterator& _Right) 
	{	//复制迭代器
		if (this != &_Right) {
			_Cur = _Right._Cur;
			_First = _Right._First;
			_Last = _Right._Last;
			_Node = _Right._Node;
		}
		return *this;
	}

	//重载运算符
	reference operator*() const { return*_Cur; }
	pointer operator->() const { return _Cur; }

	self& operator++()
	{	//++前置
		++_Cur;
		if (_Cur == _Last) {
			skip_node(_Node+1);
			_Cur = _First;
		}
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
		--_Cur;
		if (_Cur == _First) {
			skip_node(_Node - 1);
			_Cur = _Last;
		}
		return *this;
	}

	self operator--(int) 
	{
		self _Tmp = *this;
		--*this;
		return _Tmp;
	}

	self& += (difference_type n)
	{	//向前跳n个元素（如果n为负，则向后跳）
		difference_type _Off = n + (_Cur - _First);
		if (_Off >= 0
			&& _Off < difference_type(buffer_size()))
		{	//目标位置任然在同一个缓冲区内
			_Cur += n;
		}
		else
		{	//目标位置不在同一个缓冲区
			difference_type _Node_offset = _Off > 0
				? _Off / difference_type(buffer_size())
				: -difference_type((-_Off - 1) / buffer_size()) - 1;
			skip_node(_Node + _Node_offset);	/*确定正确的结点位置*/
			_Cur = _First + (_Off - _Node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n)const
	{	//用赋值形式（op=）取代其单独形式（op）
		self _Tmp = *this;
		return _Tmp += n;
	}

	self& operator-=(difference_type n)
	{	//利用operator+=来完成
		return *this += -n;
	}

	self operator -(difference_type n)const
	{	
		self _Tmp = *this;
		return _Tmp -= n;
	}

	reference operator[](difference_type n) 
	{	//调用operator*、operator+
		return *(*this + n);
	}	/*不做越界检查*/

	//重载比较操作符
	bool operator==(const self& _Right) const { return _Cur == _Right._Cur; }
	bool operator!=(const self& _Right) const { return !(this == _Right); }
	bool operator<(const self& _Right) const 
	{	//先判断缓冲区间是否相同，相同比_Cur，不同比_Node
		return _Node == _Right._Node ? (_Cur < _Right._Cur) : (_Node < _Right._Node);
	}
	bool operator>(const self& _Right) const { return !(*this < _Right); }
	bool operator<=(const self& _Right)const { return !(*this > _Right); }
	bool operator>=(const self& _Right)const { return !(*this < _Right); }


	void skip_node(map_pointer _Next_node)
	{	//更换结点(也就是更换新的缓冲区)
		_Node = _Next_node;
		_First = *_Next_node;
		_Last = _First + buffer_size;
	}

	}; //end of _Deque_iterator class
/**************************************************************************************/
	//CLASS TEMPLATE deque
template<class T>
class deque {
	typedef TinySTL::allocator<T>	_Allocator_type;	
	typedef TinySTL::allocator<T*>	_Map_allocator;			//用于申请map（map里面存的是指针）
	typedef TinySTL::allocator<T>	_Buffer_allocator;		//用于申请buffer的所需要的空间
	typedef TinySTL::allocator<T>	_Data_allocator;		//用于在buffer内创造对象

	//内嵌型别定义(用_Alloccator_type主要就是方便编写。也只可以直接定义typedef T value_type)
	typedef typename _Allocator_type::value_type			value_type;
	typedef typename _Allocator_type::pointer				pointer;
	typedef typename _Allocator_type::const_pointer			const_pointer;
	typedef typename _Allocator_type::reference				reference;
	typedef typename _Allocator_type::const_reference		const_reference;
	typedef typename _Allocator_type::size_type				size_type;
	typedef typename _Allocator_type::difference_type		difference_type;

	typedef pointer*		map_pointer;
	typedef const_pointer*	const_map_pointer;
	
	//迭代器定义
	typedef _Deque_iterator<T, T&, T*>							iterator;
	typedef _Deque_iterator<T, const T&, const T*>				const_iterator;
	typedef TinySTL::reverse_iterator<iterator>					reverse_iterator;
	typedef TinySTL::reverse_iterator<const_iterator>			const_reverse_iterator;

	//成员变量定义
	iterator _First;		//指向第一个节点
	iterator _Last;			//指向最后一个节点
	map_pointer _Map;		//指向中央控制区map
	size_type _Map_size;	//map内有多少个指针（用于map扩容）

private:
	//辅助函数声明

public:
	//成员函数声明
	_Allocator_type get_allocator() { return _Allocator_type(); }
	
	/********************************************************************************/
	//对象构造、析构、复制、移动
	deque();
	deque(size_type _Count);
	deque(size_type _Count, value_type _Val);
	template<class Iter>
	deque(Iter _First, Iter _Last);
	deque(const deque& _Right);
	deque(deque&& _Right);
	deque& operator=(const deque& _Right);
	deque& operator=(deque&& _Right);
	deque& operator=(std::initializer_list<value_type> _List);
	~deque();
	
	/********************************************************************************/
	//迭代器相关


	/********************************************************************************/
	//元素访问相关
	reference operator[](const size_type _Index);
	const_reference operator[](const size_type _Index);
	reference front();
	const_reference front();
	reference back();
	const_reference back();
	reference at(const size_type _Index);
	const_reference at(const size_type _Index);
	
	/********************************************************************************/
	//容量相关
	size_type size();
	size_type max_size();
	bool empty();
	void resize(size_type _New_size) { resize(_New_size, value_type()); }
	void resize(size_type _New_size, value_type _Val);
	void shrink_to_fit();
	
	/********************************************************************************/
	//修改容器相关
	void clear();
	iterator erase(iterator _Where);
	iterator erase(iterator _First, iterator _Last);
	void swap(deque& _Right);

	void assign(size_type _Count, value_type _Val);
	template<class Iter>
	void assign(Iter _First, Iter _Last);
	
	void push_back(value_type _Val);
	void push_front(value_type _Val);
	void pop_back();
	void pop_back();

	void insert(iterator _Where, size_type _Count, const value_type _Val);
	void insert(iterator _Where, iterator _First, iterator _Last);
	iterator insert(iterator _Where, const value_type _Val);
};	//end of deque class




} //namespace TinySTL
#endif // !DEQUE_H
