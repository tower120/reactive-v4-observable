#ifndef REACTIVE_V4_OBSERVABLE_TEST_REENTRANCY_H
#define REACTIVE_V4_OBSERVABLE_TEST_REENTRANCY_H

namespace reactive::v4{
    struct reentrant{};
    struct non_reentrant{};

    using default_reentrancy = reentrant;
}

#endif //REACTIVE_V4_OBSERVABLE_TEST_REENTRANCY_H
