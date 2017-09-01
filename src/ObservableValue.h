#ifndef REACTIVE_V4_OBSERVABLE_TEST_LAZYOBSERVABLEVALUE_H
#define REACTIVE_V4_OBSERVABLE_TEST_LAZYOBSERVABLEVALUE_H

#include "SchedulableEvent.h"
#include "Observable.h"
#include "reentrancy.h"
#include "utils/has_equal_op.h"


namespace reactive::v4{

    template<class T, class reentrancy = default_reentrancy>
    class ObservableValue : public Observable<reentrancy>{
        using Base = Observable<reentrancy>;
        static constexpr const bool is_reentrant = std::is_same_v<reentrancy, reentrant>;

        T value;
    public:
        using Base::onChange;

        ObservableValue(){}
        ObservableValue(const T& value)
            :value(value)
        {}
        ObservableValue(T&& value)
            :value(std::move(value))
        {}

        ObservableValue(const ObservableValue& other)
            : Base()
            , value(other.value)
        {}
        /*ObservableValue(ObservableValue&& other)
            : Base()
            , value(std::move(other.value))
        {}*/

        ObservableValue& operator=(const ObservableValue& other){
            this->operator=(other.value);
            return *this;
        }
        /*ObservableValue& operator=(ObservableValue&& other){
            this->operator=(std::move(other.value));
            return *this;
        }*/



        template<class Value>
        void set(Value&& value){
            if constexpr (utils::has_equal_op<T>::value){
                if (value == this->value) return;
            }

            if constexpr (is_reentrant){
                if (onChange.looping()){
                    Base::schedule([&, value = std::forward<Value>(value)](){
                        this->value = std::forward<decltype(value)>(value);
                        onChange();
                    });
                    return;
                }
            }

            this->value = std::forward<Value>(value);
            onChange();
        }

        void operator=(const T& value){
            set(value);
        }
        void operator=(T&& value){
            set(std::move(value));
        }

        template<class Closure>
        void update(Closure&& closure){
            closure(value);
            onChange();
        }

        const T& get() const {
            return value;
        }
        operator const T& () const{
            return get();
        };
        const T* operator->() const {
            return &get();
        }
        const T& operator*() const{
            return get();
        }
    };

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_LAZYOBSERVABLEVALUE_H
