// xll_array.cpp - Array functions
#include <cmath>
#include <numeric>
#include "xll_array.h"
#include "fms_iterable.h"

#ifdef _DEBUG
int fms_iterable_iota_test_ = fms::iterable::iota_test();
int fms_iterable_take_test_ = fms::iterable::take_test();
//int fms_iterable_array_test_ = fms::iterable::array_test();
int fms_iterable_iterator_test_ = fms::iterable::iterator_test();
#endif _DEBUG

#ifndef CATEGORY
#define CATEGORY "Array"
#endif

using namespace xll;

AddIn xai_array_(
	Function(XLL_HANDLEX, "xll_array_", "\\ARRAY")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array of numbers."),
		Arg(XLL_WORD, "_columns", "is an optional number of columns. Default is `."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to the in-memory array.")
	.Category(CATEGORY)
	.Documentation(R"(
Create an in-memory two-dimensional array of numbers to be used by array functions.
If <code>array</code> is a scalar and <code>_columns</code> is not zero then
return a handle to an uninitialized array having <code>array</code> rows
and <code>_columns</code> columns.

Most array functions work in two modes: <em>function</em> and <em>command</em>.
If the first argument is an array then an new array is returned and the array function
has no side effects. If the first argument is a handle to an array then the function
modifies the in-memory array and returns the array handle. 
)")
);
HANDLEX WINAPI xll_array_(const _FP12* pa, WORD c)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		if (c != 0) {
			ensure(size(*pa) == 1 || !"\\ARRAY: first argument must be scalar if second argument is not zero");
			handle<FPX> h_(new FPX(static_cast<unsigned>(pa->array[0]), c));
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

AddIn xai_array_get(
	Function(XLL_FP, "xll_array_get", "ARRAY")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to an array of numbers."),
		Arg(XLL_BOOL, "_fast", "is an option boolean to specify fast lookup. Default is FALSE.")
		})
	.FunctionHelp("Return an array associated with handle.")
	.Category(CATEGORY)
	.Documentation(R"(
Retrieve an in-memory array created by
<code>\ARRAY</code>. By default the handle is checked to
ensure the array was created by a previous call to <code>\ARRAY</code>.
)")
.SeeAlso({ "\\ARRAY" })
);
_FP12* WINAPI xll_array_get(HANDLEX h, BOOL fast)
{
#pragma XLLEXPORT
	_FP12* pa = nullptr;

	try {
		handle<FPX> h_(h, !fast);
		if (h_) {
			pa = h_->get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return pa;
}

AddIn xai_array_resize(
	Function(XLL_FP, "xll_array_resize", "ARRAY.RESIZE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "rows", "is the number of rows."),
		Arg(XLL_LONG, "columns", "is the number of columns."),
		})
	.FunctionHelp("Resize an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Resize array to <code>rows</code> and <code>columns</code>.
If <code>array</code> is a handle this function resizes the in-memory array and
returns its handle, otherwise the resized array is returned.
)")
);
_FP12* WINAPI xll_array_resize(_FP12* pa, LONG r, LONG c)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(pa);
		if (_a) {
			_a->resize(r, c);
		}
		else {
			a.resize(r,c);
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
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		})
	.FunctionHelp("Return the number of rows of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of rows of an array.
)")
);
LONG WINAPI xll_array_rows(_FP12* pa)
{
#pragma XLLEXPORT
	LONG r = 0;

	try {
		FPX* _a = ptr(pa);
		if (_a) {
			r = _a->rows();
		}
		else {
			r = pa->rows;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.ROWS: unknown exception");
	}

	return r;
}

AddIn xai_array_columns(
	Function(XLL_LONG, "xll_array_columns", "ARRAY.COLUMNS")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		})
	.FunctionHelp("Return the number of columns of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of columns of an array.
)")
);
LONG WINAPI xll_array_columns(_FP12* pa)
{
#pragma XLLEXPORT
	LONG c = 0;

	try {
		FPX* _a = ptr(pa);
		if (_a) {
			c = _a->columns();
		}
		else {
			c = pa->columns;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.COLUMNS: unknown exception");
	}

	return c;
}

AddIn xai_array_size(
	Function(XLL_LONG, "xll_array_size", "ARRAY.SIZE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array."),
		})
	.FunctionHelp("Return the size of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of rows times the number of columns of an array.
)")
);
LONG WINAPI xll_array_size(_FP12* pa)
{
#pragma XLLEXPORT
	LONG c = 0;

	try {
		FPX* _a = ptr(pa);
		if (_a) {
			c = _a->size();
		}
		else {
			c = size(*pa);
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

#ifdef _DEBUG

int xll_array_test()
{
	try {
		{
			FPX a(1,1);
			a[0] = 2;

			HANDLEX ha = to_handle<FPX>(&a);
			_FP12* pa = xll_array_get(ha, true);
			ensure(pa->array[0] == 2);
			ensure(pa->rows == 1);
			ensure(xll_array_rows(pa) == 1);
			ensure(pa->columns == 1);
			ensure(xll_array_columns(pa) == 1);
			ensure(xll_array_size(pa) == 1);

			_FP12* pb = xll_array_resize(pa, 2, 3);
			ensure(pb->array[0] == 2);
			ensure(pb->rows == 2);
			ensure(xll_array_rows(pb) == 2);
			ensure(pb->columns == 3);
			ensure(xll_array_columns(pb) == 3);
			ensure(xll_array_size(pb) == 6);
		}
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": failed");

		return FALSE;
	}

	return TRUE;
}
Auto<OpenAfter> xaoa_array_test(xll_array_test);

#endif // _DEBUG

