/*********************************************************************
名称：algobase.h
功能：包含了STL中的基本算法
*********************************************************************/
#ifndef ALGOBASE_H
#define ALGOBASE_H

#include"../fundation/utility.h"
#include<algorithm>


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
		以“字典排列方式”对两个序列[_First1,_Last1)和[_First2,_Last2)进行比较。
		也就是第一序列以字典排列方式而言不小于第二序列。
	函数说明：
		比较操作针对两序列中的对应位置上的元素进行，并持续到
		(1)某一组元素彼此不相等
		(2)同时到达_Last1和_Last2
		(3)到达_Last1或_Last2
		返 回 值：
		(1)如果第一序列的元素较小，返回true。否则返回false
		(2)如果到达_Last1而尚未到达_Last2，返回true
		(3)如果到达_Last2而尚未到达_Last1,返回false
		(4)如果同时到达_Last1和_Last2，返回false
	******************************************************/
	template<class _InIt1,
		class _InIt2>
		bool lexicographical_compare(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2, _InIt2 _Last2)
	{
		for (; _First1 != _Last1 && _First2 != _Last2; ++_First1, ++_First2)
		{	//任何一个序列到达尾部，就结束。否则两序列就相对应一一进行对比
			if (*_First1 < *_First2)
				return true;
			if (*_First1 > *_First2)
				return false;
		}
		return _First1 == _Last1 && _First2 != _Last2;/*第一序列到达尾部而第二序列尚有余额*/
	}

	//使用函数对象作为比较准则，进行重载
	template<class _InIt1,
		class _InIt2,
		class _Pr>
		bool lexicographical_compare(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2, _InIt2 _Last2, _Pr _Pred)
	{
		for (; _First1 != _Last1 && _First2 != _Last2; ++_First1, ++_First2)
		{	//任何一个序列到达尾部，就结束。否则两序列就相对应一一进行对比
			if (_Pred(*_First1,*_First2))
				return true;
			if (_Pred(*_First2,*_First1))
				return false;
		}
		return _First1 == _Last1 && _First2 != _Last2;/*第一序列到达尾部而第二序列尚有余额*/

	}

	/**************************************************************************
	函 数 名：mismatch
	函数作用：用来平行比较两个序列，指出两者之间的第一个不匹配点
	返 回 值：返回一对迭代器，分别指向两序列中不匹配的点
	注 意 点：第二序列元素个数需 >= 第一序列
	**************************************************************************/



} //namespace TinySTL
#endif // !ALGOBASE_H

