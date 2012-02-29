#ifndef VD_CONCURRENT_QUEUE_INCLUDED
#define VD_CONCURRENT_QUEUE_INCLUDED

#include <stdlib.h>
#include <stddef.h>

#include "memory/atomics.h"

namespace vd {
namespace concurrent {

template <typename T, int N> 
class Queue
{
public:

	inline Queue ()
	{
		 begin_range = VD_NEW(Range);
		 alloc_assert (begin_range);
		 begin_pos = 0;
		 back_range = NULL;
		 back_pos = 0;
		 end_range = begin_range;
		 end_pos = 0;
	}

	inline ~Queue ()
	{
		while (true) 
		{
			if (begin_range == end_range) 
			{
				VD_DELETE(begin_range);
				break;
			} 
			
			Range *old = begin_range;
			begin_range = begin_range->next;
			VD_DELETE(old);
		}

		Range *ex = extra_range.swap(NULL);
		if (ex)
			VD_DELETE(ex);
	}

	inline T &Front ()
	{
		 return begin_range->values [begin_pos];
	}

	inline T &Back ()
	{
		return back_range->values [back_pos];
	}

	inline void Push ()
	{
		back_range = end_range;
		back_pos = end_pos;

		if (++end_pos != N)
			return;

		Range *ex = extra_range.swap(NULL);
		if (ex) 
		{
			end_range->next = ex;
			ex->prev = end_range;
		} 
		else 
		{
			end_range->next = VD_NEW(Range);
			VD_ASSERT(end_range->next);
			end_range->next->prev = end_range;
		}
		end_range = end_range->next;
		end_pos = 0;
	}

	inline void Unpush ()
	{
		if (back_pos)
		{
			--back_pos;
		}
		else 
		{
			back_pos = N - 1;
			back_range = back_range->prev;
		}

		if (end_pos)
		{
			--end_pos;
		}
		else 
		{
			end_pos = N - 1;
			end_range = end_range->prev;
			VD_DELETE(end_range->next);
			end_range->next = NULL;
		}
	}

	inline void Pop ()
	{
		if (++begin_pos == N) 
		{
			Range* br = begin_range;
			begin_range = begin_range->next;
			begin_range->prev = NULL;
			begin_pos = 0;
			Range *er = extra_range.swap(br);
			if (er)
				VD_DELETE(er);
		}
	}

private:

	struct Range
	{
		 T values [N];
		 Range *prev;
		 Range *next;
	};

	Range *begin_range;
	int begin_pos;

	Range *back_range;
	int back_pos;

	Range *end_range;
	int end_pos;

	vd::memory::Atomic<Range*> extra_range;

	VD_DISABLE_COPY_CONSTRUCTORS(Queue);
};

}
}

#endif
