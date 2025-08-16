// xll_array.h - array functions
#pragma once
#include "xll24/include/xll.h"

#ifndef CATEGORY
#define CATEGORY L"Array"
#endif 

namespace xll {

	// underlying pointer if 1 x 1 and handle to FPX
	inline FPX* ptr(_FP12* pa)
	{
		if (size(*pa) == 1) {
			handle<FPX> a_(pa->array[0]);
			if (a_) {
				return a_.ptr();
			}
		}

		return nullptr;
	}
	// const version of ptr
	inline const FPX* ptr(const _FP12* pa)
	{
		if (size(*pa) == 1) {
			handle<FPX> _a(pa->array[0]);
			if (_a) {
				return _a.ptr();
			}
		}

		return nullptr;
	}

	_FP12* take(_FP12* pa, int n)
	{
		if (pa) {
			n = std::clamp(n, 0, size(*pa) - 1);
		}
		return pa;
	}
}
