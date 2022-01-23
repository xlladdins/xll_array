// xll_array.h - array functions
#pragma once
#include "xll/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY "Array"
#endif 

namespace xll {

	// underlying pointer if 1 x 1 and handle to FPX
	inline FPX* ptr(_FPX* pa)
	{
		if (size(*pa) == 1) {
			handle<FPX> _a(pa->array[0]);
			if (_a) {
				return _a.ptr();
			}
		}

		return nullptr;
	}
	inline const FPX* ptr(const _FPX* pa)
	{
		if (size(*pa) == 1) {
			handle<FPX> _a(pa->array[0]);
			if (_a) {
				return _a.ptr();
			}
		}

		return nullptr;
	}

}
