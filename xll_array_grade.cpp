// xll_array_grade.cpp - Grade or partial grade arrays.
#include <algorithm>
#include "xll_array.h"
#include <numeric>

using namespace xll;

AddIn xai_array_grade(
	Function(XLL_FP, "xll_array_grade", "ARRAY.GRADE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array to grade"),
		Arg(XLL_LONG, "_count", "is an optional number of elements to partial grade. Default is 0."),
		})
		.FunctionHelp("Grade _count elements of array in increasing (_count >= 0) or decreasing (n < 0) order.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Grade all elements of <code>array</code> in increasing numerical order when 
<code>_count = 0</code>. 
If<code>_count = -1</code> grade in decreasing order. If <code>_count > 0</code> only grade
the smallest <code>_count</code> values. If <code>_count < -1</code> only grade
the largest <code>-_count</code> values.
<p>
Note <code>ARRAY.INDEX(array, ARRAY.GRADE(array))</code> is the same as <code>ARRAY.SORT(array)</code>
<p>
<code>array</code> must not be a handle.
)xyzyx")
);
_FP12* WINAPI xll_array_grade(_FP12* pa, LONG n)
{
#pragma XLLEXPORT

	static FPX a;

	try {
		LONG na = (LONG)size(*pa);
		n = std::clamp(n, -na, na);

		a.resize(na, 1);
		std::iota(begin(a), end(a), 0);

		auto lt = [pa](double x, double y) { return pa->array[(size_t)x] < pa->array[(size_t)y];  };
		auto gt = [pa](double x, double y) { return pa->array[(size_t)x] > pa->array[(size_t)y];  };

		if (n == 0) {
			std::sort(begin(a), end(a), lt);
			n = na;
		}
		else if (n > 0) {
			std::partial_sort(begin(a), begin(a) + n, end(a), lt);
		}
		else if (n == -1) {
			std::sort(begin(a), end(a), gt);
			n = na;
		}
		else { // n < -1
			std::partial_sort(begin(a), begin(a) - n, end(a), gt);
			n = -n;
		}

		a.resize(n, 1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return nullptr;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");

		return nullptr;
	}

	return a.get();
}
