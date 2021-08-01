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

#define MAKE_UNOP(Op, op) template<class X> \
	struct _unop_ ## Op : public unop<X> { \
		X _op(const X& x) const override { return op(x); }; \
	template<class X> inline constexpr auto unop_ ## Op = _unop_ ## Op<X>{}

	//MAKE_UNOP(neg, std::negate<X>{});

	template<class X>
	struct _unop_not : public unop<X> {
		~_unop_not()
		{ }
		X _op(const X& x) const override
		{
			return !x;
		}
	};
	template<class X>
	inline constexpr auto unop_not = _unop_not<X>{};

	template<class X>
	struct _unop_bit_not : public unop<X> {
		~_unop_bit_not()
		{ }
		X _op(const X& x) const override
		{
			return ~x;
		}
	};
	template<class X>
	inline constexpr auto unop_bit_not = _unop_bit_not<X>{};

	template<class X>
	class _unop_ge : public unop<X> {
		X y;
	public:
		_unop_ge(const X& y)
			: y(y)
		{ }
		~_unop_ge()
		{ }
		X _op(const X& x) const override
		{
			return x >= y;
		}
	};
	template<class X>
	inline constexpr auto unop_ge = _unop_ge<X>{};

	template<class X>
	struct binop {
		X operator()(const X& x, const X& y) const
		{
			return _op(x, y);
		}
		//virtual ~binop() { }
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

#undef MAKE_BINOP

	/*
	template <typename ... Args>
	auto f(Args&& ... args) {
		return[... args = std::forward<Args>(args)]{
			// use args
		};
	}

	// expr = null|un|bin
	// 1 + 2*_0 : expr = binop_add(nullop(1), binop_mul(nullop(2), _0))
	// expr(3) : auto _0 = nullop(3); return expr(); 

	template<class X> requires std::is_arithmetic_v<X>
	class expr {

	};
	*/

#ifdef _DEBUG
#include <cassert>

	template<class X>
	inline int op_test() {
		{
			//assert(unop_not<X>(2) == !2);
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