// fms_moments.h
#pragma once
#include <valarray>
#include <vector>
#include "fms_monoid.h"

namespace fms {

	inline const char fms_monoid_average_doc[] = R"(
)";
	template<class N, class X>
	struct average : public monoid<std::pair<N,X>> {
		std::pair<N,X> _op() const override
		{
			return average{};
		}
		std::pair<N,X> _op(const std::pair<N, X>& x, const std::pair<N, X>& y) const override
		{
			const auto& [xn, xm] = x;
			const auto& [yn, ym] = y;

			return { xn + yn, (xn * xm + yn * ym) / (xn + yn) };
		}
	};


} // namespace fms
