/*********************************************************************
���ƣ�algobase.h
���ܣ�������STL�еĻ����㷨
*********************************************************************/
#ifndef ALGOBASE_H
#define ALGOBASE_H

#include"../fundation/utility.h"


namespace TinySTL {

	/******************************************************
	�� �� ����max
	�������ã����ض����и����һ��
	******************************************************/
	template<class _Ty>
	const _Ty& max(const _Ty& _Left, const _Ty& _Right)
	{
		return _Left < _Right ? _Right : _Left;
	}

	template<class _Ty,
		class _Pr>
		const _Ty& max(const _Ty& _Left, const _Ty& _Right, _Pr _Pred)
	{	//��_Pred��������С�Ƚϡ���׼
		return (_Pred(_Left, _Right)) ? _Right : _Left;
	}

	/******************************************************
	�� �� ����min
	�������ã����ض����и�С��һ��
	******************************************************/
	template<class _Ty>
	const _Ty& min(const _Ty& _Left, const _Ty& _Right)
	{
		return _Right < _Left ? _Right : _Left;
	}

	template<class _Ty,
		class _Pr>
		const _Ty& min(const _Ty& _Left, const _Ty& _Right, _Pr _Pred)
	{	//��_Pred��������С�Ƚϡ���׼
		return (_Pred(_Right, _Left)) ? _Right : _Left;
	}

	/******************************************************
	�� �� ����iter_swap
	�������ã�����������������ָ����
	******************************************************/
	template<class _FowIt1,
		class _FowIt2> inline
		void iter_swap(_FowIt1 _Left, _FowIt2 _Right)
	{	//����������������ָ����
		TinySTL::swap(*_Left, *_Right);
		//typename iterator_traits<_FowIt1>::value_type _Tmp = *_Left;
		//*_Left = *_Right;
		//*_Right = _Tmp;
	}

	/******************************************************
	�� �� ����equal
	�������ã��ж�����������[_First,_Last)������ȡ���ȷ���true
	ע �� �㣺�ڶ�����Ԫ�ظ��� >= ��һ����
	******************************************************/
	template<class _InIt1,
		class _InIt2> inline
		bool equal(_InIt1 _First1, _InIt1 _Last1,
			_InIt2 _First2)
	{
		for (; _First1 != _Last1; ++_First1, ++_First2)
		{	//����ڶ����и��̣�����ʱ����ɲ���Ԥ��Ľ��
			if (*_First1 != *_First2)
				return false;
			return true;
		}
	}

	/******************************************************
	�� �� ����fill
	�������ã���[_First,_Last)�ڵ�����Ԫ�ظ�����ֵ
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
	�� �� ����fill_n
	�������ã�������[_First��_First+n�����Ϊ��ֵ
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
	�� �� ����lexicographical_compare
	�������ã�
	�� �� ֵ������˵��������ȥ����һ��
	******************************************************/


} //namespace TinySTL
#endif // !ALGOBASE_H

