// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008.
//  Copyright Eric Niebler 2013.
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef RANGES_V3_VIEW_SINGLE_HPP
#define RANGES_V3_VIEW_SINGLE_HPP

#include <utility>
#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_facade.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/bindable.hpp>

namespace ranges
{
    inline namespace v3
    {
        template<typename Val>
        struct single_view
          : range_facade<single_view<Val>>
        {
        private:
            friend struct range_core_access;
            Val value_;
            struct cursor
            {
            private:
                Val value_;
                bool done_;
            public:
                using single_pass = std::true_type;
                cursor() = default;
                cursor(Val value)
                  : value_(std::move(value)), done_(false)
                {}
                bool done() const
                {
                    return done_;
                }
                void next()
                {
                    done_ = true;
                }
                Val current() const
                {
                    return value_;
                }
            };
            cursor begin_cursor() const
            {
                return {value_};
            }
        public:
            single_view() = default;
            constexpr explicit single_view(Val value)
              : value_(detail::move(value))
            {}
            constexpr std::size_t size() const
            {
                return 1;
            }
        };

        namespace view
        {
            struct single_fn : bindable<single_fn>
            {
                template<typename Val>
                static single_view<Val>
                invoke(single_fn, Val value)
                {
                    return single_view<Val>{std::move(value)};
                }
            };

            RANGES_CONSTEXPR single_fn single {};
        }
    }
}

#endif
