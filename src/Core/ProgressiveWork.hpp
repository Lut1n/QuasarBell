#ifndef QUASAR_BELL_WORK_HPP
#define QUASAR_BELL_WORK_HPP

#include "Core/Math.hpp"

#include <memory>
#include <limits>

namespace qb
{
    
    //--------------------------------------------------------------
    template<typename T>
    struct Range
    {
        T min = std::numeric_limits<T>::max();
        T max = std::numeric_limits<T>::min();
        void reset()
        {
            min = std::numeric_limits<T>::max();
            max = std::numeric_limits<T>::min();
        }
        void expends(T v)
        {
            min = qb::min(v, min);
            max = qb::max(v, max);
        }

        T size() const
        {
            return max-min+1;
        }
    };

    //--------------------------------------------------------------
    struct ProgressiveWork
    {
        struct Work { virtual ~Work() = default; };

        float progress = 0.0f;
        bool initialized = false;
        bool completed = false;
        bool oneshot = false;
        std::unique_ptr<Work> work;

        ProgressiveWork(bool oneshot = false)
            : oneshot(oneshot)
        {}

        template<class T, class... Arg>
        void initialize(Arg&&... arg)
        {
            reset();
            work = std::make_unique<T>(std::forward<Arg>(arg)...);
            initialized = true;
        }

        template<class T>
        T& getWork()
        {
            return *static_cast<T*>(work.get());
        }

        void reset()
        {
            progress = 0.0f;
            initialized = false;
            completed = false;
            work.reset();
        }
    };
};

#endif // QUASAR_BELL_WORK_HPP