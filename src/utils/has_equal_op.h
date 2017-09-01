#ifndef REACTIVE_V4_OBSERVABLE_TEST_HAS_EQUAL_OP_H
#define REACTIVE_V4_OBSERVABLE_TEST_HAS_EQUAL_OP_H

namespace reactive::v4::utils{
    // equality from #https://stackoverflow.com/a/36360646
    namespace details
    {
        template <typename T, typename R, typename = R>
        struct equality : std::false_type {};

        template <typename T, typename R>
        struct equality<T,R,decltype(std::declval<T>()==std::declval<T>())>
                : std::true_type {};
    }
    template<typename T, typename R = bool>
    struct has_equal_op : details::equality<T, R> {};
}

#endif //REACTIVE_V4_OBSERVABLE_TEST_HAS_EQUAL_OP_H
