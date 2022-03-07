// xll_monoid.cpp
#include "fms_monoid.h"
#include "xll/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY "Monoid"
#endif

using namespace xll;

AddIn xai_monoid_add(
	Function(XLL_HANDLEX, "xll_monoid_add", "MONOID.ADD")
	.Arguments({})
	.Category(CATEGORY)
	.FunctionHelp("Return handle to addition monoid.")
);
HANDLEX WINAPI xll_monoid_add()
{
#pragma XLLEXPORT
	return safe_handle<const fms::monoid<double>>(&fms::monoid_add<double>);
}

AddIn xai_monoid_op(
	Function(XLL_DOUBLE, "xll_monoid_op", "MONOID")
	.Arguments({
		Arg(XLL_HANDLEX, "op", "is a handle to a monoid."),
		Arg(XLL_DOUBLE, "x", "is the first argument."),
		Arg(XLL_DOUBLE, "y", "is the second argument."),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return op(x, y).")
);
double WINAPI xll_monoid_op(HANDLEX op, double x, double y)
{
#pragma XLLEXPORT
	auto op_ = safe_pointer<fms::monoid<double>>(op);
	if (op_) {
		return (*op_)(x, y);
	}

	return XLL_NAN;
}

AddIn xai_monoid_scan(
	Function(XLL_DOUBLE, "xll_monoid_scan", "MONOID.SCAN")
	.Arguments({
		Arg(XLL_HANDLEX, "op", "is a handle to a monoid."),
		Arg(XLL_DOUBLE, "x", "is a number."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return scan(x, y).")
);
double WINAPI xll_monoid_scan(HANDLEX op, double x)
{
#pragma XLLEXPORT
	handle<fms::monoid<double>> op_(op);
	if (op_) {
		OPER cc = Excel(xlCoerce, Excel(xlfCaller));

		return cc == 0 ? x : (*op_)(cc.as_num(), x);
	}

	return XLL_NAN;
}
