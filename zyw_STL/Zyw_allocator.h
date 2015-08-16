#pragma once
#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>	
namespace Zyw
{   
	



	template<int inst>  //inst没啥用,主要是为了使用模板
	class __malloc_alloc_template {

	private:
		//malloc和realloc失败的时候调用下面2个函数
		static void *oom_realloc(void *, size_t);
		static void *oom_malloc(size_t n);
		//	//函数指针用来处理内存不足的情况
		static void(*__malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			//分配失败,调用oom_malloc()
			if (0 == result) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);//第一级配置器直接使用free
		}
		//重新分配内存
		static void *reallocate(void*p, size_t, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (0 == result) result = oom_realloc(p, new_sz);
			return result;
		}
		//模拟set_new_handler,自己制定内存出错调用的函数
		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return(old);
		}
	};
	using  malloc_alloc = __malloc_alloc_template < 0 >;
	template<int inst>
	//用于处理内存分配出错的函数指针初始化为空
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;


	template<int inst>
	inline void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void*result;
		for (;;) {
			my_malloc_handler = __malloc_alloc_oom_handler;//如果没有配置,直接终止
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			*my_malloc_handler();
			result = realloc(p, n);
			if (result) return(result);
		}
	}

	template<int inst>
	inline void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();//定义函数指针
		void *result;
		for (;;) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { //__THROW_BAD_ALLOC; }
				my_malloc_handler();//一般用于释放之前allocate分配的内存
				result = malloc(n);//再次尝试配置内存
				if (result) return(result);
			}
			return  nullptr;
		}

	}

	//二级配置器
	enum { __ALIGN = 8 };//小型区块上调边界是8
	enum { __MAX_BYTES = 128 };//小型区块的上限为128
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; //free_list链表个数

	template<bool threads, int inst>
	class __default_alloc_template {

	private:
		//返回比bytes大一点的8的倍数(必须是8的倍数)
		static size_t ROUND_UP(size_t bytes) {

			return ((bytes)+__ALIGN - 1)&~(__ALIGN - 1);
		}
	private:
		//内存管理链表节点
		union obj {
			union obj* free_list_link;
			char client_data[1];
		};
	private:
		//指针数组管理内存
		static obj* volatile free_list[__NFREELISTS];

		//根据使用区块大小,决定使用几号free_list,n从1开始
		static size_t FREELISt_INDEX(size_t bytes) {
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		//返回一个大小为n的对象,并可能加入大小为n的其他区块到free_list
		static void *refill(size_t n);

		//配置一大块空间,可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便,nobjs会降低
		static char *chunk_alloc(size_t size, int &nobjs);

		//内存池
		static char *start_free;//内存池起始位置
		static char *end_free;//内存池结束位置
		static size_t heap_size;
	public:
		static void *allocate(size_t n);
		static void deallocate(void*p, size_t n);
		static void *reallocate(void*P, size_t old_sz, size_t new_sz);

	};

	template<bool threads, int inst>
	char*__default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char*__default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;
	template<bool threads, int inst>
	typename __default_alloc_template<threads, inst>::obj* volatile
		__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


	template<bool threads, int inst>
	void*__default_alloc_template<threads, inst>::allocate(size_t n)
	{
		obj*volatile*my_free_list;
		obj*result;
		if (n > (size_t)__MAX_BYTES) {
			return(__malloc_alloc_template<0>::allocate(n));
		}
		my_free_list = free_list + FREELISt_INDEX(n);
		result = *my_free_list;
		if (result == 0) {
			//没找到可用的free_list;
			void*r = refill(ROUND_UP(n));//重新填充
			return r;
		}
		*my_free_list = result->free_list_link;
		return result;
	}
	template<bool threads, int inst>
	void *__default_alloc_template<threads, inst>::reallocate(void*P, size_t old_sz, size_t new_sz)
	{

		deallocate(P, old_sz);
		return allocate(new_sz);

	}

	template<bool threads, int inst>
	void __default_alloc_template<threads, inst>::deallocate(void*p, size_t n)
	{
		obj*q = (obj*)p;
		obj*volatile*my_free_list;
		if (n > (size_t)__MAX_BYTES) {
			__malloc_alloc_template<0>::deallocate(p, n);
			return;
		}
		my_free_list = free_list + FREELISt_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}

	template<bool threads, int inst>
	void*  __default_alloc_template<threads, inst>::refill(size_t n) {
		int nobjs = 20;
		char *chunk = chunk_alloc(n, nobjs);
		obj*volatile*my_free_list;
		obj*result;
		obj*current_obj, *next_obj;
		int i;
		if (1 == nobjs)  return chunk;
		my_free_list = free_list + FREELISt_INDEX(n);
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);
		for (i = 1;; ++i) {
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				current_obj->free_list_link = 0;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;
			}

		}
		return  result;
	}

	template<bool threads, int inst>
	char*__default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs)
	{

		char* result = nullptr;
		size_t total_bytes = size*nobjs;//一次配置n个nobjs
		size_t byte_left = end_free - start_free;//内存池剩余空间
		if (byte_left >= total_bytes) {   //内存足够
			result = start_free;
			start_free += total_bytes;
			return result;
		}//内存池剩余空间不能完全满足需求量,但可以提供一个以上
		else if (byte_left >= size) {
			nobjs = byte_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else  //一个都无法满足
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size);
			//让内存池中的残余零头还有利用价值
			if (byte_left > 0)
			{
				obj*volatile*my_free_list = free_list + FREELISt_INDEX(byte_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			//配置heap空间,用来补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free) {
				int i;
				obj*volatile*my_free_list, *p;
				for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
					my_free_list = free_list + FREELISt_INDEX(i);
					p = *my_free_list;
					if (0 != p) {
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;//如果出现意外
				//这里将导致抛出异常,因为上面调用malloc失败,下面一定失败
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);


			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归自己调用自己,为了修正nobjs
			return chunk_alloc(size, nobjs);
		}

	}

	
	//sim_alloc简化配置器

	template<class T, class Alloc>
	class sim_alloc {
	public:
		static T* allocate(size_t n) {
			return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
		}
		static T*allocate() {
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T*p, size_t n)
		{
			if (0 != n)Alloc::deallocate(p, n*sizeof(T));
		}
		static void deallocate(T*p)
		{
			Alloc::deallocate(p, sizeof(T));
		}


	};
	using alloc = __default_alloc_template<0, 0>; //默认使用二级配置器

}
		




