// fms_monoid.h - NVI moniod class
#pragma once
#include <iterator>
#include <type_traits>
#include "fms_op.h"

namespace fms {

	template<class X> requires std::is_arithmetic_v<X>
	struct monoid {
		X operator()() const
		{
			return _op();
		}
		X operator()(const X& x, const X& y) const
		{
			return _op(x, y);
		}
	private:
		virtual X _op() const = 0;
		virtual X _op(const X&, const X&) const = 0;
	};
	
	template<class X> requires std::is_arithmetic_v<X>
	struct _monoid_add : public monoid<X> {
		X _op() const override
		{
			return nullop_zero<X>();
		}
		X _op(const X& x, const X& y) const override
		{
			return binop_add<X>(x, y);
		}
	};
	template<class X>
	inline constexpr auto monoid_add = _monoid_add<X>{};
	// sub, mul, ...

	template<class X>
	inline X fold(const monoid<X>& m)
	{
		return m();
	}
	template<class X>
	inline X fold(const monoid<X>&, X x)
	{
		return x;
	}
	template<class X, class ...Xs>
	inline X fold(const monoid<X>& m, X x, Xs ...xs)
	{
		return m(x, fold<X>(m, xs...));
	}

	template<class I, class T = I::value_type>
	class foldable {
		const monoid<T>& m;
		I i;
		T t;
	public:
		using iterator_category = typename std::iterator_traits<I>::iterator_category;
		using value_type = T;
		foldable(const monoid<T>& m, const I& i) // I&& i???
			: m(m), i(i), t(m())
		{ }
		explicit operator bool() const
		{
			return i;
		}
		value_type operator*() const
		{
			return t;
		}
		foldable& operator++()
		{
			if (operator bool()) {
				t = m(t, *i);
				++i;
			}

			return *this;
		}
	};

#ifdef _DEBUG
#include <cassert>

	template<class X>
	inline int monoid_test()
	{
		{
			assert(monoid_add<X>() == 0);
			assert(monoid_add<X>(1, 2) == 1 + 2);
		}
		{
			assert(fold<X>(monoid_add<X>) == 0);
			assert(fold<X>(monoid_add<X>, 1) == 1);
			assert(fold<X>(monoid_add<X>, 1, 2) == 1 + 2);
			assert(fold<X>(monoid_add<X>, 1, 2, 3) == 1 + 2 + 3);
		}
		{
			X i[] = { 1,2,3 };
			foldable f(monoid_add<X>, &i[0]);
			assert(f);
			assert(*f == 0);
			++f;
			assert(f);
			assert(*f == 1);
			++f;
			assert(f);
			assert(*f == 3);
			++f;
			assert(f);
			assert(*f == 6);
		}

		return 0;
	}
#endif // _DEBUG
}
