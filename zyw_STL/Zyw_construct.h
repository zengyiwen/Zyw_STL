#pragma once
#include<new>
#include"Zyw_type_traits.h"
#include "Zyw_iterator.h"


	//----------构造工具
	template<class T1, class T2>
	//在p所指定的内存区域上构造函数
	inline void construct(T1*p, const T2&value) {
		new(p) T1(value);//placement new;调用T1::T1(value);
	}

	//--------析构工具
	template<class T>
	inline void destroy(T *pointer)
	{
		pointer->~T();
	} 
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {

		__destroy(first, last, first);
		//value_type找出其类似是否是IS_POD
	}

	template<class ForwardIterator, class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
		//判断是否有无用的构造函数
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destrutor;
		__destroy_aux(first, last, trivial_destrutor());
	}
	//如果是自定义类类型函数
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first != last; ++first)
			destroy(&*first);
	}
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

	//---------------Destroy特化版本
	inline void destroy(char *, char *) {}
	inline void destroy(wchar_t*, wchar_t*) {}


