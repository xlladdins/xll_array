// xll_array_mask.cpp - Mask array elements
#include "xll_array.h"

using namespace xll;

AddIn xai_array_mask(
	Function(XLL_FPX, "xll_array_mask", "ARRAY.MASK")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_FPX, "mask", "is a mask or handle to a mask to be applied to array."),
		})
	.FunctionHelp("Return array values where corresponding mask is non-zero.")
	.Category(CATEGORY)
	.Documentation(R"(
If <code>mask</code> is smaller than <code>array</code> then it is
applied using cyclic indices. If <code>array</code> has more than one
row then the mask is applied to rows.
)")
);
_FPX* WINAPI xll_array_mask(_FPX* pa, const _FPX* pm)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(pa);
		const FPX* _m = ptr(pm);
		if (_m) {
			pm = _m->get();
		}

		if (_a) {
			mask(_a->get(), pm);
		}
		else {
			mask(a.get(), pm);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.TAKE: unknown exception");
	}

	return a.get();
}
