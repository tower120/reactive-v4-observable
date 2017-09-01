#ifndef REACTIVE_V4_OBSERVABLE_TEST_OBSERVE_H
#define REACTIVE_V4_OBSERVABLE_TEST_OBSERVE_H

#include <memory>

#include "utils/foreach_args.h"

namespace reactive::v4{

    template<class Closure, class ...Events>
    auto observe(const Closure& closure, Events&... events) {
        using namespace utils;

        // TODO: ?
        // put closure in shared_ptr too

        std::shared_ptr<std::atomic<bool>> unsubscribed = std::make_shared<std::atomic<bool>>(false);

        foreach([&](auto& event){
            event.subscribe([closure, unsubscribed](){
                if (*unsubscribed){
                    return unsubscribe;
                }

                using Ret = decltype(closure());
                if constexpr (std::is_same_v<Ret, void>)
                {
                    closure();
                } else {
                    EventControl ctrl = closure();
                    if(ctrl.unsubscribe){
                        *unsubscribed = true;
                        return unsubscribe;
                    }
                }

                return proceed;
            });
        }, events...);


        // unsubscription
        return [unsubscribed](){
            *unsubscribed = true;
        };
    };


    /*template<class Closure, class ...Events>
    auto observe_v1(const Closure& closure, Events&... events){
        using namespace utils;

        struct MultiObserver{
            std::tuple<Events&... > list;
            DelegateTag tag;

            MultiObserver(const DelegateTag& tag, Events&... events)
                : tag(tag)
                , list(events...)
            {}

            void unsubscribe(){
                std::apply([&](auto&... events){
                    foreach([&](auto& event){
                        event -= tag;
                    }, events...);
                }, list);
            }
        };

        DelegateTag tag{DelegateTag::make()};

        using Ret = decltype(closure());
        if constexpr (std::is_same_v<Ret, void>){
            // do not copy MultiObserver
            foreach([&](auto& event){
                event.subscribe(tag, closure);
            }, events...);
        } else {
            std::shared_ptr<MultiObserver> multi_observer(std::make_shared<MultiObserver>(
                    tag, events...
            ));

            foreach([&](auto& event){
                // separate closure from multi_observer allow to exploit std::function small buffer allocation optimisation(<=40 bytes)
                // and keep subscribers in linear space
                // multiobserver unsubscribe will be called once per life
                event.subscribe(tag, [closure, multi_observer](){
                    EventControl ctrl = closure();
                    if (ctrl.unsubscribe){
                        multi_observer->unsubscribe();
                    }
                });
            }, events...);
        }

        return tag;
    }*/

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_OBSERVE_H
