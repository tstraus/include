#ifndef __ACTION_H__
#define __ACTION_H__

#include <unordered_map>
#include <functional>
#include <utility>
#include <mutex>
#include <memory>

namespace tstraus
{
    template<class Signature>
    class Action
    {
    public:
        // default constructor
        Action(uint32_t id = 1) : nextID(id), mtx(std::unique_ptr<std::mutex>(new std::mutex())) {}

        // make the given function the only one in the action
        template<typename Function>
        void operator = (Function&& f)
        {
            std::lock_guard<std::mutex> lock(*mtx);

            actions.clear();

            actions[0] = std::forward<Function>(f);
        }

        // adds a function to the action
        template<typename Function>
        uint32_t operator += (Function&& f)
        {
            std::lock_guard<std::mutex> lock(*mtx);

            actions[++nextID] = std::forward<Function>(f);

            return nextID;
        }

        // remove the function with the given id from the action
        void operator -= (uint32_t id)
        {
            std::lock_guard<std::mutex> lock(*mtx);

            actions.erase(id);
        }

        // execute the action
        template<class... Args>
        void operator () (Args&&... args)
        {
            std::lock_guard<std::mutex> lock(*mtx);

            for (auto& f : actions)
                f.second(args...);
        }

        Action<Signature> operator + (Action<Signature>& a)
        {
            std::lock_guard<std::mutex> l1(*mtx);
            std::lock_guard<std::mutex> l2(*a.mtx);

            Action<Signature> out;

            for (auto& i : actions)
                out += i.second;

            for (auto& i : a.actions)
                out += i.second;

            return out;
        }

        // clear the functions in the action
        void clear()
        {
            std::lock_guard<std::mutex> lock(*mtx);

            nextID = 1;
            actions.clear();
        }

    private:
        uint32_t nextID;

        std::unique_ptr<std::mutex> mtx;

        std::unordered_map<uint32_t, std::function<Signature>> actions;
    };
}

#endif // __ACTION_H__
