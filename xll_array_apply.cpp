// xll_array_apply.cpp - Apply a function to an array.
#include "xll_array.h"

using namespace xll;


AddIn xai_array_apply(
	Function(XLL_FP, "xll_array_apply", "ARRAY.APPLY")
	.Arguments({
		Arg(XLL_LPOPER, "function", "is a function."),
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Apply a function to each element of an array.")
	.Category(CATEGORY)
	.Documentation(R"()")
);
_FPX* WINAPI xll_array_apply(LPOPER pf, _FPX* pa)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);
	if (_a) {
		pa = _a->get();
	}

	for (unsigned i = 0; i < size(*pa); ++i) {
		pa->array[i] = Excel(xlUDF, *pf, OPER(pa->array[i])).as_num();
	}

	return pa;
}
