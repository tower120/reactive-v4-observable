#ifndef REACTIVE_V4_OBSERVABLE_TEST_EVENT_H
#define REACTIVE_V4_OBSERVABLE_TEST_EVENT_H

#include <functional>
#include <vector>

#include "DelegateTag.h"

namespace reactive::v4{

    struct EventControl{
        bool unsubscribe{false};
    };
    static const constexpr EventControl unsubscribe{true};
    static const constexpr EventControl proceed{false};



    namespace details{
        static const DelegateTag deleteTag = DelegateTag::make();
    }

    // non-thread safe
    // safe to subscribe/unsubscribe from iteration loop
    template<class ...Args>
    class Event{
    protected:
        std::vector<std::pair<DelegateTag, std::function<EventControl(Args...)> >> delegates;
        unsigned int loop_count = 0;      // reentancy count

        void clear_head(){
            while(delegates.back().first == details::deleteTag){
                delegates.pop_back();
            }
        }

        template<class Closure>
        auto makeDelegate(Closure&& closure){
            using RetT = decltype(closure(std::declval<Args>()...));
            if constexpr (std::is_same_v<RetT, void>){
                return [closure = std::forward<Closure>(closure)]() -> EventControl{
                    closure();
                    return proceed;
                };
            } else {
                return std::forward<Closure>(closure);
            }
        }
    public:
        template<class Closure>
        void subscribe(Closure&& closure){
            delegates.emplace_back(DelegateTag::empty(), makeDelegate(std::forward<Closure>(closure)));
        }
        template<class Closure>
        void subscribe(const DelegateTag& tag, Closure&& closure){
            delegates.emplace_back(tag, makeDelegate(std::forward<Closure>(closure)));
        }
        void unsubscribe(const DelegateTag& tag){
            for(auto& delegate : delegates){
                if (delegate.first == tag){
                    delegate.first = details::deleteTag;
                    break;
                }
            }
        }

        template<class Closure>
        DelegateTag operator+=(Closure&& closure) {
            DelegateTag tag = DelegateTag::make();
            delegates.emplace_back(tag, makeDelegate(std::forward<Closure>(closure)));
            return tag;
        }
        void operator-=(const DelegateTag& tag){
            unsubscribe(tag);
        }

        template<class ...ArgsT>
        void operator()(ArgsT&&... args){
            loop_count++;
            std::size_t size = delegates.size();
            for(int i=0;i<size;++i){
                auto& delegate = delegates[i];
                if (delegate.first == details::deleteTag){
                    if (loop_count != 1) continue;

                    // do compact
                    clear_head();

                    if (i < delegates.size()){
                        delegate = std::move(delegates.back());
                        delegates.pop_back();
                        size = delegates.size();

                        // is this the end?
                        const bool the_end = (i >= delegates.size());
                        if (the_end) {
                            // we finished, bail out
                            break;
                        }
                    } else {
                        // our delegate was cleared with clear_head
                        break;
                    }
                }

                const EventControl ctrl = delegate.second(std::forward<ArgsT>(args)...);
                if (ctrl.unsubscribe){
                    delegate.first = details::deleteTag;
                }
            }
            loop_count--;
        }
    };

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_EVENT_H
