#define  D_SCL_SECURE_NO_WARNINGS
#pragma once
#include<algorithm>
#include "Zyw_allocator.h"
#include "Zyw_algorithm.h"

namespace Zyw
{

	template<class T, class Alloc = alloc>
	class vector {
	public:
		typedef T value_type;
		typedef value_type*  pointer;
		typedef value_type*  iterator;
		typedef value_type&  reference;
		typedef size_t      size_type;
		typedef ptrdiff_t difference_type;
	protected:
		typedef sim_alloc<value_type, Alloc> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;
		void insert_aux(iterator position, const T&x);
		void deallocate() {
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}                     //9                  6
		void fill_initialize(size_type n, const T&value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
	public:
		iterator begin() const{ return start; }
		iterator end() const { return finish; }
		size_type size() const { return (int)(finish - start); }
		size_type capacity() const {
			return (size_type) (end_of_storage -begin());
		}
		bool empty() const { return begin() == end(); }
		reference operator[](size_type n) { return *(begin() + n); }
		vector() :start(0), finish(0), end_of_storage(0) {}
		vector(size_type n, const T&value) { fill_initialize(n, value); }
		vector(int n, const T&value) { fill_initialize(n, value); }
		vector(long n, const T&value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		~vector() {
			destroy(start, finish);
			deallocate();
		}
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		void push_back(const T&x)
		{
			if (finish != end_of_storage) {
				construct(finish, x);
				++finish;
			}
			else
			{
				insert_aux(end(), x);
			}
		}
		void pop_back() {
			--finish;
			destroy(finish);
		}
		iterator erase(iterator first, iterator last) {
			iterator i = std::copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;

		}
		iterator erase(iterator position) {
			if (position + 1 != end())
				std::copy(position + 1, finish, position);
			--finish;
			destroy(finish);
			return position;
		}
		void resize(size_type new_size, const T&x) {
			if (new_size < size())
				erase(begin() + new_size, end());
			else
				insert(end(), new_size - size(), x);
		}
		void resize(size_type new_size) { resize(new_size, T()); }
		void clear() { erase(begin(), end()); }
	protected:
		                          //9              //6
		iterator allocate_and_fill(size_type n, const T&x) {
			iterator result = data_allocator::allocate(n);
			return Zyw:: uninitialized_fill_n(result, n, x);
		}


	};

	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T&x)
	{
		if (finish != end_of_storage) {
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;
			Zyw::copy_backward(position, finish - 2, finish - 1);
		}
		else {
			const size_type old_size = size();
			size_type len = 0;
			if (old_size == 0)
			{
				len = 1;
			}
			else
			{

				len = 2 * old_size;
			}
			//	const size_type len=old_size*2;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = Zyw::uninitialized_copy(start, position, new_start);
				construct(new_finish, x);
				++new_finish;
				new_finish = Zyw::uninitialized_copy(position, finish, new_finish);
			}
			catch (int)
			{
				destroy(begin(), end());
				deallocate();
			}


			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

}