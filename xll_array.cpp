// xll_array.cpp - Sample xll project.
#include <cmath>
//Uncomment to build for versions of Excel prior to 2007
//#define XLOPERX XLOPER
#include "xll_array.h"

#ifndef CATEGORY
#define CATEGORY "Array"
#endif

using namespace xll;


AddIn xai_array_set(
	Function(XLL_HANDLE, "xll_array_set", "\\ARRAY")
	.Arguments({
		Arg(XLL_FP, "array", "is an array of numbers."),
		Arg(XLL_WORD, "_columns", "is an optional number of columns. Default is 0."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to the in-memory array.")
	.Category(CATEGORY)
	.Documentation(R"(
Create an in-memory two-dimensional array of numbers to be used by array functions.
)")
);
HANDLEX WINAPI xll_array_set(const _FPX* pa, WORD c)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		if (c != 0) {
			ensure(size(*pa) == 1 || !"\\ARRAY: first argument must be scalar if second argument is not zero");
			handle<FPX> h_(new FPX(static_cast<WORD>(pa->array[0]), c));
			ensure(h_);
			h = h_.get();
		}
		else {
			handle<FPX> h_(new FPX(*pa));
			h = h_.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("\\ARRAY: unknown exception");
	}

	return h;
}

AddIn xai_array_resize(
	Function(XLL_FPX, "xll_array_resize", "ARRAY.RESIZE")
	.Arguments({
		Arg(XLL_FPX, "handle", "is a handle to an array."),
		Arg(XLL_LONG, "rows", "is the number of rows."),
		Arg(XLL_LONG, "columns", "is the number of columns."),
		})
		.FunctionHelp("Resize an in-memory array.")
	.Category(CATEGORY)
	.Documentation(R"(
Resize <code>rows</code> and <code>columns</code> of an in-memory array
given its <code>handle</code>
)")
);
_FPX* WINAPI xll_array_resize(_FPX* h, LONG r, LONG c)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *h;

		FPX* ph = ptr(h);
		if (ph) {
			ph->resize(r, c);
		}
		else {
			a.resize(r, c);
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

AddIn xai_array_rows(
	Function(XLL_LONG, "xll_array_rows", "ARRAY.ROWS")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Return the number of rows of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of rows of an in-memory array
given its <code>handle</code>.
)")
);
LONG WINAPI xll_array_rows(_FPX* pa)
{
#pragma XLLEXPORT
	LONG r = 0;

	try {
		FPX* a = ptr(pa);
		if (a) {
			r = a->rows();
		}
		else {
			r = pa->rows;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return r;
}

AddIn xai_array_columns(
	Function(XLL_LONG, "xll_array_columns", "ARRAY.COLUMNS")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Return the number of columns of an in-memory array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of columns of an in-memory array
given its <code>handle</code>.
)")
);
LONG WINAPI xll_array_columns(_FPX* pa)
{
#pragma XLLEXPORT
	LONG c = 0;

	try {
		FPX* a = ptr(pa);
		if (a) {
			c = a->columns();
		}
		else {
			c = pa->columns;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return c;
}

AddIn xai_array_get(
	Function(XLL_FP, "xll_array_get", "ARRAY.GET")
	.Arguments({
		Arg(XLL_HANDLE, "handle", "is a handle to an array of numbers.")
		})
	.FunctionHelp("Return an array associated with handle.")
	.Category(CATEGORY)
	.Documentation(R"(
Retrieve an in-memory array created by
<a href=\"ARRAY.SET.html\">\ARRAY.SET</a>.
)")
);
_FPX* WINAPI xll_array_get(HANDLEX h)
{
#pragma XLLEXPORT
	_FPX* pa = nullptr;

	try {
		handle<FPX> h_(h);
		if (h_) {
			pa = h_->get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.GET: unknown exception");
	}

	return pa;
}

AddIn xai_array_sequence(
	Function(XLL_FP, "xll_array_sequence", "ARRAY.SEQUENCE")
	.Arguments({
		Arg(XLL_DOUBLE, "start", "is the first value in the sequence.", "0"),
		Arg(XLL_DOUBLE, "stop", "is the last value in the sequence.", "3"),
		Arg(XLL_DOUBLE, "_incr", "is an optional value to increment by. Default is 1.")
		})
	.FunctionHelp("Return a one column array from start to stop with specified optional increment.")
	.Category(CATEGORY)
	.Documentation(R"(
Return a one columns array <code>{start; start + incr; ...; stop}<code>.
)")
);
_FPX* WINAPI xll_array_sequence(double start, double stop, double incr)
{
#pragma XLLEXPORT
	static xll::FPX a;

	try {
		if (incr == 0) {
			incr = 1;
			if (start > stop) {
				incr = -1;
			}
		}

		unsigned n = 1u + static_cast<unsigned>(fabs((stop - start) / incr));
		a.resize(n, 1);
		for (unsigned i = 0; i < n; ++i) {
			a[i] = start + i * incr;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.SEQUENCE: unknown exception");
	}

	return a.get();
}

