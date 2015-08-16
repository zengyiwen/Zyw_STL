#pragma once
#define D_SCL_SECURE_NO_WARNINGS
#include"Zyw_construct.h"
#include<algorithm>
#include"Zyw_type_traits.h"
#include<cstring>


namespace Zyw
{


	template<class InputIterator, class ForwardIterator>
	//将[first,last]范围内的每一个对象复制到result中
	ForwardIterator
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{           //最后一个参数是为了取出迭代器对应的类型,以判断是否是内置类型还是用户自定义类型
		return  __uninitialized_copy(first, last, result, (result));
	}

	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
		__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;   //POD表示C++内置类型
		return __uninitialized_copy_aux(first, last, result, is_POD());

	}
	//如果类型是is_pod类型(内置类型)那么直接调用copy函数
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last
			, ForwardIterator result, __true_type) {
		return  std::copy(first, last, result);


	}
	//如果是类类型或者其他用户自定义类型,然后一个一个拷贝构造
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		__uninitialized_copy_aux(InputIterator first, InputIterator last
			, ForwardIterator result, __false_type) {

		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
	}


	//特化版本
	inline char*uninitialized_copy(const char*first, const char*last, char*result)
	{
		memmove(result, first, last - first);
	}


	//将[first,first+n]之间未初始化区间
	template<class ForwardIterator, class Size, class T>
	ForwardIterator
		uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{    //用来判断是否是pod,pod类型表示的是系统定义的,而非自己定义的类型
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




	//first和last迭代器之间在调用此函数之前都是未初始化的
	//此函数的目的把first到last之间的迭代器所指的的地方初始化并赋值为x
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
