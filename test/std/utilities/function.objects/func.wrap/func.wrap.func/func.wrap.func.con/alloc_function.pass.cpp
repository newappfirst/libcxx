//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <functional>

// class function<R(ArgTypes...)>

// template<class A> function(allocator_arg_t, const A&, const function&);


#include <functional>
#include <cassert>

#include "min_allocator.h"
#include "test_allocator.h"
#include "count_new.hpp"

class A
{
    int data_[10];
public:
    static int count;

    A()
    {
        ++count;
        for (int i = 0; i < 10; ++i)
            data_[i] = i;
    }

    A(const A&) {++count;}

    ~A() {--count;}

    int operator()(int i) const
    {
        for (int j = 0; j < 10; ++j)
            i += data_[j];
        return i;
    }
};

int A::count = 0;

int g(int) {return 0;}

int main()
{
    assert(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = A();
    assert(A::count == 1);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(f.target<A>());
    assert(f.target<int(*)(int)>() == 0);
    std::function<int(int)> f2(std::allocator_arg, bare_allocator<A>(), f);
    assert(A::count == 2);
    assert(globalMemCounter.checkOutstandingNewEq(2));
    assert(f2.target<A>());
    assert(f2.target<int(*)(int)>() == 0);
    }
    assert(A::count == 0);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = g;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(f.target<int(*)(int)>());
    assert(f.target<A>() == 0);
    std::function<int(int)> f2(std::allocator_arg, bare_allocator<int(*)(int)>(), f);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(f2.target<int(*)(int)>());
    assert(f2.target<A>() == 0);
    }
    assert(globalMemCounter.checkOutstandingNewEq(0));
    {
    assert(globalMemCounter.checkOutstandingNewEq(0));
    non_default_test_allocator<std::function<int(int)> > al(1);
    std::function<int(int)> f2(std::allocator_arg, al, g);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(f2.target<int(*)(int)>());
    assert(f2.target<A>() == 0);
    }
    assert(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(f.target<int(*)(int)>() == 0);
    assert(f.target<A>() == 0);
    std::function<int(int)> f2(std::allocator_arg, bare_allocator<int>(), f);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(f2.target<int(*)(int)>() == 0);
    assert(f2.target<A>() == 0);
    }
}
