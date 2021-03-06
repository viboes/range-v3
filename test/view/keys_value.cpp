//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/

#include <map>
#include <string>
#include <range/v3/core.hpp>
#include <range/v3/view/map.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"

int main()
{
    using namespace ranges;
    std::map<std::string, int> m = {
        {"this", 0},
        {"that", 1},
        {"other", 2}};
    auto && keys = m | view::keys;
    has_type<std::string const &>(*begin(keys));
    models<concepts::SizedIterable>(keys);
    models<concepts::Range>(keys);
    models<concepts::BidirectionalIterator>(begin(keys));
    CHECK(&*begin(keys) == &m.begin()->first);
    ::check_equal(keys, {"other", "that", "this"});

    auto && values = m | view::values;
    has_type<int &>(*begin(values));
    models<concepts::SizedIterable>(values);
    models<concepts::Range>(values);
    models<concepts::BidirectionalIterator>(begin(values));
    CHECK(&*begin(values) == &m.begin()->second);
    ::check_equal(values, {2, 1, 0});

    return test_result();
}
