/*********************************************************************
名称：algobase.h
功能：包含了STL中的基本算法
*********************************************************************/
#ifndef ALGOBASE_H
#define ALGOBASE_H

#include"../fundation/utility.h"


namespace TinySTL {

	/******************************************************
	函 数 名：max
	函数作用：返回二者中更大的一方
	******************************************************/
	template<class _Ty>
	const _Ty& max(const _Ty& _Left, const _Ty& _Right)
	{
		return _Left < _Right ? _Right : _Left;
	}

	template<class _Ty,
		class _Pr>
		const _Ty& max(const _Ty& _Left, const _Ty& _Right, _Pr _Pred)
	{	//由_Pred决定“大小比较”标准
		return (_Pred(_Left, _Right)) ? _Right : _Left;
	}

	/******************************************************
	函 数 名：min
	函数作用：返回二者中更小的一方
	******************************************************/
	template<class _Ty>
	const _Ty& min(const _Ty& _Left, const _Ty& _Right)
	{
		return _Right < _Left ? _Right : _Left;
	}

	template<class _Ty,
		class _Pr>
		const _Ty& min(const _Ty& _Left, const _Ty& _Right, _Pr _Pred)
	{	//由_Pred决定“大小比较”标准
		return (_Pred(_Right, _Left)) ? _Right : _Left;
	}

	/******************************************************
	函 数 名：iter_swap
	函数作用：交换两个迭代器所指对象
	******************************************************/
	template<class _FowIt1,
		class _FowIt2> inline
		void iter_swap(_FowIt1 _Left, _FowIt2 _Right)
	{	//交换两个迭代器所指对象
		TinySTL::swap(*_Left, *_Right);
		//typename iterator_traits<_FowIt1>::value_type _Tmp = *_Left;
		//*_Left = *_Right;
		//*_Right = _Tmp;
	}

	/******************************************************
	函 数 名：equal
	函数作用：判断两个序列在[_First,_Last)区间相等。相等返回true
	注 意 点：第二序列元素个数 >= 第一序列
	******************************************************/
	template<class _InIt1,
		class _InIt2> inline
		bool equal(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2)
	{
		for (; _First1 != _Last1; ++_First1, ++_First2)
		{	//如果第二序列更短，迭代时会造成不可预测的结果
			if (*_First1 != *_First2)
				return false;
			return true;
		}
	}

	/******************************************************
	函 数 名：fill
	函数作用：将[_First,_Last)内的所有元素改填新值
	******************************************************/
	template<class _FowIt,
		class _Ty>
		void fill(_FowIt _First, _FowIt _Last, _Ty _Val)
	{
		for (; _First != _Last; ++_First)
		{
			*_First = _Val;
		}
	}


	/******************************************************
	函 数 名：fill_n
	函数作用：将区间[_First，_First+n）填充为新值
	******************************************************/
	template<class _OutIt,
		class Size,
		class _Ty>
		_OutIt fill_n(_OutIt _First, Size n, const _Ty& _Val)
	{
		for (; n > 0; --n, ++_First)
			*_First = _Val;
		return _First;
	}

	/******************************************************
	函 数 名：lexicographical_compare
	函数作用：
	返 回 值：有则说明，无则去除这一行
	******************************************************/


} //namespace TinySTL
#endif // !ALGOBASE_H

