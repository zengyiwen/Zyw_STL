#pragma once
namespace Zyw
{
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		while (last != first) *(--result) = *(--last);
		return result;
	}
}
