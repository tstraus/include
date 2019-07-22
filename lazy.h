#include <functional>
#include <memory>

namespace tstraus
{
    template<typename T>
    class Lazy
    {
    public:
        Lazy(std::function<T()> fun)
        {
            this->fun = fun;
        }

        Lazy(const Lazy& l)
        {
            fun = l.fun;
            t = l.t;
        }

        T get()
        {
            if (t == nullptr)
                t = std::make_shared<T>(fun());

            return *t;
        }

        bool evaluated() const
        {
            return t != nullptr;
        }

    private:
        std::function<T()> fun;
        std::shared_ptr<T> t;
    };
};
