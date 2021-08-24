// xll_array.cpp - Sample xll project.
#include <cmath>
//Uncomment to build for versions of Excel prior to 2007
//#define XLOPERX XLOPER
#include "xll_array.h"

#ifndef CATEGORY
#define CATEGORY "Array"
#endif

using namespace xll;

using xcstr = traits<XLOPERX>::xcstr;

AddIn xai_array_(
	Function(XLL_HANDLE, "xll_array_", "\\ARRAY")
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

Most array functions work in two modes: <em>function</em> and <em>command</em>.
If the first argument is an array then an new array is returned and the array function
has no side effects. If the first argument is a handle to an array then the function
modifies the in-memory array and returns the array handle. 
)")
);
HANDLEX WINAPI xll_array_(const _FPX* pa, WORD c)
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
.SeeAlso({ "\\ARRAY" })
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
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return pa;
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
			_FPX* pa = xll_array_get(ha, true);
			ensure(pa->array[0] == 2);
			ensure(pa->rows == 1);
			ensure(xll_array_rows(pa) == 1);
			ensure(pa->columns == 1);
			ensure(xll_array_columns(pa) == 1);
			ensure(xll_array_size(pa) == 1);

			_FPX* pb = xll_array_resize(pa, 2, 3);
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

AddIn xai_array_index(
	Function(XLL_FPX, "xll_array_index", "ARRAY.INDEX")
	.Arguments({
		Arg(XLL_FPX, "array", "is a array or handle to a array."),
		Arg(XLL_LPOPER, "rows", "are an array of rows to return."),
		Arg(XLL_LPOPER, "columns", "are an array of columns to return."),
		})
		.FunctionHelp("Return rows and columns of array.")
	.Category(CATEGORY)
	.Documentation(R"(
This works like <code>INDEX</code> for arrays except indices are cyclic.
If <code>rows</code> or <code>columns</code> are missing then all
rows or columns are returned.
)")
);
_FPX* WINAPI xll_array_index(_FPX* pa, LPOPER pr, LPOPER pc)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		unsigned r = pr->size();
		unsigned c = pc->size();

		if (size(*pa) == 1) {
			handle<FPX> h_(pa->array[0]);
			if (h_) {
				pa = h_->get();
			}
		}

		if (pr->is_missing()) {
			r = pa->rows;
		}
		if (pc->is_missing()) {
			c = pa->columns;
		}

		a.resize(r, c);

		for (unsigned i = 0; i < r; ++i) {
			unsigned ri = pr->is_missing() ? i : static_cast<unsigned>((*pr)[i].val.num);
			for (unsigned j = 0; j < c; ++j) {
				unsigned cj = pc->is_missing() ? j : static_cast<unsigned>((*pc)[j].val.num);
				a(i, j) = index(*pa, ri, cj);
			}
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
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
If <code>_incr</code> is greater than 1 return <code>_incr</code> values
from <code>start</code> to <code>stop</code>
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

		unsigned n;
		if (incr > 1) {
			n = static_cast<unsigned>(incr);
			incr = (stop - start) / (n - 1);
		}
		else {
			n = 1u + static_cast<unsigned>(fabs((stop - start) / incr));
		}

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

AddIn xai_array_take(
	Function(XLL_FPX, "xll_array_take", "ARRAY.TAKE")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is then number of items to take."),
		})
		.FunctionHelp("Take items from front (n > 0) or back (n < 0) of array.")
	.Category(CATEGORY)
	.Documentation(R"(
Take items from front (n > 0) or back (n < 0) of array
If <code>array</code> has more than one row then take <code>n</code> rows.
)")
);
_FPX* WINAPI xll_array_take(_FPX* pa, LONG n)
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
#if 0
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
		FPX b = *xll_array_sequence(1, 10, 1);
		_FPX r = { .rows = 1, .columns = 1 };
		r.array[0] = 2;
		WORD c = 3;
		r.array[0] = xll_array_set(&r, c);
		_FPX* pab = xll_array_join(&r, b.get());
		ensure(pab->rows == 1 + b.size());
		ensure(pab->columns == 1);
		ensure(pab->array[0] == r.array[0]);
		ensure(pab->array[1] == b[0]);
	}
	{
		FPX b = *xll_array_sequence(1, 10, 1);
		_FPX r = { .rows = 1, .columns = 1 };
		r.array[0] = 2;
		WORD c = 3;
		r.array[0] = xll_array_set(&r, c);
		_FPX* pab = xll_array_join(b.get(), &r);
		ensure(pab->rows == 1 + b.size());
		ensure(pab->columns == 1);
		ensure(pab->array[10] == r.array[0]);
		ensure(pab->array[0] == b[0]);
	}

	return TRUE;
}

#endif // _DEBUG
#endif // 0

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


#if 0
#ifdef _DEBUG

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
	static FPX a;
	a = *pa;
	FPX* _a = ptr(&a);

	if (n == 0) {
		n = _a->size();
		std::sort(_a->begin(), _a->end());
	}
	else if (n > 0) {
		if (n > _a->size()) {
			n = _a->size();
		}
		std::partial_sort(_a->begin(), _a->begin() + n, _a->end());
	}
	else if (n == -1) {
		n = _a->size();
		std::sort(_a->begin(), _a->end(), std::greater<double>{});
	}
	else {
		if (-n > _a->size()) {
			n = _a->size();
			n = -n;
		}
		std::partial_sort(_a->begin(), _a->begin() - n, _a->end(), std::greater<double>{});
	}

	if (_a->rows() == 1) {
		_a->resize(1, abs(n));
	}
	else if (_a->columns() == 1) {
		_a->resize(abs(n), 1);
	}
	else {
		double inf = std::numeric_limits<double>::infinity();
		if (n < 0) {
			inf = -inf;
		}
		n = abs(n);
		_a->resize(n / _a->columns(), n % _a->columns());
		unsigned m = _a->columns();
		if (m != 0) {
			// pad
			_a->resize(_a->rows() + 1, _a->columns());
			for (unsigned i = 0; i < _a->columns() - m; ++i) {
				(*_a)[n + i] = inf;
			}
		}
	}

	return a.get();
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
	static FPX a;
	a = *pa;
	FPX* _a = ptr(&a);
	auto e = std::unique(_a->begin(), _a->end());

	unsigned n = static_cast<unsigned>(e - _a->begin());

	if (pa->rows == 1) {
		_a->resize(1, n);
	}
	else if (pa->columns == 1) {
		_a->resize(n, 1);
	}
	else {
		_a->resize(n / pa->columns, n % pa->columns);
		unsigned m = _a->columns();
		if (m != 0) {
			// pad
			++pa->rows;
			for (unsigned i = 0; i < pa->columns - m; ++i) {
				(*_a)[n + i] = std::numeric_limits<double>::quiet_NaN();
			}
		}
	}

	return a.get();
}

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
	static FPX a;

	a = *pa;
	FPX* _a = ptr(&a);
	for (int i = 0; i < _a->size(); ++i) {
		(*_a)[i] = Excel(xlUDF, *pf, OPER((*_a)[i])).as_num();
	}

	return a.get();
}

int test_array()
{
	{
		_FPX a = { .rows = 1, .columns = 1 };
		HANDLEX h = xll_array_(&a, 0);
		_FPX* pa = xll_array_get(h, TRUE);
		ensure(pa);
		ensure(pa->rows == 1);
		ensure(pa->columns == 1);
	}
	{
		_FPX a = { .rows = 1, .columns = 1 };
		a.array[0] = 2;
		HANDLEX h = xll_array_(&a, 3);
		_FPX* pa = xll_array_get(h, TRUE);
		ensure(pa);
		ensure(xll_array_rows(pa) == 2);
		ensure(xll_array_columns(pa) == 3);
		ensure(xll_array_size(pa) == 6);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}

	return TRUE;
});

#endif // _DEBUG
#endif // 0


AddIn xai_array_diff(
	Function(XLL_FPX, "xll_array_diff", "ARRAY.DIFF")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_CSTRING, "_op", "is an optional binary operator to use. Default is minus.")
		})
	.FunctionHelp("Return adjacent differences of array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
Return <code>{a0, op(a1,a0), op(a2, a1),...}</code>.
)xyzyx")
);
_FPX* WINAPI xll_array_diff(_FPX* pa, xcstr op)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);

	if (_a) {
		pa = _a->get();
	}

	if (!*op or *op == '-') {
		pa = xll::diff<std::minus<double>>(pa);
	}
	if (!*op or *op == '/') {
		pa = xll::diff<std::divides<double>>(pa);
	}

	return pa;
}

AddIn xai_array_shift(
	Function(XLL_FPX, "xll_array_shift", "ARRAY.SHIFT")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_LONG, "n", "is the number of elements to shift.")
		})
	.FunctionHelp("Return shifted array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
If <code>n &gt; 0</code> then shift right. If <code>n &lt; 0</code> then shift left. 
)xyzyx")
);
_FPX* WINAPI xll_array_shift(_FPX* pa, LONG n)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);

	if (_a) {
		pa = _a->get();
	}

	shift(pa, n);

	return pa;
}
