// xll_array_acf.cpp - Auto covariance/correlation
#include <numeric>
#include "xll_array.h"

using namespace xll;

// covariance
double cov(unsigned n, const double* x, const double* y, double x_, double y_)
{
	double c = 0;

	for (unsigned i = 0; i < n; ++i) {
		c += (x[i] - x_) * (y[i] - y_);
	}

	return c / n;
}

AddIn xai_array_acf(
	Function(XLL_FPX, "xll_array_acf", "ARRAY.ACF")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array or handle to an array."),
		Arg(XLL_BOOL, "_correlation", "is an optional flag indicating correlations should be returned.")
		})
	.FunctionHelp("Return auto covariance of the array.")
	.Category(CATEGORY)
	.Documentation(R"xyzyx(
)xyzyx")
);
_FPX* WINAPI xll_array_acf(_FPX* pa, BOOL corr)
{
#pragma XLLEXPORT
	static FPX acf;

	try {
		FPX* _a = ptr(pa);
		if (_a) {
			pa = _a->get();
		}

		acf.resize(pa->rows, pa->columns);
		unsigned n = acf.size();

		// (a[0] + ... a[n - i - 1])/(n - i)
		double ai_ = std::accumulate(begin(*pa), end(*pa), 0.) / n;
		// (a[i] + ... + a[n-1])/(n - i)
		double _ai = ai_;

		acf[0] = cov(n, &pa->array[0], &pa->array[0], ai_, _ai);

		// cov(a, a + i)
		for (unsigned i = 1; i < n; ++i) {
			ai_ *= n - i + 1;
			ai_ -= pa->array[n - i];
			ai_ /= (n - i);

			_ai *= n - i + 1;
			_ai -= pa->array[i - 1];
			_ai /= (n - i);

			acf[i] = cov(n - i, &pa->array[0], &pa->array[i], ai_, _ai);
			if (corr) {
				acf[i] /= acf[0];
			}
		}
		if (corr) {
			acf[0] = 1;
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return acf.get();
}
