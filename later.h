#ifndef __LATER_H__
#define __LATER_H__

#include <functional>
#include <chrono>
#include <thread>
#include <memory>
#include <stdint.h>

namespace tstraus
{
    namespace Later
    {
        template<class Function, class... arguments>
        const std::shared_ptr<std::thread> startPointer(const std::chrono::steady_clock::duration& delay, Function&& function, arguments&&... args)
        {
            auto begin = std::chrono::steady_clock::now();

            std::function<typename std::result_of<Function(arguments...)>::type()> task(std::bind(std::forward<Function>(function), std::forward<arguments>(args)...));

            auto t = std::make_shared<std::thread>([delay, begin, task]() {
                std::this_thread::sleep_until(begin + delay);
                task();
            });

            return t;
        }

        template<class Function, class... arguments>
        const std::shared_ptr<std::thread> startPointer(const uint32_t& millis, Function&& function, arguments&&... args)
        {
            return startPointer(std::chrono::milliseconds(millis), function, args...);
        }

        template <class Function, class... arguments>
        void block(const std::chrono::steady_clock::duration& delay, Function&& function, arguments&&... args)
        {
            auto t = startPointer(delay, function, args...);
            t->join();
        }

        template <class Function, class... arguments>
        void block(const uint32_t& millis, Function&& function, arguments&&... args)
        {
            auto t = startPointer(std::chrono::milliseconds(millis), function, args...);
            t->join();
        }

        template <class Function, class... arguments>
        void start(const std::chrono::steady_clock::duration& delay, Function&& function, arguments&&... args)
        {
            auto t = startPointer(delay, function, args...);
            t->detach();
        }

        template <class Function, class... arguments>
        void start(const uint32_t& millis, Function&& function, arguments&&... args)
        {
            auto t = startPointer(std::chrono::milliseconds(millis), function, args...);
            t->detach();
        }
    };
}

#endif // __LATER_H__
