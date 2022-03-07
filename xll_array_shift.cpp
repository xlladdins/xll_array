// xll_array_shift.cpp - Shift elements right or left
#include "xll_array.h"

using namespace xll;

AddIn xai_array_shift(
	Function(XLL_FPX, "xll_array_shift", "ARRAY.SHIFT")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is the number of elements to shift.")
		})
	.FunctionHelp("Return shifted array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
If <code>n &gt; 0</code> then shift right. If <code>n &lt; 0</code> then shift left. 
)xyzyx")
);
_FPX* WINAPI xll_array_shift(_FPX* pa, LONG n)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);

	if (_a) {
		pa = _a->get();
	}

	shift(pa, n);

	return pa;
}

