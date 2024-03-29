// xll_array_drop.cpp - Drop from front or back of array.
#include "xll_array.h"

using namespace xll;

AddIn xai_array_drop(
	Function(XLL_FPX, "xll_array_drop", "ARRAY.DROP")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is then number of items to drop."),
		})
	.FunctionHelp("Drop items from front (n > 0) or back (n < 0) of array.")
	.Category(CATEGORY)
	.Documentation(R"(
Drop items from front (n > 0) or back (n < 0) of array
If <code>array</code> has more than one row then drop <code>n</code> rows.
)")
);
_FPX* WINAPI xll_array_drop(_FPX* pa, LONG n)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(pa);
		if (_a) {
			_a->drop(n);
		}
		else {
			a.drop(n);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return a.get();
}
#ifdef _DEBUG

_FPX* WINAPI xll_array_sequence(double start, double stop, double incr);

int xll_array_drop_test()
{
	{
		FPX a = *xll_array_sequence(1, 10, 1);
		ensure(*xll_array_drop(a.get(), 0) == a);
		ensure(xll_array_drop(a.get(), 5)->rows == 5);
		ensure(xll_array_drop(a.get(), 5)->array[0] == 6);
		ensure(xll_array_drop(a.get(), -5)->rows == 5);
		ensure(xll_array_drop(a.get(), -5)->array[0] == 1);
		ensure(xll_array_drop(a.get(), 100)->rows == 0);
		ensure(xll_array_drop(a.get(), -100)->rows == 0);
	}

	return TRUE;
}
Auto<OpenAfter> xaoa_array_drop_test(xll_array_drop_test);

#endif // _DEBUG
