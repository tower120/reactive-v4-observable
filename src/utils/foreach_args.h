#ifndef REACTIVE_V4_OBSERVABLE_TEST_FOREACH_TUPLE_H
#define REACTIVE_V4_OBSERVABLE_TEST_FOREACH_TUPLE_H

namespace reactive::v4::utils{
    template<int i, class Closure>
    static void foreach_i(Closure &&closure) {}
    template<int i, class Closure, class Arg, class ...Args>
    static void foreach_i(Closure &&closure, Arg &&arg, Args &&... args) {
        closure(
                std::integral_constant<int, i>{},
                std::forward<Arg>(arg)
        );
        foreach_i<i + 1>(std::forward<Closure>(closure), std::forward<Args>(args)...);
    }
    template<class Closure, class ...Args>
    static void foreach_i(Closure &&closure, Args &&... args) {
        foreach_i<0>(std::forward<Closure>(closure), std::forward<Args>(args)...);
    }

    template<class Closure, class ...Args>
    static void foreach(Closure &&closure, Args &&... args) {
        foreach_i([&](auto&&, auto&& arg){
            closure(std::forward<decltype(arg)>(arg));
        }, std::forward<Args>(args)...);
    }

}

#endif //REACTIVE_V4_OBSERVABLE_TEST_FOREACH_TUPLE_H
