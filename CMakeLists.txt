cmake_minimum_required(VERSION 3.14)
project(UT_training)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)
include_directories(include)

add_executable(
    hello_test
    src/hello_test.cc
)
target_link_libraries(
    hello_test
    GTest::gtest_main
)
gtest_discover_tests(hello_test)

add_executable(
    free_functions
    src/free_functions.cc
)
target_link_libraries(
    free_functions
    GTest::gtest_main
    GTest::gmock_main
)
gtest_discover_tests(free_functions)

add_executable(
    private_methods
    src/private_methods.cc
)
target_link_libraries(
    private_methods
    GTest::gtest_main
    GTest::gmock_main
)
gtest_discover_tests(private_methods)

add_executable(
    mock_injection
    src/mock_injection.cc
)
target_link_libraries(
    mock_injection
    GTest::gtest_main
    GTest::gmock_main
)
gtest_discover_tests(mock_injection)

add_executable(
    timers
    src/timers.cc
)
target_link_libraries(
    timers
    GTest::gtest_main
    GTest::gmock_main
)
gtest_discover_tests(timers)
