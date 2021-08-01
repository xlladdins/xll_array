// xll_monoid.cpp
#include "fms_monoid.h"
#include "xll/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY "Monoid"
#endif

using namespace xll;

HANDLEX handle_add;
Auto<OpenAfter> xaoa_handles([] {
	handle<fms::monoid<double>> add_(new fms::_monoid_add<double>{});
	handle_add = add_.get();

	return TRUE;
});

AddIn xai_monoid_add(
	Function(XLL_HANDLEX, "xll_monoid_add", "MONOID.ADD")
	.Arguments({})
	.Category(CATEGORY)
	.FunctionHelp("Return handle to addition monoid.")
);
HANDLEX WINAPI xll_monoid_add()
{
#pragma XLLEXPORT
	return handle_add;
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
	handle<fms::monoid<double>> op_(op);
	if (op_) {
		return (*op_)(x, y);
	}

	return std::numeric_limits<double>::quiet_NaN();
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

	return std::numeric_limits<double>::quiet_NaN();
}
