//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads

// <shared_mutex>

// template <class Mutex> class shared_lock;

// explicit shared_lock(mutex_type& m);

#include <shared_mutex>
#include <thread>
#include <vector>
#include <cstdlib>
#include <cassert>

#if _LIBCPP_STD_VER > 11

std::shared_timed_mutex m;

typedef std::chrono::system_clock Clock;
typedef Clock::time_point time_point;
typedef Clock::duration duration;
typedef std::chrono::milliseconds ms;
typedef std::chrono::nanoseconds ns;

void f()
{
    time_point t0 = Clock::now();
    time_point t1;
    {
    std::shared_lock<std::shared_timed_mutex> ul(m);
    t1 = Clock::now();
    }
    ns d = t1 - t0 - ms(250);
    assert(d < ms(50));  // within 50ms
}

void g()
{
    time_point t0 = Clock::now();
    time_point t1;
    {
    std::shared_lock<std::shared_timed_mutex> ul(m);
    t1 = Clock::now();
    }
    ns d = t1 - t0;
    assert(d < ms(50));  // within 50ms
}

#endif  // _LIBCPP_STD_VER > 11

int main()
{
#if _LIBCPP_STD_VER > 11
    m.lock();
    std::vector<std::thread> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(std::thread(f));
    std::this_thread::sleep_for(ms(250));
    m.unlock();
    for (auto& t : v)
        t.join();
    m.lock_shared();
    for (auto& t : v)
        t = std::thread(g);
    std::thread q(f);
    std::this_thread::sleep_for(ms(250));
    m.unlock_shared();
    for (auto& t : v)
        t.join();
    q.join();
#endif  // _LIBCPP_STD_VER > 11
}
