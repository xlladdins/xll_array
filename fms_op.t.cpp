#include "fms_op.h"

using namespace fms;

int fms_binop_test_i = fms::op_test<int>();
int fms_binop_test_d = fms::op_test<double>();

int fms_op_test()
{
	{
	}

	return 0;
}
int fms_op_test_ = fms_op_test();