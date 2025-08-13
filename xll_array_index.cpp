// xll_array_index.cpp - Project rows/columns
#include "xll_array.h"

using namespace xll;

AddIn xai_array_index(
	Function(XLL_FP, "xll_array_index", "ARRAY.INDEX")
	.Arguments({
		Arg(XLL_FP, "array", "is a array or handle to a array."),
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
_FP12* WINAPI xll_array_index(_FP12* pa, LPOPER pr, LPOPER pc)
{
#pragma XLLEXPORT
	static FPX a;

	try {
		unsigned r = size(*pr);
		unsigned c = size(*pc);

		if (size(*pa) == 1) {
			handle<FPX> h_(pa->array[0]);
			if (h_) {
				pa = h_->get();
			}
		}

		if (isMissing(*pr)) {
			r = pa->rows;
		}
		if (isMissing(*pc)) {
			c = pa->columns;
		}

		a.resize(r, c);

		for (unsigned i = 0; i < r; ++i) {
			unsigned ri = isMissing(*pr) ? i : static_cast<unsigned>((*pr)[i].val.num);
			for (unsigned j = 0; j < c; ++j) {
				unsigned cj = isMissing(*pc) ? j : static_cast<unsigned>((*pc)[j].val.num);
				a(i, j) = index(*pa, ri, cj);
			}
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return a.get();
}
