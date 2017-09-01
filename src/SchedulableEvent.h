#ifndef REACTIVE_V4_OBSERVABLE_TEST_SCHEDULABLEEVENT_H
#define REACTIVE_V4_OBSERVABLE_TEST_SCHEDULABLEEVENT_H

#include "Event.h"

namespace reactive::v4{

    template<class ...Args>
    class SchedulableEvent : public Event<Args...> {
        using BaseEvent = Event<Args...>;
        std::vector<std::function<void()>> m_schedule;  // use GrowOnlyChunkedArray

    public:
        bool looping() const {
            return BaseEvent::loop_count > 0;
        }

        template<class Closure>
        void schedule(Closure&& closure){
            if (!looping()){
                closure();
                return;
            }

            m_schedule.emplace_back(std::forward<Closure>(closure));
        }

        template<class ...ArgsT>
        void operator()(ArgsT&&... args) {
            BaseEvent::operator()(std::forward<ArgsT>(args)...);

            if (!looping()){
                // do scheduled tasks
                if (m_schedule.empty()) return;

                auto list = std::move(m_schedule);
                m_schedule.clear();

                for(auto& closure : list){
                    closure();
                }
            }
        }
    };

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_SCHEDULABLEEVENT_H
