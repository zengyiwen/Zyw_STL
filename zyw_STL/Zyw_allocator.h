#pragma once
#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>	
namespace Zyw
{   
	



	template<int inst>  //instûɶ��,��Ҫ��Ϊ��ʹ��ģ��
	class __malloc_alloc_template {

	private:
		//malloc��reallocʧ�ܵ�ʱ���������2������
		static void *oom_realloc(void *, size_t);
		static void *oom_malloc(size_t n);
		//	//����ָ�����������ڴ治������
		static void(*__malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			//����ʧ��,����oom_malloc()
			if (0 == result) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);//��һ��������ֱ��ʹ��free
		}
		//���·����ڴ�
		static void *reallocate(void*p, size_t, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (0 == result) result = oom_realloc(p, new_sz);
			return result;
		}
		//ģ��set_new_handler,�Լ��ƶ��ڴ������õĺ���
		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return(old);
		}
	};
	using  malloc_alloc = __malloc_alloc_template < 0 >;
	template<int inst>
	//���ڴ����ڴ�������ĺ���ָ���ʼ��Ϊ��
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;


	template<int inst>
	inline void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void*result;
		for (;;) {
			my_malloc_handler = __malloc_alloc_oom_handler;//���û������,ֱ����ֹ
			if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
			*my_malloc_handler();
			result = realloc(p, n);
			if (result) return(result);
		}
	}

	template<int inst>
	inline void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();//���庯��ָ��
		void *result;
		for (;;) {
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler) { //__THROW_BAD_ALLOC; }
				my_malloc_handler();//һ�������ͷ�֮ǰallocate������ڴ�
				result = malloc(n);//�ٴγ��������ڴ�
				if (result) return(result);
			}
			return  nullptr;
		}

	}

	//����������
	enum { __ALIGN = 8 };//С�������ϵ��߽���8
	enum { __MAX_BYTES = 128 };//С�����������Ϊ128
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; //free_list�������

	template<bool threads, int inst>
	class __default_alloc_template {

	private:
		//���ر�bytes��һ���8�ı���(������8�ı���)
		static size_t ROUND_UP(size_t bytes) {

			return ((bytes)+__ALIGN - 1)&~(__ALIGN - 1);
		}
	private:
		//�ڴ��������ڵ�
		union obj {
			union obj* free_list_link;
			char client_data[1];
		};
	private:
		//ָ����������ڴ�
		static obj* volatile free_list[__NFREELISTS];

		//����ʹ�������С,����ʹ�ü���free_list,n��1��ʼ
		static size_t FREELISt_INDEX(size_t bytes) {
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		//����һ����СΪn�Ķ���,�����ܼ����СΪn���������鵽free_list
		static void *refill(size_t n);

		//����һ���ռ�,������nobjs����СΪsize������
		//�������nobjs��������������,nobjs�ή��
		static char *chunk_alloc(size_t size, int &nobjs);

		//�ڴ��
		static char *start_free;//�ڴ����ʼλ��
		static char *end_free;//�ڴ�ؽ���λ��
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
			//û�ҵ����õ�free_list;
			void*r = refill(ROUND_UP(n));//�������
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
		size_t total_bytes = size*nobjs;//һ������n��nobjs
		size_t byte_left = end_free - start_free;//�ڴ��ʣ��ռ�
		if (byte_left >= total_bytes) {   //�ڴ��㹻
			result = start_free;
			start_free += total_bytes;
			return result;
		}//�ڴ��ʣ��ռ䲻����ȫ����������,�������ṩһ������
		else if (byte_left >= size) {
			nobjs = byte_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else  //һ�����޷�����
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size);
			//���ڴ���еĲ�����ͷ�������ü�ֵ
			if (byte_left > 0)
			{
				obj*volatile*my_free_list = free_list + FREELISt_INDEX(byte_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			//����heap�ռ�,���������ڴ��
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
				end_free = 0;//�����������
				//���ｫ�����׳��쳣,��Ϊ�������mallocʧ��,����һ��ʧ��
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);


			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//�ݹ��Լ������Լ�,Ϊ������nobjs
			return chunk_alloc(size, nobjs);
		}

	}

	
	//sim_alloc��������

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
	using alloc = __default_alloc_template<0, 0>; //Ĭ��ʹ�ö���������

}
		




