// xll_array_diff.cpp - Adjacent differences of array
#include <numeric>
#include "xll_array.h"

using namespace xll;

AddIn xai_array_diff(
	Function(XLL_FPX, "xll_array_diff", "ARRAY.DIFF")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
	.FunctionHelp("Return adjacent differences of array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Return <code>{a0, a1 - a0, a2 - a1,...}</code>.
)xyzyx")
);
_FPX* WINAPI xll_array_diff(_FPX* pa)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);

	if (_a) {
		pa = _a->get();
	}

	std::adjacent_difference(begin(*pa), end(*pa), begin(*pa));

	return pa;
}

