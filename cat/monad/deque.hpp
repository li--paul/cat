/******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-15 Nicola Bonelli <nicola@pfq.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ******************************************************************************/

#pragma once

#include <deque>
#include <cat/monad/monad.hpp>

namespace cat
{
    // deque is a monad:
    //

    template <> struct is_monad<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename A, typename Fun, typename Ma_, typename A_>
    struct MonadInstance<std::deque<A>, Fun, Ma_, A_> final : Monad<std::deque>::
    template _<Fun, A, Ma_, A_>
    {
        using B = inner_type_t<std::result_of_t<Fun(A)>>;

        std::deque<B>
        mbind(Ma_ && xs, Fun f) override
        {
            std::deque<B> ret;
            for(auto & x : xs)
                for(auto & y : f(forward_as<Ma_>(x)))
                    ret.push_back(std::move(y));
            return ret;
        }

        std::deque<A>
        mreturn(A_ && a) override
        {
            return {std::forward<A_>(a)};
        }

    };

    // deque is a monad_plus:
    //

    template <> struct is_monad_plus<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename A, typename Ma_, typename Mb_>
    struct MonadPlusInstance<std::deque<A>, Ma_, Mb_> final : MonadPlus<std::deque>::
    template _<A, Ma_, Mb_>
    {
        std::deque<A>
        mzero() override
        {
            return {};
        }

        std::deque<A>
        mplus(Ma_ && a, Mb_ && b) override
        {
            auto r = std::forward<Ma_>(a);
            r.insert(std::end(r),
                     auto_begin(b),
                     auto_end(b));
            return r;
        }
    };



} // namespace cat

