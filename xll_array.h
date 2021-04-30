// xll_array.h - array functions
#pragma once
#include "xll/xll/xll.h"

namespace xll {

	// underlying pointer if 1 x 1 and handle to FPX
	inline FPX* ptr(_FPX* pa)
	{
		if (size(*pa) != 1) {
			return nullptr;
		}

		handle<FPX> h(pa->array[0]);

		return h ? h.ptr() : nullptr;
	}

	inline _FPX* array_take(int n, _FPX* pa)
	{
		// so take works for a single row
		if (pa->rows == 1) {
			std::swap(pa->rows, pa->columns);
		}

		if (n == 0) {
			pa->rows = 0;
			pa->columns = 0;
		}
		else if (n > 0 and static_cast<unsigned>(n) * pa->columns < size(*pa)) {
			pa->rows = n * pa->columns;
		}
		else if (static_cast<unsigned>(-n) * pa->columns < size(*pa)) {
			pa->rows = -n * pa->columns;
			MoveMemory(pa->array, pa->array + size(*pa) + n * pa->columns, -n * sizeof(double));
		}

		if (pa->columns == 1) {
			std::swap(pa->rows, pa->columns);
		}

		return pa;
	}

}
