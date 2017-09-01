#ifndef REACTIVE_V4_OBSERVABLE_TEST_OBSERVABLE_H
#define REACTIVE_V4_OBSERVABLE_TEST_OBSERVABLE_H

#include "SchedulableEvent.h"
#include "reentrancy.h"

namespace reactive::v4{

    template<class reentrancy = default_reentrancy>
    class Observable{
        static constexpr const bool is_reentrant = std::is_same_v<reentrancy, reentrant>;

    public:
        using OnChange = std::conditional_t<is_reentrant, SchedulableEvent<>, Event<> >;
        OnChange onChange;

        Observable(){}

        // non copy/movable constructable. Use heap if you need to move around.
        Observable(const Observable&) = delete;
        Observable(Observable&&)  = delete;

        // assignable
        Observable& operator=(const Observable&){
            // do not copy event list
            //changed();    /* called before derived ctr */
        }

        Observable& operator=(const Observable&&) = delete;

        template<class Closure,
                bool t_is_reentrant = is_reentrant, typename = std::enable_if_t<t_is_reentrant> >
        void schedule(Closure&& closure){
            onChange.schedule(std::forward<Closure>(closure));
        }
    };

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_OBSERVABLE_H