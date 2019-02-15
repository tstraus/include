#ifndef __PARALLEL_FOREACH__
#define __PARALLEL_FOREACH__

#include "threadpool.h"

namespace tstraus {
    template <class Container, class Function>
    void foreach(const Container& c, Function f, uint32_t threads = std::thread::hardware_concurrency()) //std::function<void(const T&)> f, uint32_t threads = std::thread::hardware_concurrency())
    {
        ThreadPool tp(threads);
        for (auto it = c.begin(); it != c.end(); ++it)
            tp.enqueue(f, *it);
    }
};

#endif // __PARALLEL_FOREACH__