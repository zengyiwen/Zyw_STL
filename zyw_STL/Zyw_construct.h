#pragma once
#include<new>
#include"Zyw_type_traits.h"
#include "Zyw_iterator.h"


	//----------���칤��
	template<class T1, class T2>
	//��p��ָ�����ڴ������Ϲ��캯��
	inline void construct(T1*p, const T2&value) {
		new(p) T1(value);//placement new;����T1::T1(value);
	}

	//--------��������
	template<class T>
	inline void destroy(T *pointer)
	{
		pointer->~T();
	} 
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {

		__destroy(first, last, first);
		//value_type�ҳ��������Ƿ���IS_POD
	}

	template<class ForwardIterator, class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
		//�ж��Ƿ������õĹ��캯��
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destrutor;
		__destroy_aux(first, last, trivial_destrutor());
	}
	//������Զ��������ͺ���
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first != last; ++first)
			destroy(&*first);
	}
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

	//---------------Destroy�ػ��汾
	inline void destroy(char *, char *) {}
	inline void destroy(wchar_t*, wchar_t*) {}

