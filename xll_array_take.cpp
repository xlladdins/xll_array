// xll_array_take.cpp - Take elements from front or back of array.
#include "xll_array.h"

using namespace xll;

AddIn xai_array_take(
	Function(XLL_FP, "xll_array_take", "ARRAY.TAKE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is then number of items to take."),
		})
		.FunctionHelp("Take items from front (n > 0) or back (n < 0) of array.")
	.Category(CATEGORY)
	.Documentation(R"(
Take items from front (n > 0) or back (n < 0) of array
If <code>array</code> has more than one row then take <code>n</code> rows.
)")
);
_FP12* WINAPI xll_array_take(_FP12* pa, LONG n)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(pa);
		if (_a) {
			_a->take(n);
		}
		else {
			a.take(n);
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

_FP12* WINAPI xll_array_sequence(double start, double stop, double incr);

int xll_array_take_test()
{
	{
		FPX a = *xll_array_sequence(1, 10, 1);
		ensure(xll_array_take(a.get(), 0)->rows == 0);
		ensure(xll_array_take(a.get(), 0)->columns == 0);
		ensure(xll_array_take(a.get(), 5)->rows == 5);
		ensure(xll_array_take(a.get(), 5)->array[0] == 1);
		ensure(xll_array_take(a.get(), -5)->rows == 5);
		ensure(xll_array_take(a.get(), -5)->array[0] == 6);
		ensure(xll_array_take(a.get(), 100)->rows == 10);
		ensure(xll_array_take(a.get(), -100)->rows == 10);
	}

	return TRUE;
}
Auto<OpenAfter> xaoa_array_take_test(xll_array_take_test);

#endif // _DEBUG
