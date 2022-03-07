// fms_iterable.h - iterator with operator bool() const 
#pragma once
#ifdef _DEBUG
#include <cassert>
#include <vector>
#endif
#include <compare>
#include <concepts>
#include <iterator>
#include <limits>
#include <type_traits>

namespace fms::iterable {

	template<class I>
	concept iterable = requires (I i) {
		//std::is_base_of_v<std::forward_iterator_tag, typename I::interator_category>;
		//typename I::iterator_concept;
		typename I::iterator_category;
		typename I::difference_type;
		typename I::value_type;
		typename I::reference;
		{ i.operator==(i) };
//		{ i.begin() } -> std::same_as<I>;
		{ i.end() }; // maybe not same as I
		{ !!i } -> std::same_as<bool>;
		{  *i } -> std::convertible_to<typename I::value_type>;
		{ ++i } -> std::same_as<I&>;
	}
	&& std::copyable<I>
	// && default_constructable ??? ...
	;

	template<iterable I>
	inline auto begin(const I& i)
	{
		return i;
	}
	template<iterable I>
	inline auto end(const I& i)
	{
		return i.end();
	}

	// t, t + dT, ...
	template<class T, long dT = 1>
	class iota {
		T t;
	public:
		// !!! random access iterator
		using iterator_category = std::forward_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using reference = T&;

		iota(const T& t = 0)
			: t(t)
		{ }

		bool operator==(const iota&) const = default;

		iota begin() const
		{
			return *this;
		}
		iota end() const
		{
			return iota(std::numeric_limits<T>::max());
		}

		explicit operator bool() const
		{
			return true;
		}
		value_type operator*() const
		{
			return t;
		}
		reference operator*()
		{
			return t;
		}
		iota& operator++()
		{
			t += dT;

			return *this;
		}
		iota operator++(int)
		{
			iota i(*this);

			operator++();

			return i;
		}
	};

#ifdef _DEBUG
	inline int iota_test()
	{
		{
			iota<int> i;
			auto i2{ i };
			assert(i2 == i);
			i = i2;
			assert(!(i != i2));
		
			assert(i);
			assert(0 == *i);
			++i;
			assert(1 == *i);

			*i = 3;
			assert(3 == *i);
			assert(3 == *i++);
			assert(4 == *i);
		}
		{
			iota<int, 0> i;
			assert(i);
			assert(0 == *i);
			++i;
			assert(0 == *i);
		}
		{
			iota<int, -1> i;
			assert(0 == *i);
			assert(-1 == *++i);
		}
		{
			int i = 0;
			for (auto is : iota<int>{}) {
				assert(i == is);
				++i;
				if (i > 3) {
					break;
				}
			}
		}

		return 0;
	}
#endif // _DEBUG

	template<class T>
	using constant = iota<T, 0>;

	// equal elements
	template<iterable I, iterable J>
	inline bool equal(I i, J j)
	{
		while (i and j) {
			if (*i != *j) {
				return false;
			}
			++i;
			++j;
		}

		return !i and !j;
	}

	// size(j, size(i)) = size(j) + size(i)
	template<iterable I>
	inline size_t size(I i, size_t n = 0)
	{
		while (i) {
			++n;
			++i;
		}

		return n;
	}

	template<iterable I>
	inline I drop(size_t n, I i)
	{
		while (i and n--) {
			++i;
		}

		return i;
	}

	template<class I>
	class take {
		size_t n;
		I i;
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename I::difference_type;
		using value_type = typename I::value_type;
		using reference = typename I::reference;

		take()
			: n(0)
		{ }
		take(size_t n, const I& i)
			: n(n), i(i)
		{ }

		bool operator==(const take&) const = default;
		take begin() const
		{
			return *this;
		}
		take end() const
		{
			return take(0, i.end());
		}

		explicit operator bool() const
		{
			return n != 0;
		}

		value_type operator*() const
		{
			return *i;
		}
		reference operator*()
		{
			return *i;
		}
		take& operator++()
		{
			if (n) {
				--n;
				++i;
			}

			return *this;
		}
		take operator++(int)
		{
			take t(*this);

			operator++();

			return t;
		}
	};
	template<class I>
	inline take<I> take_(size_t n, I i)
	{
		return take<I>(n, i);
	}

#ifdef _DEBUG

	inline int take_test()
	{
		{
			auto t = take(3, iota<int>{});
			assert(t);
			auto t2{ t };
			assert(t2);
			assert(t2 == t);
			t = t2;
			assert(!(t != t2));
			assert(3 == size(t));
		}
		{
			auto i2 = take(2, iota<int>{});
			auto i3 = take(3, iota<int>{});
			assert(5 == size(i2, size(i3)));
		}

		return 0;
	}
#endif // _DEBUG
#if 0
	// iterable from array
	template<class T>
	class array {
		T* b;
		T* e;
	public:
		/// TODO: make random access 
		using iterator_category = std::forward_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using type = typename array<T>;

		array() = default;
		array(T* b, T* e)
			: b(b), e(e)
		{ }
		template<size_t N>
		array(T(&a)[N])
			: array(a, a + N)
		{ }
		array(const array&) = default;
		array& operator=(const array&) = default;
		~array() = default;

		bool operator==(const array&) const = default;

		array begin() const
		{
			return array(b, e);
		}
		array end() const
		{
			return array(e, e);
		}

		explicit operator bool() const
		{
			return b != e;
		}
		value_type operator*() const
		{
			return *b;
		}
		reference operator*()
		{
			return *b;
		}
		array& operator++()
		{
			if (b != e) {
				++b;
			}

			return *this;
		}
	};

#ifdef _DEBUG

	inline int array_test()
	{
		{
			int i[] = { 1,2,3 };
			auto a = array(i);
			assert(a);
			auto a2{ a };
			assert(a2 == a);
			a = a2;
			assert(!(a != a2));

			auto b = begin(a);
			assert(b);
			assert(*b == 1);
			assert(*++b == 2);
			++b;
			++b;
			assert(!b);
			assert(b == end(a));

			assert(*a == 1);
			++a;
			assert(a);
			assert(*a == 2);
			assert(*++a == 3);
			++a;
			assert(!a);
			++a;
			assert(!a);
		}
		{
			int i[] = { 1,2,3 };
			auto a = array(i);
			int j = 1;
			for (auto aj : a) {
				assert(aj == j);
				++j;
			}
		}
		{
			int i[] = { 1,2,3 };
			double j[] = { 1, 2, 3 };
			auto ai = array(i);
			auto aj = array(j);
			assert(equal(ai, aj));
			assert(!equal(drop(1, ai), aj));
			assert(equal(drop(2, ai), drop(2, aj)));
			assert(equal(drop(3, ai), drop(3, aj)));		}

		return 0;
	}

#endif // _DEBUG
#endif

	// use operator I() ???
	template<class I>
	class iterator {
		I b, e;
	public:
		using iterator_category = std::iterator_traits<I>::iterator_category;
		using difference_type = std::iterator_traits<I>::difference_type;
		using value_type = std::iterator_traits<I>::value_type;
		using reference = std::iterator_traits<I>::reference;

		iterator() = default;
		iterator(const I& b, const I& e)
			: b(b), e(e)
		{ }
		iterator(const iterator&) = default;
		iterator& operator=(const iterator&) = default;
		~iterator() = default;

		bool operator==(const iterator&) const = default;

		iterator begin() const
		{
			return iterator(b, e);
		}
		iterator end() const
		{
			return iterator(e, e);
		}

		// operator I()

		explicit operator bool() const
		{
			return b != e;
		}
		value_type operator*() const
		{
			return *b;
		}
		reference operator*()
		{
			return *b;
		}
		iterator& operator++()
		{
			if (b != e) {
				++b;
			}
			
			return *this;
		}
	};

	template<class T, size_t N>
	inline auto array(T(&a)[N])
	{
		return iterator<T*>(a, a + N);
	}

#ifdef _DEBUG

	inline int iterator_test()
	{
		{
			int i[] = { 1,2,3 };
			auto ia = array(i);
			assert(ia);
			assert(3 == size(ia));

		}
		{
			std::vector<int> i{ 1,2,3 };
			auto ia = iterator(i.begin(), i.end());
			auto ia2{ ia };
			assert(ia2 == ia);
			ia = ia2;
			assert(!(ia != ia2));
			assert(ia);
			assert(*ia == 1);
			++ia;
			assert(ia);
			assert(*ia == 2);
			assert(*++ia == 3);
			++ia;
			assert(!ia);
			++ia;
			assert(!ia);
		}

		return 0;
	}

	// advance interable until relation is satisfied
	template<class R, iterable I, class T = I::value_type>
	inline I relation(I i, T t)
	{
		while (!R(*i, t)) {
			++i;
		}

		return i;
	};

	template<iterable I, typename T = I::value_type>
	inline I ge(I i, T t)
	{
		return relation<std::greater_equal<T>, I, T>(i, t);
	};


#endif // _DEBUG

	// intersection
	template<iterable I>
	class cap {
		I i;
		I j;
		void advance()
		{
			if (i and j and *i != *j) {
				if (*i < *j) {
					i = ge(i, *j);
				}
				else if (*j < *i) {
					j = ge(j, *i);
				}
			}
		}
	public:
		using iterator_category = std::iterator_traits<I>::iterator_category;
		using difference_type = std::iterator_traits<I>::difference_type;
		using value_type = std::iterator_traits<I>::value_type;
		using reference = std::iterator_traits<I>::reference;

		cap() = default;
		cap(const I& i, const I& j)
			: i(i), j(j)
		{
			advance();
		}
		cap(const cap&) = default;
		cap& operator=(const cap&) = default;
		~cap()
		{ }

		bool operator==(const cap&) const = default;
		cap begin() const
		{
			return *this;
		}
		cap end() const
		{
			return cap(i.end(), j.end());
		}
		
		explicit operator bool() const
		{
			return i and j;
		}

		value_type operator*() const
		{
			return *i;
		}
		reference operator*()
		{
			return *i;
		}
		cap& operator++()
		{
			++i;
			++j;
			advance();

			return *this;
		}
	};

}