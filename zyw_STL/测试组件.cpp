#pragma warning(disable:4996)
#include "Zyw_allocator.h"
#include "Zyw_iterator.h"
#include "Zyw_construct.h"
#include "Zyw_uninitialized.h"
#include "Zyw_type_traits.h"
#include "zyw_vector.h"

int main()
{
	Zyw::vector<int> k(9, 6);
	Zyw::vector<int> qq(10);
	std::cout << qq.capacity() << std::endl;
	std::cout<<k.size()<<std::endl;
	k.clear();
	for (auto qq : k)
	{
		std::cout << qq << std::endl;
	}

	system("pause");
	return 0;
	
}
