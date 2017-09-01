cmake_minimum_required(VERSION 3.8)
project(reactive_v4_observable_test)

set(CMAKE_CXX_STANDARD 17)

# Visual Studio 2017 support C++17 https://stackoverflow.com/a/44964919
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

include_directories(../src)

add_executable(reactive_v4_observable ${SOURCE_FILES})
