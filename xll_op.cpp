// xll_op.cpp - operators
#include "fms_op.h"
#include "xll/xll/xll.h"

#ifndef CATEGORY
#define CATEGORY "XLL"
#endif

using namespace fms;
using namespace xll;

AddIn xai_add(
	Function(XLL_HANDLEX, "xll_add", "ADD")
	.Arguments({})
	.FunctionHelp("Binary addition.")
	.Category(CATEGORY)
);
HANDLEX WINAPI xll_add()
{
#pragma XLLEXPORT
	static HANDLEX handle_add = to_handle<const binop<double>>(&binop_add<double>);

	return handle_add;
}