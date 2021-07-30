// fms_binop.h - hoist n-ary operators to a class
#pragma once
#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>

namespace fms {

	template<class X> requires std::is_arithmetic_v<X>
	class nullop {
		X x;
	public:
		constexpr nullop(X x)
			: x(x)
		{ }
		X operator()() const
		{
			return X(0);
		}
	};

	template<class X>
	inline constexpr auto nullop_zero = nullop<X>(0);
	template<class X>
	inline constexpr auto nullop_one = nullop<X>(1);
	template<class X>
	inline constexpr auto nullop_max = nullop<X>(std::numeric_limits<X>::max());
	template<class X>
	inline constexpr auto nullop_min = nullop<X>(std::numeric_limits<X>::min());

	template<class X> requires std::is_arithmetic_v<X>
	struct unop {
		X operator()(const X& x) const
		{
			return _op(x);
		}
	protected:
		virtual X _op(const X&) const = 0;
	};

	template<class X> requires std::is_arithmetic_v<X>
	struct unop_neg : public unop<X> {
		X _op(const X& x) const override
		{
			return -x;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct unop_not : public unop<X> {
		X _op(const X& x) const override
		{
			return !x;
		}
	};
	template<class X> requires std::is_integral_v<X>
	struct unop_bit_not : public unop<X> {
		X _op(const X& x) const override
		{
			return ~x;
		}
	};

	template<class X> requires std::is_arithmetic_v<X>
	struct binop {
		X operator()(const X& x, const X& y) const
		{
			return _op(x, y);
		}
	protected:
		virtual X _op(const X&, const X&) const = 0;
	};

	template<class X> requires std::is_arithmetic_v<X>
	struct _binop_add : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x + y;
		}
	};
	template<class X>
	inline constexpr auto binop_add = _binop_add<X>{};

	template<class X> requires std::is_arithmetic_v<X>
	struct binop_sub : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x - y;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct binop_mul : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x * y;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct binop_div : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x / y;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct binop_mod : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x % y;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct _binop_max : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return std::max(x, y);
		}
	};
	template<class X>
	inline constexpr auto binop_max = _binop_max<X>{};

	template<class X> requires std::is_arithmetic_v<X>
	struct binop_min : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return std::min(x, y);
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct binop_logical_or : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x || y;
		}
	};
	template<class X> requires std::is_arithmetic_v<X>
	struct binop_logical_and : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x & y;
		}
	};
	template<class X> requires std::is_integral_v<X>
	struct binop_bit_or : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x | y;
		}
	};
	template<class X> requires std::is_integral_v<X>
	struct binop_bit_and : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x & y;
		}
	};
	template<class X> requires std::is_integral_v<X>
	struct binop_bit_xor : public binop<X> {
		X _op(const X& x, const X& y) const override
		{
			return x ^ y;
		}
	};

#ifdef _DEBUG
#include <cassert>

	template<class X>
	inline int op_test() {
		{
			X x(2), y(1);

			assert(binop_add<X>(x, y) == x + y);

			assert(binop_max<X>(x, y) == ((x > y) ? x : y));
		}

		return 0;
	}

#endif // _DEBUG

}