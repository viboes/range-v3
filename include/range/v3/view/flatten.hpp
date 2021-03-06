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

#ifndef RANGES_V3_VIEW_FLATTEN_HPP
#define RANGES_V3_VIEW_FLATTEN_HPP

#include <utility>
#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/size.hpp>
#include <range/v3/numeric.hpp> // for accumulate
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_adaptor.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/utility/bindable.hpp>
#include <range/v3/utility/invokable.hpp>
#include <range/v3/view/all.hpp>

namespace ranges
{
    inline namespace v3
    {
        template<typename Rng>
        struct flatten_view
          : range_adaptor<flatten_view<Rng>, Rng,
                is_infinite<Rng>::value || is_infinite<range_value_t<Rng>>::value>
        {
        private:
            CONCEPT_ASSERT(Iterable<Rng>());
            CONCEPT_ASSERT(Iterable<range_value_t<Rng>>());
            using size_type = common_type_t<range_size_t<Rng>, range_size_t<range_value_t<Rng>>>;

            friend range_core_access;
            range_view_all_t<range_value_t<Rng>> cur_;

            struct adaptor;
            friend struct adaptor;
            struct adaptor : default_adaptor
            {
            private:
                using base_cursor_t = ranges::base_cursor_t<flatten_view>;
                using derived_cursor_t = ranges::derived_cursor_t<base_cursor_t, adaptor>;
                flatten_view *rng_;
                range_iterator_t<range_value_t<Rng>> it_;
                void satisfy(base_cursor_t &pos)
                {
                    auto &cur = rng_->cur_;
                    auto const end = default_adaptor::end(*rng_);
                    while(it_ == ranges::end(cur))
                    {
                        pos.next();
                        if(end.equal(pos))
                            break;
                        cur = view::all(pos.current());
                        it_ = ranges::begin(cur);
                    }
                }
            public:
                using single_pass = std::true_type;
                adaptor() = default;
                adaptor(flatten_view &rng)
                  : rng_(&rng), it_{}
                {}
                base_cursor_t begin(flatten_view &)
                {
                    base_cursor_t pos = default_adaptor::begin(*rng_);
                    auto const end = default_adaptor::end(*rng_);
                    // BUGBUG pos.equal(end) doesn't work. Find out why.
                    if(!end.equal(pos))
                    {
                        rng_->cur_ = pos.current();
                        it_ = ranges::begin(rng_->cur_);
                        satisfy(pos);
                    }
                    return pos;
                }
                bool equal(derived_cursor_t const &pos, derived_cursor_t const &that) const
                {
                    RANGES_ASSERT(this == &pos.adaptor());
                    RANGES_ASSERT(rng_ == that.adaptor().rng_);
                    return pos.equal(that) && it_ == that.adaptor().it_;
                }
                void next(derived_cursor_t &pos)
                {
                    ++it_;
                    satisfy(pos);
                }
                auto current(derived_cursor_t const &) const -> decltype(*it_)
                {
                    return *it_;
                }
            };
            adaptor begin_adaptor()
            {
                return {*this};
            }
            adaptor end_adaptor()
            {
                return {*this};
            }
        public:
            flatten_view() = default;
            explicit flatten_view(Rng &&rng)
              : range_adaptor_t<flatten_view>(std::forward<Rng>(rng)), cur_{}
            {}
            CONCEPT_REQUIRES(ForwardSizedIterable<Rng>() &&
                             SizedIterable<range_value_t<Rng>>())
            size_type size() const
            {
                return accumulate(view::transform(this->base(), ranges::size), size_type{});
            }
        };

        namespace view
        {
            struct flatten_fn : bindable<flatten_fn>, pipeable<flatten_fn>
            {
                template<typename Rng>
                static flatten_view<Rng> invoke(flatten_fn, Rng && rng)
                {
                    // Must be a range of ranges
                    CONCEPT_ASSERT(Iterable<Rng>());
                    CONCEPT_ASSERT(Iterable<range_value_t<Rng>>());
                    return flatten_view<Rng>{std::forward<Rng>(rng)};
                }
            };

            RANGES_CONSTEXPR flatten_fn flatten{};
        }
    }
}

#endif
