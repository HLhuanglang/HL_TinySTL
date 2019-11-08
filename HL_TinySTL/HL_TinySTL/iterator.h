/************************************************************
���ƣ�iterator.h
���ã���ȡ����������
*************************************************************/

#ifndef ITERATOR_H
#define ITERATOR_H
#include<cstddef> //Ϊ��ʹ��ptrdiff_t(һ���������ص��������ͣ�ptrdiff_t���ͱ���ͨ��������������ָ����������Ľ��)
namespace TinySTL {

	//�������ͱ�ǩ��������ȡ�ͺ�������
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//input_iterator
	template<class T, class Distance>
	struct input_iterator {
		typedef input_iterator_tag		iterator_category;
		typedef T									value_type;
		typedef Distance						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	//output_iterator
	template <class T, class Distance>
	struct output_iterator {
		typedef output_iterator_tag			iterator_category;
		typedef T											value_type;
		typedef Distance								difference_type;
		typedef T*										pointer;
		typedef T&										reference;
	};

	//forward_iterator
	template <class T, class Distance>
	struct forward_iterator {
		typedef forward_iterator_tag		iterator_category;
		typedef T										value_type;
		typedef Distance							difference_type;
		typedef T*									pointer;
		typedef T&									reference;
	};

	//bidirectional_iterator
	template <class T, class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T												value_type;
		typedef Distance									difference_type;
		typedef T*											pointer;
		typedef T&											reference;
	};

	//random_access_iterator
	template <class T, class Distance>
	struct random_access_iterator {
		typedef random_access_iterator_tag	iterator_category;
		typedef T												value_type;
		typedef Distance									difference_type;
		typedef T*											pointer;
		typedef T&											reference;
	};



	//���̳У��� class ListIterator�� public iterator<bidirectional_iterator_tag, T>
	template<class Category,
		class T,
		class Distance = ptrdiff_t,
		class Pointer = T * ,
		class Reference = T & >
		struct iterator {
		typedef Category		iterator_catagory;
		typedef T						value_type;
		typedef Distance			difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};

	//��ȡ����������
	//���ģ��Ϊ���������������Զ�����һ�ױ�׼�����ͱ��������㷨�ȿ����õ�������Ҳ������һ���ָ��
	template<typename Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category		iterator_category;
		typedef typename Iterator::value_type				value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer						pointer;
		typedef typename Iterator::reference					reference;
	};

	//���pointer ��pointer-to-const�����ػ�
	/*
		������ʵ�������Ҫ���ȫ���������������õ�ָ���ϣ���˶���ָ�룬iterator_category
		���ǵ�ͬ��random_access_iterator_tag
	*/
	template<typename T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag					iterator_category;
		typedef T																value_type;
		typedef ptrdiff_t													difference_type;
		typedef T*															pointer;
		typedef T&															reference;
	};

	template<typename T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag					iterator_category;
		typedef T																value_type;
		typedef ptrdiff_t													difference_type;
		typedef const T*													pointer;
		typedef const T&												reference;
	};

	//����������������������� category
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//�����������ĳ����������distance type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//�������������������value type
	template<typename Iterator>
	inline typename iterator_traits<Iterator>::value_type
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	//**********************************************************************************
	//reverse_iterator
	template<typename Iterator>
	class reverse_iterator {
	public:
		typedef typename iterator_traits<Iterator>::iterator_category		iterator_category;
		typedef typename iterator_traits<Iterator>::value_type					value_type;
		typedef typename iterator_traits<Iterator>::difference_type			difference_type;
		typedef typename iterator_traits<Iterator>::pointer						pointer;
		typedef typename iterator_traits<Iterator>::reference					reference;

		typedef Iterator																				iterator_type;
		typedef reverse_iterator<Iterator>													self;
	private:
		Iterator current; //�Ƕ�Ӧ�����������

	public:
		//���캯��
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) : current(i) {}
		reverse_iterator(const self& rhs) : current(rhs.current) {}

	public:
		//ȡ����Ӧ�����������
		iterator_type base() const {
			return current;
		}
		//���ز�����
		reference operator*() const {
			auto tmp = current;
			return *(--tmp);
		}
		pointer operator->() const {
			return &(operator*());
		}
		//ǰ��++ ��Ϊ����--
		self& operator++() {
			--current;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//����--��Ϊǰ��++
		self& operator--() {
			++current;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			++current;
			return tmp;
		}

		self&operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self operator+(difference_type n) const {
			return self(current - n);
		}
		self&operator-=(difference_type n) const {
			current += n;
			return *this;
		}

		self operator-(difference_type n) const {
			return self(current + n);
		}
		reference operator[](difference_type n)const {
			return *(*this + n);
		}

		// ���� operator-
		template <class Iterator>
		typename reverse_iterator<Iterator>::difference_type
			operator-(const reverse_iterator<Iterator>& lhs,
				const reverse_iterator<Iterator>& rhs)
		{
			return rhs.base() - lhs.base();
		}

		// ���رȽϲ�����
		template <class Iterator>
		bool operator==(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template <class Iterator>
		bool operator<(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return rhs.base() < lhs.base();
		}

		template <class Iterator>
		bool operator!=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(lhs == rhs);
		}

		template <class Iterator>
		bool operator>(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return rhs < lhs;
		}

		template <class Iterator>
		bool operator<=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(rhs < lhs);
		}

		template <class Iterator>
		bool operator>=(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs)
		{
			return !(lhs < rhs);
		}
	};

} //namespace TinySTL
#endif // !ITERATOR_H

