// xll_array_unique.cpp - Remove adjacent duplicates
#include <algorithm>
#include "xll_array.h"

using namespace xll;

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
)xyzyx")
.SeeAlso({ "ARRAY.SORT" })
);
_FP12* WINAPI xll_array_unique(_FP12* pa)
{
#pragma XLLEXPORT
	FPX* _a = ptr(pa);
	if (_a) {
		pa = _a->get();
	}

	auto e = std::unique(begin(*pa), end(*pa));

	unsigned n = static_cast<unsigned>(e - begin(*pa));

	if (pa->rows == 1) {
		pa->columns = n;
	}
	else if (pa->columns == 1) {
		pa->rows = n;
	}

	return pa;
}
