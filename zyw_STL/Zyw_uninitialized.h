#pragma once
#define D_SCL_SECURE_NO_WARNINGS
#include"Zyw_construct.h"
#include<algorithm>
#include"Zyw_type_traits.h"
#include<cstring>


namespace Zyw
{


	template<class InputIterator, class ForwardIterator>
	//��[first,last]��Χ�ڵ�ÿһ�������Ƶ�result��
	ForwardIterator
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{           //���һ��������Ϊ��ȡ����������Ӧ������,���ж��Ƿ����������ͻ����û��Զ�������
		return  __uninitialized_copy(first, last, result, (result));
	}

	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
		__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;   //POD��ʾC++��������
		return __uninitialized_copy_aux(first, last, result, is_POD());

	}
	//���������is_pod����(��������)��ôֱ�ӵ���copy����
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last
			, ForwardIterator result, __true_type) {
		return  std::copy(first, last, result);


	}
	//����������ͻ��������û��Զ�������,Ȼ��һ��һ����������
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last
			, ForwardIterator result, __false_type) {

		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}


	//�ػ��汾
	inline char*uninitialized_copy(const char*first, const char*last, char*result)
	{
		memmove(result, first, last - first);
	}


	//��[first,first+n]֮��δ��ʼ������
	template<class ForwardIterator, class Size, class T>
	ForwardIterator
		uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{    //�����ж��Ƿ���pod,pod���ͱ�ʾ����ϵͳ�����,�����Լ����������
		return __uninitialized_fill_n(first, n, x, first);

	}
	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T&x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return  __uninitialized_fill_n_aux(first, n, x, is_POD());
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator
		__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&x, __true_type) {
		std::fill_n(first, n, x);
		return first;
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator
		__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&x, __false_type) {
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, x);
		return cur;
	}




	//first��last������֮���ڵ��ô˺���֮ǰ����δ��ʼ����
	//�˺�����Ŀ�İ�first��last֮��ĵ�������ָ�ĵĵط���ʼ������ֵΪx
	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T&x)
	{
		__uninitialized_fill(first, last, x, value_type(first));
	}

	template<class ForwardIterator, class T, class T1>

	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator, const T&x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		__uninitialized_fill_aux(first, last, x, is_POD());
	}


	template<class ForwardIterator, class T>
	void
		__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T&x, __false_type)
	{

		ForwardIterator cur = first;
		for (; cur != last; ++cur)
			construct(&*cur, x);
	}

	template<class ForwardIterator, class T>
	void
		__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T&x, __true_type)
	{

		fill(first, last, x);
	}



}
