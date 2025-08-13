// xll_array_shift.cpp - Shift elements right or left
#include "xll_array.h"

using namespace xll;

AddIn xai_array_shift(
	Function(XLL_FP, "xll_array_shift", "ARRAY.SHIFT")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is the number of elements to shift.")
		})
	.FunctionHelp("Return shifted array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
If <code>n &gt; 0</code> then shift right. If <code>n &lt; 0</code> then shift left. 
)xyzyx")
);
_FP12* WINAPI xll_array_shift(_FP12* pa, LONG n)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);

	if (_a) {
		pa = _a->get();
	}

	shift(pa, n);

	return pa;
}

