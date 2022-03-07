// fms_monoid.h - NVI moniod class
#pragma once
#include <iterator>
#include <type_traits>
#include "fms_op.h"

namespace fms {

	inline const char monoid_doc[] = R"xyzyx(
A <em>moniod</em> is a set with a binary operation
that is associative and has an identity.
)xyzyx";
	template<class X>
	struct monoid {
		X operator()() const
		{
			return _op();
		}
		X operator()(const X& x, const X& y) const
		{
			return _op(x, y);
		}
		constexpr virtual ~monoid() { }
	private:
		virtual X _op() const = 0;
		virtual X _op(const X&, const X&) const = 0;
	};

	template<class X>
	class _monoid : public monoid<X> {
		const nullop<X>& id;
		const binop<X>& op;
	public:
		constexpr _monoid(const nullop<X>& id, const binop<X>& op) noexcept
			: id(id), op(op)
		{ }
		constexpr ~_monoid() noexcept { }
		X _op() const override
		{
			return id();
		}
		X _op(const X& x, const X& y) const override
		{
			return op(x, y);
		}
	};
	
	template<class X>
	constexpr auto monoid_add = _monoid<X>(nullop_zero<X>, binop_add<X>);

	template<class X>
	constexpr auto monoid_mul = _monoid<X>(nullop_one<X>, binop_mul<X>);
	
	template<class X>
	constexpr auto monoid_max = _monoid<X>(nullop_max<X>, binop_max<X>);
	
	template<class X>
	constexpr auto monoid_min = _monoid<X>(nullop_min<X>, binop_min<X>);

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
 			auto u = monoid_max<X>();
			assert(monoid_max<X>(u, X(1)) == X(1));
		}
		{
			assert(fold<X>(monoid_add<X>) == 0);
			assert(fold<X>(monoid_add<X>, 1) == 1);
			assert(fold<X>(monoid_add<X>, 1, 2) == 1 + 2);
			assert(fold<X>(monoid_add<X>, 1, 2, 3) == 1 + 2 + 3);
		}
		{
			assert(fold<X>(monoid_mul<X>) == 1);
			assert(fold<X>(monoid_mul<X>, 1) == 1);
			assert(fold<X>(monoid_mul<X>, 1, 2) == 1 * 2);
			assert(fold<X>(monoid_mul<X>, 1, 2, 3) == 1 * 2 * 3);
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
