// Boost.Range library
//
//  Copyright Eric Niebler 2014.
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef RANGES_V3_VIEW_GENERATE_HPP
#define RANGES_V3_VIEW_GENERATE_HPP

#include <utility>
#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/size.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_adaptor.hpp>
#include <range/v3/utility/bindable.hpp>
#include <range/v3/utility/invokable.hpp>
#include <range/v3/utility/optional.hpp>

namespace ranges
{
    inline namespace v3
    {
        template<typename G>
        struct generate_view
          : range_facade<generate_view<G>, true>
        {
        private:
            friend struct range_core_access;
            optional<G> gen_;
            struct cursor
            {
            private:
                G const *gen_;
            public:
                using single_pass = std::true_type;
                cursor() = default;
                cursor(G const &g)
                  : gen_(&g)
                {}
                constexpr bool done() const
                {
                    return false;
                }
                auto current() const -> decltype((*gen_)())
                {
                    return (*gen_)();
                }
                void next() const
                {}
            };
            cursor begin_cursor() const
            {
                RANGES_ASSERT(!!gen_);
                return {*gen_};
            }
        public:
            generate_view() = default;
            explicit generate_view(G g)
              : gen_(std::move(g))
            {}
        };

        namespace view
        {
            struct generate_fn : bindable<generate_fn>
            {
                template<typename G,
                    CONCEPT_REQUIRES_(Function<G>())>
                static generate_view<G> invoke(generate_fn, G g)
                {
                    return generate_view<G>{std::move(g)};
                }
            };

            RANGES_CONSTEXPR generate_fn generate{};
        }
    }
}

#endif
