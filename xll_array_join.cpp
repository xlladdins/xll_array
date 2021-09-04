// xll_array_join.cpp - Join arrays
#include "xll_array.h"

using namespace xll;

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
.SeeAlso({ "\\ARRAY", "ARRAY.TAKE" })
);
_FPX* WINAPI xll_array_join(_FPX* pa1, _FPX* pa2)
{
#pragma XLLEXPORT
	static xll::FPX a;

	try {
		FPX* _a1 = ptr(pa1);
		if (_a1) {
			pa1 = _a1->get();
		}
		FPX* _a2 = ptr(pa2);
		if (_a2) {
			pa2 = _a2->get();
		}

		a = *pa1;
		unsigned n = a.size();

		// prefer columns
		if (a.columns() == 1) {
			a.resize(n + size(*pa2), 1);
		}
		else if (a.rows() == 1) {
			a.resize(1, n + size(*pa2));
		}
		else {
			a.resize((n + size(*pa2)) / a.columns(), a.columns());
		}

		std::copy(begin(*pa2), begin(*pa2) + a.size() - n, a.begin() + n);
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

_FPX* WINAPI xll_array_sequence(double start, double stop, double incr);
HANDLEX WINAPI xll_array_(const _FPX* pa, WORD c);

int xll_array_join_test()
{
	{
		FPX a = *xll_array_sequence(1, 3, 1); // {1,2,3}
		FPX b = *xll_array_sequence(4, 7, 1); // {4,5,6,7}
		_FPX* pab = xll_array_join(a.get(), b.get());
		ensure(pab->rows == a.size() + b.size());
		ensure(pab->columns == 1);
		ensure(pab->array[0] == a[0]);
		ensure(pab->array[3] == b[0]);
	}

	return TRUE;
}
Auto<OpenAfter> xaoa_array_join_test(xll_array_join_test);

#endif // _DEBUG