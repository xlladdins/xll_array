// xll_array.h - array functions
#pragma once
#include "xll/xll/xll.h"

namespace xll {

	// underlying pointer if 1 x 1 and handle to FPX
	inline FPX* ptr(_FPX* pa)
	{
		if (size(*pa) == 1) {
			handle<FPX> _a(pa->array[0]);
			if (_a) {
				return _a.ptr();
			}
		}

		return nullptr;
	}

	// take elements from front (n > 0) or back (n < 0) of array
	inline _FPX* array_take(int n, _FPX* pa)
	{
		// so take works for a single row
		if (pa->rows == 1) {
			std::swap(pa->rows, pa->columns);
		}

		if (n < pa->rows and -n < pa->rows) {
			if (n >= 0) {
				pa->rows = n;
			}
			else {
				size_t N = static_cast<size_t>(-n) * pa->columns;
				MoveMemory(pa->array, end(*pa) - N, N * sizeof(double));
				pa->rows = -n;
			}
		}
		/*
		if (pa->columns == 1) {
			std::swap(pa->rows, pa->columns);
		}
		*/

		return pa;
	}

	// drop elements from front (n > 0) or back (n < 0) of array
	inline _FPX* array_drop(int n, _FPX* pa)
	{
		// so take works for a single row
		if (pa->rows == 1) {
			std::swap(pa->rows, pa->columns);
		}

		if (n >= pa->rows or -n >= pa->rows) {
			pa->rows = 0;
			pa->columns = 0;
		}
		else if (n < 0) {
			pa->rows = pa->rows + n;
		}
		else if (n > 0) {
			pa->rows = pa->rows - n;
			size_t N = static_cast<size_t>(pa->rows) * pa->columns;
			MoveMemory(end(*pa) - N, pa->array, N * sizeof(double));
		}

		if (pa->columns == 1) {
			std::swap(pa->rows, pa->columns);
		}

		return pa;
	}

	// mask elements
	inline _FPX* array_mask(_FPX* pm, _FPX* pa)
	{
		unsigned j = 0;

		for (unsigned i = 0; i < size(*pa); ++i) {
			if (index(*pm, i) != 0) {
				pa->array[j] = pa->array[i];
				++j;
			}
		}

		if (pa->rows == 1) {
			pa->columns = j;
		}
		else if (pa->columns == 1) {
			pa->rows = j;
		}
		else {
			pa->rows = j / pa->columns;
			unsigned m = j % pa->columns;
			if (m != 0) {
				++pa->rows;
				for (unsigned i = 0; i < pa->columns - m; ++i) {
					pa->array[j + i] = std::numeric_limits<double>::quiet_NaN();
				}
			}
		}

		return pa;
	}

}
