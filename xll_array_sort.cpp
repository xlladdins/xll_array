// xll_array_sort.cpp - Sort or partial sort arrays.
#include <algorithm>
#include "xll_array.h"

using namespace xll;

AddIn xai_array_sort(
	Function(XLL_FP, "xll_array_sort", "ARRAY.SORT")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array to sort"),
		Arg(XLL_LONG, "_count", "is an optional number of elements to partial sort. Default is 0."),
		})
	.FunctionHelp("Sort _count elements of array in increasing (_count >= 0) or decreasing (n < 0) order.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Sort all elements of <code>array</code> in increasing numerical order when 
<code>_count = 0</code>. 
If<code>_count = -1</code> sort in decreasing order. If <code>_count > 0</code> only sort
the smallest <code>_count</code> values. If <code>_count < -1</code> only sort
the largest <code>-_count</code> values.
If <code>array</code> is not a row or column then partial elements of
the last row are set to \(\infty\) or \(-\infty\) if the sort is
increasing or decreasing, respecively.
<p>
If <code>array</code> is a handle return a handle to the sorted array.
)xyzyx")
);
_FP12* WINAPI xll_array_sort(_FP12* pa, LONG n)
{
#pragma XLLEXPORT

	FPX* _a = ptr(pa);
	if (_a) {
		pa = _a->get();
	}

	LONG na = (LONG)size(*pa);
	n = std::clamp(n, -na, na);

	if (n == 0) {
		n = na;
		std::sort(begin(*pa), end(*pa));
	}
	else if (n > 0) {
		std::partial_sort(begin(*pa), begin(*pa) + n, end(*pa));
	}
	else if (n == -1) {
		n = na;
		std::sort(begin(*pa), end(*pa), std::greater<double>{});
	}
	else { // n < -1
		std::partial_sort(begin(*pa), begin(*pa) - n, end(*pa), std::greater<double>{});
	}

	return pa;
}
