// fms_binop.h - hoist n-ary operators to a class
#pragma once
#include <algorithm>
#include <concepts>
#include <functional>
#include <limits>

namespace fms {

	// constant function
	template<class X>
	class nullop {
		X x;
	public:
		constexpr nullop(X x)
			: x(x)
		{ }
		X operator()() const
		{
			return X(x);
		}
	};

	template<class X>
	inline constexpr auto nullop_zero = nullop<X>(0);
	template<class X>
	inline constexpr auto nullop_one = nullop<X>(1);
	template<class X>
	inline constexpr auto nullop_max = nullop<X>(-std::numeric_limits<X>::max());
	template<class X>
	inline constexpr auto nullop_min = nullop<X>(std::numeric_limits<X>::max());

	// unary function
	template<class X>
	struct unop {
		X operator()(const X& x) const
		{
			return _op(x);
		}
	protected:
		virtual X _op(const X&) const = 0;
	};

	template<class X>
	struct _unop_identity : public unop<X> {
		X _op(const X& x) const override
		{
			return x;
		}
	};
	template<class X>
	inline constexpr auto unop_identity = _unop_identity<X>{};

	template<class X>
	struct _unop_neg : public unop<X> {
		X _op(const X& x) const override
		{
			return -x;
		}
	};
	template<class X>
	inline constexpr auto unop_neg = _unop_neg<X>{};

	template<class X>
	struct _unop_logical_not : public unop<X> {
		X _op(const X& x) const override
		{
			return static_cast<X>(!x);
		}
	};
	template<class X>
	inline constexpr auto unop_logical_not = _unop_logical_not<X>{};

	template<class X>
	struct _unop_bit_not : public unop<X> {
		X _op(const X& x) const override
		{
			return ~x;
		}
	};
	template<class X>
	inline constexpr auto unop_bit_not = _unop_bit_not<X>{};

	/*
	template<class X>
	class _unop_ge : public unop<X> {
		X y;
	public:
		_unop_ge(const X& y)
			: y(y)
		{ }
		X _op(const X& x) const override
		{
			return x >= y;
		}
	};
	template<class X>
	inline constexpr auto unop_ge = _unop_ge<X>{};
	*/

	template<class X>
	struct binop {
		X operator()(const X& x, const X& y) const
		{
			return _op(x, y);
		}
	protected:
		virtual X _op(const X&, const X&) const = 0;
	};

#define MAKE_BINOP(Op, op) template<class X> \
	struct _binop_ ## Op : public binop<X> { \
		X _op(const X& x, const X& y) const override { return op(x, y); } }; \
		template<class X> inline constexpr auto binop_ ## Op = _binop_ ## Op <X>{} \

	MAKE_BINOP(add, std::plus<X>{});
	MAKE_BINOP(sub, std::minus<X>{});
	MAKE_BINOP(mul, std::multiplies<X>{});
	MAKE_BINOP(div, std::divides<X>{});
	MAKE_BINOP(mod, std::modulus<X>{});
	MAKE_BINOP(max, std::max<X>);
	MAKE_BINOP(min, std::min<X>);

	MAKE_BINOP(logical_or, std::logical_or<X>{});
	MAKE_BINOP(logical_and, std::logical_and<X>{});
	MAKE_BINOP(bit_or, std::bit_or<X>{});
	MAKE_BINOP(bit_and, std::bit_and<X>{});
	MAKE_BINOP(bit_xor, std::bit_xor<X>{});

	MAKE_BINOP(lt, std::less<X>{});
	MAKE_BINOP(le, std::less_equal<X>{});
	MAKE_BINOP(gt, std::greater<X>{});
	MAKE_BINOP(ge, std::greater_equal<X>{});
	MAKE_BINOP(eq, std::equal_to<X>{});
	MAKE_BINOP(ne, std::not_equal_to<X>{});

#undef MAKE_BINOP

	/*
	template <typename ... Args>
	auto f(Args&& ... args) {
		return[... args = std::forward<Args>(args)]{
			// use args
		};
	}
	*/

	// expr = null|un|bin
	// 1 + 2*_0 : expr = binop_add(nullop(1), binop_mul(nullop(2), _0))
	// expr(3) : auto _0 = nullop(3); return expr(); 
	/*
	template<class X>
	inline auto bindl(const auto& op, const X& x)
	{
		return [x, &op](const X& y) { return op(x, y);  };
	}
	template<class X>
	inline auto bindr(const auto& op, const X& y)
	{
		return [y, &op](const X& x) { return op(x, y);  };
	}
	template<class X>
	inline auto bind(const auto& op, const X& x)
	{
		return [x, &op]() { return op(x); };
	}
	*/

#ifdef _DEBUG
#include <cassert>

	template<class X>
	inline int op_test() {
		{
			assert(unop_neg<X>(-2) == 2);
			assert(unop_logical_not<X>(2) == X(!2));
			assert(unop_bit_not<int>(2) == ~2);
		}
		{
			X x(2), y(1);

			assert(binop_add<X>(x, y) == x + y);

			assert(binop_max<X>(x, y) == ((x > y) ? x : y));
		}

		return 0;
	}

#endif // _DEBUG

}