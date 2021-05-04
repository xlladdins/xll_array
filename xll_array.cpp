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
If <code>array</code> is a scalar and <code>_columns</code> is not zero then
return a handle to an uninitialized array having <code>array</code> rows
and <code>_columns</code> columns.
)")
);
HANDLEX WINAPI xll_array_set(const _FPX* pa, WORD c)
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

AddIn xai_array_resize(
	Function(XLL_FPX, "xll_array_resize", "ARRAY.RESIZE")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "rows", "is the number of rows."),
		Arg(XLL_LONG, "columns", "is the number of columns."),
		})
		.FunctionHelp("Resize an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Resize array to <code>rows</code> and <code>columns</code>.
)")
);
_FPX* WINAPI xll_array_resize(const _FPX* pa, LONG r, LONG c)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(&a);
		_a->resize(r, c);
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
Return the number of rows of an array.
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
		XLL_ERROR("ARRAY.ROWS: unknown exception");
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
Return the number of columns of an array
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
		XLL_ERROR("ARRAY.COLUMNS: unknown exception");
	}

	return c;
}

AddIn xai_array_size(
	Function(XLL_LONG, "xll_array_size", "ARRAY.SIZE")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Return the size of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of size of an in-memory array
given its <code>handle</code>.
)")
);
LONG WINAPI xll_array_size(_FPX* pa)
{
#pragma XLLEXPORT
	LONG c = 0;

	try {
		FPX* a = ptr(pa);
		if (a) {
			c = a->size();
		}
		else {
			c = size(*pa);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.SIZE: unknown exception");
	}

	return c;
}

AddIn xai_array_get(
	Function(XLL_FP, "xll_array_get", "ARRAY.GET")
	.Arguments({
		Arg(XLL_HANDLE, "handle", "is a handle to an array of numbers."),
		Arg(XLL_BOOL, "_fast", "is an option boolean to specify fast lookup. Default is FALSE.")
		})
	.FunctionHelp("Return an array associated with handle.")
	.Category(CATEGORY)
	.Documentation(R"(
Retrieve an in-memory array created by
<a href=\"ARRAY.SET.html\">\ARRAY.SET</a>.
)")
);
_FPX* WINAPI xll_array_get(HANDLEX h, BOOL fast)
{
#pragma XLLEXPORT
	_FPX* pa = nullptr;

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
		XLL_ERROR("ARRAY.GET: unknown exception");
	}

	return pa;
}

AddIn xai_array_take(
	Function(XLL_FPX, "xll_array_take", "ARRAY.TAKE")
	.Arguments({
		Arg(XLL_LONG, "n", "is then number of items to take."),
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Take items from front (n > 0) or back (n < 0) of array.")
	.Category(CATEGORY)
	.Documentation(R"(
If <code>array</code> has more than one column then take <code>n</code> rows.
)")
);
_FPX* WINAPI xll_array_take(LONG n, _FPX* pa)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(&a);
		array_take(n, _a->get());
		_a->resize(_a->rows(), _a->columns());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.TAKE: unknown exception");
	}

	return a.get();
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

#ifdef _DEBUG

AddIn xai_array_sort(
	Function(XLL_FP, "xll_array_sort", "ARRAY.SORT")
	.Arguments({
		Arg(XLL_FP, "array", "is an array to sort"),
		Arg(XLL_LONG, "_count", "is an optional number of elements to partial sort. Default is 0."),
		})
		.FunctionHelp("Sort _count elements of array in increasing (_count >= 0) or decreasing (n < 0) order.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Sort all elements of <code>array</code> in increasing numerical order when <code>_count = 0</code>. If
<code>_count = -1</code> sort in decreasing order. If <code>_count > 0</code> only sort
the smallest <code>_count</code> values. If <code>_count < -1</code> only sort
the largest <code>_count</code> values.
)xyzyx")
);
_FPX* WINAPI xll_array_sort(_FPX* pa, LONG n)
{
#pragma XLLEXPORT
	if (n == 0) {
		n = size(*pa);
		std::sort(begin(*pa), end(*pa));
	}
	else if (n > 0) {
		if (static_cast<unsigned>(n) > size(*pa)) {
			n = size(*pa);
		}
		std::partial_sort(begin(*pa), begin(*pa) + n, end(*pa));
	}
	else if (n == -1) {
		n = size(*pa);
		std::sort(begin(*pa), end(*pa), std::greater<double>{});
	}
	else {
		if (static_cast<unsigned>(-n) > size(*pa)) {
			n = size(*pa);
			n = -n;
		}
		std::partial_sort(begin(*pa), begin(*pa) - n, end(*pa), std::greater<double>{});
	}

	if (pa->rows == 1) {
		pa->columns = abs(n);
	}
	else if (pa->columns == 1) {
		pa->rows = abs(n);
	}
	else {
		double inf = std::numeric_limits<double>::infinity();
		if (n < 0) {
			inf = -inf;
		}
		n = abs(n);
		pa->rows = n / pa->columns; // might truncate
		auto m = n % pa->columns;
		if (m != 0) {
			// pad
			++pa->rows;
			for (int i = 0; i < pa->columns - m; ++i) {
				pa->array[n + i] = inf;
			}
		}
	}

	return pa;
}

AddIn xai_array_unique(
	Function(XLL_FP, "xll_array_unique", "ARRAY.UNIQUE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array to unique"),
		})
		.FunctionHelp("Remove consecutive duplicates from array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
<code>ARRAY.UNIQUE</code> calls the STL function 
<a href="https://en.cppreference.com/w/cpp/algorithm/unique"<code>std::unique</code></a>
on <code>array</code>. Just like <code>std::unique</code>, the array must be sorted
to guarantee all duplicate entries are removed.
)xyzyx")
	.SeeAlso({"ARRAY.SORT"})
);
_FPX* WINAPI xll_array_unique(_FPX* pa)
{
#pragma XLLEXPORT
	auto e = std::unique(begin(*pa), end(*pa));

	unsigned n = static_cast<unsigned>(e - begin(*pa));

	if (pa->rows == 1) {
		pa->columns = n;
	}
	else if (pa->columns == 1) {
		pa->rows = n;
	}
	else {
		pa->rows = 1 * (n % pa->columns) + n / pa->columns; // don't truncate
	}

	return pa;
}

int test_array()
{
	{
		_FPX a = { .rows = 1, .columns = 1 };
		HANDLEX h = xll_array_set(&a, 0);
		_FPX* pa = xll_array_get(h, TRUE);
		ensure(pa);
		ensure(pa->rows == 1);
		ensure(pa->columns == 1);
	}
	{
		_FPX a = { .rows = 1, .columns = 1 };
		a.array[0] = 2;
		HANDLEX h = xll_array_set(&a, 3);
		_FPX* pa = xll_array_get(h, TRUE);
		ensure(pa);
		ensure(xll_array_rows(pa) == 2);
		ensure(xll_array_columns(pa) == 3);
		ensure(xll_array_size(pa) == 6);
	}

	return TRUE;
}
int test_take()
{
	{
		FPX a = *xll_array_sequence(1, 10, 1);
		ensure(xll_array_take(0, a.get()) == nullptr);
		ensure(xll_array_take(5, a.get())->rows == 5);
		ensure(xll_array_take(5, a.get())->array[0] == 1);
		ensure(xll_array_take(-5, a.get())->rows == 5);
		ensure(xll_array_take(-5, a.get())->array[0] == 6);
		ensure(xll_array_take(100, a.get())->rows == 10);
		ensure(xll_array_take(-100, a.get())->rows == 10);
	}

	return TRUE;
}

Auto<OpenAfter> xaoa_test_array([]() {
	//_crtBreakAlloc = 2089;
	return TRUE
		&& test_array()
		&& test_take()
		;

	});

#endif // _DEBUG