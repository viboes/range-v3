//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <algorithm>
#include <range/v3/core.hpp>
#include <range/v3/algorithm/mismatch.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

template<typename Iter, typename Sent = Iter>
void test_iter()
{
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    constexpr unsigned sa = ranges::size(ia);
    int ib[] = {0, 1, 2, 3, 0, 1, 2, 3};
    using Pair = std::pair<Iter, Iter>;
    CHECK(ranges::mismatch(Iter(ia), Sent(ia + sa), Iter(ib)) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    CHECK(ranges::mismatch(Iter(ia),Sent(ia + sa),Iter(ib),Sent(ib + sa)) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    CHECK(ranges::mismatch(Iter(ia),Sent(ia + sa),Iter(ib),Sent(ib + 2)) ==
                           Pair{Iter(ia+2),Iter(ib+2)});

    CHECK(ranges::mismatch(Iter(ia),Sent(ia + sa),Iter(ib),std::equal_to<int>()) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    CHECK(ranges::mismatch(Iter(ia),Sent(ia + sa),Iter(ib),Sent(ib + sa),std::equal_to<int>()) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    CHECK(ranges::mismatch(Iter(ia), Sent(ia + sa), Iter(ib), Sent(ib + 2), std::equal_to<int>()) ==
                           Pair{Iter(ia+2),Iter(ib+2)});
}

template<typename Iter, typename Sent = Iter>
void test_range()
{
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    constexpr unsigned sa = ranges::size(ia);
    int ib[] = {0, 1, 2, 3, 0, 1, 2, 3};
    using Pair = std::pair<Iter, Iter>;
    auto rng1 = ranges::range(Iter(ia), Sent(ia + sa));
    CHECK(ranges::mismatch(rng1, Iter(ib)) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    auto rng2 = ranges::range(Iter(ia),Sent(ia + sa));
    auto rng3 = ranges::range(Iter(ib),Sent(ib + sa));
    CHECK(ranges::mismatch(rng2,rng3) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    auto rng4 = ranges::range(Iter(ia),Sent(ia + sa));
    auto rng5 = ranges::range(Iter(ib),Sent(ib + 2));
    CHECK(ranges::mismatch(rng4,rng5) ==
                           Pair{Iter(ia+2),Iter(ib+2)});

    auto rng6 = ranges::range(Iter(ia),Sent(ia + sa));
    CHECK(ranges::mismatch(rng6,Iter(ib),std::equal_to<int>()) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    auto rng7 = ranges::range(Iter(ia),Sent(ia + sa));
    auto rng8 = ranges::range(Iter(ib),Sent(ib + sa));
    CHECK(ranges::mismatch(rng7,rng8,std::equal_to<int>()) ==
                           Pair{Iter(ia+3),Iter(ib+3)});
    auto rng9 = ranges::range(Iter(ia), Sent(ia + sa));
    auto rng10 = ranges::range(Iter(ib), Sent(ib + 2));
    CHECK(ranges::mismatch(rng9,rng10,std::equal_to<int>()) ==
                           Pair{Iter(ia+2),Iter(ib+2)});
}

struct S
{
    int i;
};

int main()
{
    test_iter<input_iterator<const int*>>();
    test_iter<forward_iterator<const int*>>();
    test_iter<bidirectional_iterator<const int*>>();
    test_iter<random_access_iterator<const int*>>();
    test_iter<const int*>();
    test_iter<input_iterator<const int*>, sentinel<const int*>>();
    test_iter<forward_iterator<const int*>, sentinel<const int*>>();
    test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

    test_range<input_iterator<const int*>>();
    test_range<forward_iterator<const int*>>();
    test_range<bidirectional_iterator<const int*>>();
    test_range<random_access_iterator<const int*>>();
    test_range<const int*>();
    test_range<input_iterator<const int*>, sentinel<const int*>>();
    test_range<forward_iterator<const int*>, sentinel<const int*>>();
    test_range<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_range<random_access_iterator<const int*>, sentinel<const int*>>();

    // works with initializer_lists?
    std::pair<int const *, int const *> pi =
        ranges::mismatch({1,2,3,4,-4,5,6,40,7,8,9}, {1,2,3,4,5,6,7,8,9});
    CHECK(*pi.first == -4);
    CHECK(*pi.second == 5);

    // Works with projections?
    S s1[] = {S{1},S{2},S{3},S{4},S{-4},S{5},S{6},S{40},S{7},S{8},S{9}};
    std::pair<S const *, int const *> ps1
        = ranges::mismatch(s1, {1,2,3,4,5,6,7,8,9}, std::equal_to<int>(), &S::i);
    CHECK(ps1.first->i == -4);
    CHECK(*ps1.second == 5);

    S s2[] = {S{1},S{2},S{3},S{4},S{5},S{6},S{40},S{7},S{8},S{9}};
    std::pair<S const *, S const *> ps2
        = ranges::mismatch(s1, s2, std::equal_to<int>(), &S::i, &S::i);
    CHECK(ps2.first->i == -4);
    CHECK(ps2.second->i == 5);

    return test_result();
}
