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
#include <range/v3/algorithm/minmax_element.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

template <class Iter, class Sent = Iter>
void
test_iter(Iter first, Sent last)
{
    std::pair<Iter, Iter> p = ranges::minmax_element(first, last);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
        {
            CHECK(!(*j < *p.first));
            CHECK(!(*p.second < *j));
        }
    }
    else
    {
        CHECK(p.first == last);
        CHECK(p.second == last);
    }

    auto rng = ranges::range(first, last);
    p = ranges::minmax_element(rng);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
        {
            CHECK(!(*j < *p.first));
            CHECK(!(*p.second < *j));
        }
    }
    else
    {
        CHECK(p.first == last);
        CHECK(p.second == last);
    }
}

template <class Iter, class Sent = Iter>
void
test_iter(unsigned N)
{
    std::unique_ptr<int[]> a{new int[N]};
    std::iota(a.get(), a.get()+N, 0);
    std::random_shuffle(a.get(), a.get()+N);
    test_iter(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter()
{
    test_iter<Iter, Sent>(0);
    test_iter<Iter, Sent>(1);
    test_iter<Iter, Sent>(2);
    test_iter<Iter, Sent>(3);
    test_iter<Iter, Sent>(10);
    test_iter<Iter, Sent>(1000);
    {
        const unsigned N = 100;
        std::unique_ptr<int[]> a{new int[N]};
        std::fill_n(a.get(), N, 5);
        std::random_shuffle(a.get(), a.get()+N);
        std::pair<Iter, Iter> p = ranges::minmax_element(Iter(a.get()), Sent(a.get()+N));
        CHECK(base(p.first) == a.get());
        CHECK(base(p.second) == a.get()+N-1);
    }
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(Iter first, Sent last)
{
    typedef std::greater<int> Compare;
    Compare comp;
    std::pair<Iter, Iter> p = ranges::minmax_element(first, last, comp);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
        {
            CHECK(!comp(*j, *p.first));
            CHECK(!comp(*p.second, *j));
        }
    }
    else
    {
        CHECK(p.first == last);
        CHECK(p.second == last);
    }

    auto rng = ranges::range(first, last);
    p = ranges::minmax_element(rng, comp);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
        {
            CHECK(!comp(*j, *p.first));
            CHECK(!comp(*p.second, *j));
        }
    }
    else
    {
        CHECK(p.first == last);
        CHECK(p.second == last);
    }
}

template <class Iter, class Sent = Iter>
void
test_iter_comp(unsigned N)
{
    std::unique_ptr<int[]> a{new int[N]};
    std::iota(a.get(), a.get()+N, 0);
    std::random_shuffle(a.get(), a.get()+N);
    test_iter_comp(Iter(a.get()), Sent(a.get()+N));
}

template <class Iter, class Sent = Iter>
void
test_iter_comp()
{
    test_iter_comp<Iter, Sent>(0);
    test_iter_comp<Iter, Sent>(1);
    test_iter_comp<Iter, Sent>(2);
    test_iter_comp<Iter, Sent>(3);
    test_iter_comp<Iter, Sent>(10);
    test_iter_comp<Iter, Sent>(1000);
    {
        const unsigned N = 100;
        std::unique_ptr<int[]> a{new int[N]};
        std::fill_n(a.get(), N, 5);
        std::random_shuffle(a.get(), a.get()+N);
        typedef std::greater<int> Compare;
        Compare comp;
        std::pair<Iter, Iter> p = ranges::minmax_element(Iter(a.get()), Sent(a.get()+N), comp);
        CHECK(base(p.first) == a.get());
        CHECK(base(p.second) == a.get()+N-1);
    }
}

struct S
{
    int i;
};

int main()
{
    test_iter<forward_iterator<const int*> >();
    test_iter<bidirectional_iterator<const int*> >();
    test_iter<random_access_iterator<const int*> >();
    test_iter<const int*>();
    test_iter<forward_iterator<const int*>, sentinel<const int*>>();
    test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

    test_iter<forward_iterator<const int*> >();
    test_iter<bidirectional_iterator<const int*> >();
    test_iter<random_access_iterator<const int*> >();
    test_iter<const int*>();
    test_iter<forward_iterator<const int*>, sentinel<const int*>>();
    test_iter<bidirectional_iterator<const int*>, sentinel<const int*>>();
    test_iter<random_access_iterator<const int*>, sentinel<const int*>>();

    // works with initializer_lists?
    std::pair<int const *, int const *> pi = ranges::minmax_element({1,2,3,4,-4,5,6,40,7,8,9});
    CHECK(*pi.first == -4);
    CHECK(*pi.second == 40);

    // Works with projections?
    S s[] = {S{1},S{2},S{3},S{4},S{-4},S{5},S{6},S{40},S{7},S{8},S{9}};
    std::pair<S const *, S const *> ps = ranges::minmax_element(s, std::less<int>{}, &S::i);
    CHECK(ps.first->i == -4);
    CHECK(ps.second->i == 40);

    return test_result();
}
