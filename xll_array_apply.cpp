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
_FP12* WINAPI xll_array_apply(LPOPER pf, _FP12* pa)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);
	if (_a) {
		pa = _a->get();
	}

	for (int i = 0; i < size(*pa); ++i) {
		pa->array[i] = Num(Excel(xlUDF, *pf, pa->array[i]));
	}

	return pa;
}
