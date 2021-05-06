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
		Arg(XLL_FP, "array", "is an array or handle to an array of numbers."),
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
HANDLEX WINAPI xll_array_set(_FPX* pa, WORD c)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		_FPX& a = *_ptr(pa);
		if (c != 0) {
			ensure(size(a) == 1 || !"\\ARRAY: first argument must be scalar if second argument is not zero");
			handle<FPX> h_(new FPX(static_cast<unsigned>(a.array[0]), c));
			ensure(h_);
			h = h_.get();
		}
		else {
			handle<FPX> h_(new FPX(a));
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
If <code>array</code> is a handle this function resizes the in-memory array and
returns its handle, otherwise the resized array is returned.
)")
);
_FPX* WINAPI xll_array_resize(_FPX* pa, LONG r, LONG c)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		ptr(&a)->resize(r, c);
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
		.FunctionHelp("Return the number of columns of an array.")
	.Category(CATEGORY)
	.Documentation(R"(
Return the number of columns of an array.
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
Return the number of rows times the number of columns of an array.
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
<code>\ARRAY</code>. By default the handle is checked to
ensure the array was created by a previous call to <code>\ARRAY</code>.
)")
	.SeeAlso({"\\ARRAY"})
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
#endif // _DEBUG

AddIn xai_array_take(
	Function(XLL_FPX, "xll_array_take", "ARRAY.TAKE")
	.Arguments({
		Arg(XLL_LONG, "n", "is then number of items to take."),
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Take items from front (n > 0) or back (n < 0) of array.")
	.Category(CATEGORY)
	.Documentation(R"(
Take items from front (n > 0) or back (n < 0) of array
If <code>array</code> has more than one column then take <code>n</code> rows.
If <code>array</code> is a handle then take from the in-memory array
and return its handle.
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

#ifdef _DEBUG

int test_array_take()
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

#endif // _DEBUG

// array.drop

AddIn xai_array_join(
	Function(XLL_FP, "xll_array_join", "ARRAY.JOIN")
	.Arguments({
		Arg(XLL_FP, "array1", "is an array or handle to an array."),
		Arg(XLL_FP, "array2", "is an array."),
		})
		.FunctionHelp("Return the concatenation of two arrays.")
	.Category(CATEGORY)
	.Documentation(R"(
Join two arrays. 
The shape of the returned array is determined by the shape of the first array.
If the first array is a single column then so is the result. If the first
array is a single row then so is the result.
If the first array has more than one row the resulting array has the same
number of columns as the first array. If the size of the second array 
is not a multiple of the number of columns of the first array then
the last partial row is omitted.
<p>
If <code>array1</code> is a handle then <code>array2</code> is appended to
the associated in-memory array and the handle to the first array is returned. 
If <code>array2</code> is a handle and <code>array1</code> is not, 
then <code>array1</code> is prepended to the 
in-memory array and the handle for the second array is returned.
In this case the shape of the result is determined by the second 
array as described above.
<p>
This function is useful for buffering data produced by Excel into memory. For example,
<code>ARRAY.TAKE(4, ARRAY.JOIN(RAND(), handle))</code> returns a handle to (at most)
the last 4 values generated by <code>RAND()</code>.
Use <code>ARRAY.TAKE(-4, ARRAY.JOIN(handle, RAND())</code> to get the same items
in reverse order more efficiently.
)")
	.SeeAlso({"\\ARRAY", "ARRAY.TAKE"})
);
_FPX* WINAPI xll_array_join(_FPX* pa1, _FPX* pa2)
{
#pragma XLLEXPORT
	static xll::FPX a;

	try {
		// pa1 is a handle or pa2 is not
		if (ptr(pa1) != nullptr or ptr(pa2) == nullptr) {
			a = *pa1; // copy either handle or array
			FPX* pa = ptr(&a);
			unsigned n = pa->size();
			const _FPX& a2 = *_ptr(pa2);

			// prefer columns
			if (pa->columns() == 1) {
				pa->resize(n + size(a2), 1);
			}
			else if (pa->rows() == 1) {
				pa->resize(1, n + size(a2));
			}
			else {
				pa->resize((n + size(a2)) / pa->columns(), pa->columns());
			}

			std::copy(begin(a2), begin(a2) + pa->size() - n, pa->begin() + n);
		}
		else {
			// pa1 is not a handle and pa2 is
			a = *pa2;
			FPX* pa = ptr(&a);
			unsigned n = pa->size();
			const _FPX& a1 = *_ptr(pa1);

			// prefer columns
			if (pa->columns() == 1) {
				pa->resize(size(a1) + n, 1);
			}
			else if (pa->rows() == 1) {
				pa->resize(1, size(a1) + n);
			}
			else {
				pa->resize((size(a1) + n) / pa->columns(), pa->columns());
			}

			auto i = std::copy_backward(pa->begin(), pa->begin() + n, pa->end());
			std::copy(begin(a1), begin(a1) + (i - pa->begin()), pa->begin());
		}

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("ARRAY.JOIN: unknown exception");
	}

	return a.get();
}
#ifdef _DEBUG

int test_array_join()
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

#endif // _DEBUG

AddIn xai_array_mask(
	Function(XLL_FPX, "xll_array_mask", "ARRAY.MASK")
	.Arguments({
		Arg(XLL_FPX, "mask", "is a mask or handle to a mask to be applied to array."),
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		})
		.FunctionHelp("Return array values where corresponding mask is non-zero.")
	.Category(CATEGORY)
	.Documentation(R"(
If <code>mask</code> is smaller than <code>array</code> then it is
applied using cyclic indices. If <code>array</code> is a handle then
the handle to the masked array is returned.
)")
);
_FPX* WINAPI xll_array_mask(_FPX* pm, _FPX* pa)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		a = *pa;
		FPX* _a = ptr(&a);
		_FPX* _m = _ptr(pm);
		array_mask(_m, _a->get());
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


AddIn xai_array_sort(
	Function(XLL_FP, "xll_array_sort", "ARRAY.SORT")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or handle to an array to sort"),
		Arg(XLL_LONG, "_count", "is an optional number of elements to partial sort. Default is 0."),
		})
		.FunctionHelp("Sort _count elements of array in increasing (_count >= 0) or decreasing (n < 0) order.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Sort all elements of <code>array</code> in increasing numerical order when 
<code>_count = 0</code>. 
If<code>_count = -1</code> sort in decreasing order. If <code>_count > 0</code> only sort
the smallest <code>_count</code> values. If <code>_count < -1</code> only sort
the largest <code>-_count</code> values.
If <code>array</code> is not a row or column then partial elements of
the last row are set to \(\infty\) or \(-\infty\) if the sort is
increasing or decreasing, respecively.
<p>
If <code>array</code> is a handle return a handle to the sorted array.
)xyzyx")
);
_FPX* WINAPI xll_array_sort(_FPX* pa, LONG n)
{
#pragma XLLEXPORT
	_FPX& a = *_ptr(pa);

	if (n == 0) {
		n = size(a);
		std::sort(begin(a), end(a));
	}
	else if (n > 0) {
		if (static_cast<unsigned>(n) > size(a)) {
			n = size(a);
		}
		std::partial_sort(begin(a), begin(a) + n, end(a));
	}
	else if (n == -1) {
		n = size(a);
		std::sort(begin(a), end(a), std::greater<double>{});
	}
	else {
		if (static_cast<unsigned>(-n) > size(a)) {
			n = size(a);
			n = -n;
		}
		std::partial_sort(begin(a), begin(a) - n, end(a), std::greater<double>{});
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
		unsigned m = n % pa->columns;
		if (m != 0) {
			// pad
			++pa->rows;
			for (unsigned i = 0; i < pa->columns - m; ++i) {
				pa->array[n + i] = inf;
			}
		}
	}

	return pa;
}

AddIn xai_array_unique(
	Function(XLL_FP, "xll_array_unique", "ARRAY.UNIQUE")
	.Arguments({
		Arg(XLL_FP, "array", "is an array or a handle to an array"),
		})
		.FunctionHelp("Remove consecutive duplicates from array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
This function calls the STL function 
<a href="https://en.cppreference.com/w/cpp/algorithm/unique"<code>std::unique</code></a>
on <code>array</code>. Just like <code>std::unique</code>, the array must be sorted
to guarantee all duplicate entries are removed.
<p>
If <code>array</code> is not a row or column then partial elements of
the last row are set to NaN.
)xyzyx")
	.SeeAlso({"ARRAY.SORT"})
);
_FPX* WINAPI xll_array_unique(_FPX* pa)
{
#pragma XLLEXPORT
	_FPX& a = *_ptr(pa);
	auto e = std::unique(begin(a), end(a));

	unsigned n = static_cast<unsigned>(e - begin(a));

	if (pa->rows == 1) {
		pa->columns = n;
	}
	else if (pa->columns == 1) {
		pa->rows = n;
	}
	else {
		pa->rows = n / pa->columns; // might truncate
		unsigned m = n % pa->columns;
		if (m != 0) {
			// pad
			++pa->rows;
			for (unsigned i = 0; i < pa->columns - m; ++i) {
				pa->array[n + i] = std::numeric_limits<double>::quiet_NaN();
			}
		}
	}

	return pa;
}

#ifdef _DEBUG
Auto<OpenAfter> xaoa_test_array([]() {
	//_crtBreakAlloc = 2089;
	try {
		test_array();
		test_array_take();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
});

#endif // _DEBUG