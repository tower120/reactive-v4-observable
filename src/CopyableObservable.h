#ifndef REACTIVE_V4_OBSERVABLE_TEST_COPYOBSERVABLE_H
#define REACTIVE_V4_OBSERVABLE_TEST_COPYOBSERVABLE_H


#include <type_traits>

#include "utils/has_equal_op.h"

namespace reactive::v4{

    /* Class must be Observable */
    template<class Class>
    class CopyableObservable : public Class{
    public:
        CopyableObservable& operator=(const CopyableObservable& other){
            operator=(static_cast<const Class&>(other));
            return *this;
        }
        CopyableObservable& operator=(const Class& other){
            Class::operator=(other);

            if constexpr(utils::has_equal_op<Class>::value){
                if (other != static_cast<const Class&>(*this)){
                    Class::onChange();
                }
            } else {
                Class::onChange();
            }

            return *this;
        }


        template<typename = std::enable_if_t<
                !std::is_copy_constructible<Class>::value
                && std::is_copy_assignable<Class>::value
        > >
        CopyableObservable(const Class& other)
            :Class()
        {
            Class::operator=(other);
        }

        using Class::Class;
        using Class::operator=;
    };

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_COPYOBSERVABLE_H
