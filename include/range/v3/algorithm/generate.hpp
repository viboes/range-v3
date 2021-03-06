//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef RANGES_V3_ALGORITHM_GENERATE_HPP
#define RANGES_V3_ALGORITHM_GENERATE_HPP

#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>

namespace ranges
{
    inline namespace v3
    {
        struct generate_fn
        {
            template<typename O, typename S, typename F,
                CONCEPT_REQUIRES_(Function<F>() &&
                    OutputIterator<O, concepts::Function::result_t<F>>() && Sentinel<S, O>())>
            std::pair<O, F> operator()(O begin, S end, F fun) const
            {
                for(; begin != end; ++begin)
                    *begin = fun();
                return {begin, fun};
            }

            template<typename Rng, typename F,
                typename O = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(Function<F>() &&
                    OutputIterable<Rng, concepts::Function::result_t<F>>())>
            std::pair<O, F> operator()(Rng & rng, F fun) const
            {
                return (*this)(begin(rng), end(rng), std::move(fun));
            }
        };

        RANGES_CONSTEXPR generate_fn generate{};
    } // namespace v3
} // namespace ranges

#endif // include guard
